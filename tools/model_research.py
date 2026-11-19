#!/usr/bin/env python3
"""Research helpers for the monster model pack (DATA/MONSTER.MRG): one 1 MiB slot per monster.

Reads your own disc image only; anything it writes goes to the ignored out/research/ntsc-u/models/.

Layout of a model file ("RH0"), read from the matched loader and draw code
(RhModel_BindAddress, RhModel_Draw, lRhModel_SetupNoMotionRecursive, lRhModelCpu_*):

  +0x00  "RH0\\0"
  +0x10  ten (offset, size) pairs: 0 nodes, 1 opaque draw list, 2 strip data, 3 translucent draw
         list, 4 ?, 5 texture list, 6 bind list, 7 second bind list, 8 relocation list, 9 options
  node (64 bytes): scale[3] f32, flags u16, matrix u16, rot[3] f32, child u32, trans[3] f32,
         next u32, sphere[4] f32. child/next are offsets into the node segment (top nibble a tag).
  draw list: 16-byte aligned records. 0x10000008 texture (0x90 bytes), 0x10000000 nothing (0x10),
         0x10000003 material (0x40), 0x3....... reference (0x10): word 3 zero = strip data at the
         tagged address in word 1, (word 0 & 0xffff) * 16 bytes; non-zero = a matrix for VU memory.
  strip: word 1 bits 0-14 point count (bit 15: one more word follows the points), word 3 bit 15
         weighted. Plain: 0x18-byte header, word 4 the matrix slot, points of 18 bytes
         (u, v s16 in 1/4096; 8 bytes; x, y, z s16). Weighted: 0x14-byte header, points of
         u, v, count, then count records of (matrix u16, weight u16 in 1/4096, 8 bytes, x, y, z).
"""

import argparse
import math
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))

import project  # noqa: E402
from runtime import ROOT  # noqa: E402

PACK_LBA, SLOT, SLOTS = 68243, 0x100000, 683
OUT = ROOT / "out/research/ntsc-u/models"


def slot(iso, index):
    if not 0 <= index < SLOTS:
        raise ValueError("monster slot out of range")
    with open(iso, "rb") as stream:
        stream.seek(PACK_LBA * 2048 + index * SLOT)
        return stream.read(SLOT)


def hexdump(data, start, length):
    for offset in range(start, min(start + length, len(data)), 16):
        row = data[offset:offset + 16]
        words = " ".join(f"{w:08x}" for w in struct.unpack_from(f"<{len(row) // 4}I", row))
        text = "".join(chr(b) if 32 <= b < 127 else "." for b in row)
        print(f"{offset:06x}: {words}  {text}")


def segments(data):
    if data[:4] != b"RH0\0":
        raise ValueError("not an RH0 model")
    return [struct.unpack_from("<II", data, 0x10 + 8 * i) for i in range(10)]


def multiply(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


def local_matrix(rot, trans):
    """Column-vector matrix T * Rz * Ry * Rx (the order is an assumption to be checked by eye)."""
    cx, sx = math.cos(rot[0]), math.sin(rot[0])
    cy, sy = math.cos(rot[1]), math.sin(rot[1])
    cz, sz = math.cos(rot[2]), math.sin(rot[2])
    rx = [[1, 0, 0, 0], [0, cx, -sx, 0], [0, sx, cx, 0], [0, 0, 0, 1]]
    ry = [[cy, 0, sy, 0], [0, 1, 0, 0], [-sy, 0, cy, 0], [0, 0, 0, 1]]
    rz = [[cz, -sz, 0, 0], [sz, cz, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]
    t = [[1, 0, 0, trans[0]], [0, 1, 0, trans[1]], [0, 0, 1, trans[2]], [0, 0, 0, 1]]
    return multiply(t, multiply(rz, multiply(ry, rx)))


def nodes(data):
    """The node tree in walk order with bind-pose matrices, as lRhModel_SetupNoMotionRecursive builds them."""
    base, _ = segments(data)[0]
    result, matrices = [], {}
    identity = [[float(i == j) for j in range(4)] for i in range(4)]

    def walk(offset, parent, parent_scale, depth):
        while True:
            at = base + offset
            scale = struct.unpack_from("<3f", data, at)
            flags, matrix = struct.unpack_from("<HH", data, at + 12)
            # Rotations are 16-bit phases, 65536 to a turn (RhMath_SinI).
            rot = tuple(math.radians(((v + 0x8000) % 0x10000 - 0x8000) * 360.0 / 65536.0)
                        for v in struct.unpack_from("<3i", data, at + 16))
            child, = struct.unpack_from("<I", data, at + 28)
            trans = struct.unpack_from("<3f", data, at + 32)
            nxt, = struct.unpack_from("<I", data, at + 44)
            world, local_scale = parent, list(parent_scale)
            if flags & 3 != 3:
                t = [0.0, 0.0, 0.0] if flags & 1 else [trans[i] * parent_scale[i] for i in range(3)]
                world = multiply(parent, local_matrix((0, 0, 0) if flags & 2 else rot, t))
            if not flags & 4:
                local_scale = [local_scale[i] * scale[i] for i in range(3)]
            if matrix:
                scaled = [[world[i][j] * (local_scale[j] if j < 3 else 1.0) for j in range(4)] for i in range(4)]
                matrices[matrix] = scaled
            result.append(dict(offset=offset, depth=depth, flags=flags, matrix=matrix, scale=scale,
                               rot=rot, trans=trans, child=child, next=nxt))
            if child:
                walk(child & 0x0fffffff, world, local_scale, depth + 1)
            if not nxt:
                return
            offset = nxt & 0x0fffffff

    walk(0, identity, [1.0, 1.0, 1.0], 0)
    return result, matrices


def strips(data, start, size):
    """Yield (weighted, matrix slot, points) for each strip record in [start, start + size)."""
    at, end = start, start + size
    while at < end:
        words = struct.unpack_from("<6I", data, at)
        count, weighted = words[1] & 0x7fff, bool(words[3] & 0x8000)
        points = []
        if weighted:
            cursor = at + 0x14
            for _ in range(count):
                u, v, n = struct.unpack_from("<hhH", data, cursor)
                weights = []
                for k in range(n):
                    matrix, weight = struct.unpack_from("<HH", data, cursor + 6 + 18 * k)
                    xyz = struct.unpack_from("<3h", data, cursor + 6 + 18 * k + 12)
                    weights.append((matrix, weight / 4096.0, xyz))
                points.append((u, v, weights))
                cursor += 6 + 18 * n
        else:
            cursor = at + 0x18
            for _ in range(count):
                u, v = struct.unpack_from("<hh", data, cursor)
                xyz = struct.unpack_from("<3h", data, cursor + 12)
                points.append((u, v, [(words[4], 1.0, xyz)]))
                cursor += 18
        yield weighted, words[4], points
        at = (cursor + 3) & ~3
        if words[1] & 0x8000:
            at += 4
        while at < end and struct.unpack_from("<I", data, at)[0] == 0:
            at += 4


def draw_list(data, start, size, segs):
    """Yield ("strips", offset, size) and ("matrix", slot, address) entries of a draw list."""
    at, end = start, start + size
    if size and struct.unpack_from("<I", data, at)[0] == 0x20000000:
        # The list opens with a jump over its matrix area (seven quadwords a matrix).
        at = start + (struct.unpack_from("<I", data, at + 4)[0] & 0x0fffffff)
    while at < end:
        command, address, _, extra = struct.unpack_from("<4I", data, at)
        if command == 0x10000008:
            yield "texture", at, 0
            at += 0x90
        elif command == 0x10000000:
            at += 0x10
        elif command == 0x10000003:
            yield "material", at, 0
            at += 0x40
        elif command & 0xf0000000 == 0x30000000:
            tag = address >> 28
            target = segs[tag - 1][0] + (address & 0x0fffffff) if 2 <= tag <= 5 else address
            if extra == 0:
                yield "strips", target, (command & 0xffff) << 4
            else:
                yield "matrix", extra & 0x3ff, target
            at += 0x10
        else:
            yield "unknown", at, command
            return


def export(data, path):
    vertices, coords, faces, matrices, missing = mesh(data)
    lines = [f"v {x:.4f} {y:.4f} {z:.4f}" for x, y, z in vertices]
    lines += [f"vt {u:.5f} {v:.5f}" for u, v in coords]
    lines += [f"f {a}/{a} {b}/{b} {c}/{c}" for a, b, c in faces]
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n")
    return dict(vertices=len(vertices), triangles=len(faces), matrices=sorted(matrices),
                missing_matrices=sorted(missing))


def mesh(data):
    """Bind-pose vertices, texture coordinates and triangles (1-based indices) of a model."""
    segs = segments(data)
    _, matrices = nodes(data)
    vertices, coords, faces, missing = [], [], [], set()
    for part in (1, 3):
        start, size = segs[part]
        bound = {}
        for kind, a, b in draw_list(data, start, size, segs):
            if kind == "matrix":
                # A matrix of the list's matrix area (node index = offset / 16) is sent to VU
                # address `a`; strips name the VU address, so the binding changes along the list.
                bound[a] = (b - segs[1][0]) >> 4
            if kind != "strips":
                continue
            for _, _, points in strips(data, a, b):
                first = len(vertices) + 1
                for u, v, weights in points:
                    position = [0.0, 0.0, 0.0]
                    for matrix, weight, xyz in weights:
                        m = matrices.get(bound.get(matrix))
                        if m is None:
                            missing.add(matrix)
                            m = [[float(i == j) for j in range(4)] for i in range(4)]
                        for i in range(3):
                            position[i] += weight * (m[i][0] * xyz[0] + m[i][1] * xyz[1] + m[i][2] * xyz[2] + m[i][3])
                    vertices.append(position)
                    coords.append((u / 4096.0, 1.0 - v / 4096.0))
                for i in range(len(points) - 2):
                    a0, b0, c0 = first + i, first + i + 1, first + i + 2
                    faces.append((a0, b0, c0) if i % 2 == 0 else (b0, a0, c0))
    return vertices, coords, faces, matrices, missing


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("command", choices=("dump", "extract", "info", "export"))
    parser.add_argument("index", type=int, help="monster slot (card number)")
    parser.add_argument("--iso")
    parser.add_argument("--start", type=lambda v: int(v, 0), default=0)
    parser.add_argument("--length", type=lambda v: int(v, 0), default=0x200)
    args = parser.parse_args(argv)
    data = slot(project.find_iso(args.iso), args.index)
    if args.command == "extract":
        OUT.mkdir(parents=True, exist_ok=True)
        (OUT / f"monster-{args.index:03d}.bin").write_bytes(data)
        print(OUT / f"monster-{args.index:03d}.bin")
    elif args.command == "dump":
        print(f"slot {args.index}: {len(data.rstrip(bytes(1)))} bytes used of {SLOT}")
        hexdump(data, args.start, args.length)
    elif args.command == "info":
        segs = segments(data)
        for i, (offset, size) in enumerate(segs):
            print(f"segment {i}: 0x{offset:06x} + 0x{size:x}")
        tree, matrices = nodes(data)
        print(f"{len(tree)} nodes, matrix slots {sorted(matrices)}")
        for n in tree[:12]:
            print("  " * n["depth"] + f"node +0x{n['offset']:x} flags {n['flags']:04x} matrix {n['matrix']} "
                  f"scale {tuple(round(v, 3) for v in n['scale'])} rot {tuple(round(v, 3) for v in n['rot'])} "
                  f"trans {tuple(round(v, 2) for v in n['trans'])}")
        for part in (1, 3):
            counts, slots = {}, set()
            for kind, a, b in draw_list(data, *segs[part], segs):
                counts[kind] = counts.get(kind, 0) + 1
                if kind == "matrix":
                    slots.add(a)
                if kind == "unknown":
                    print(f"  unknown command 0x{b:08x} at 0x{a:x}")
            print(f"draw list {part}: {counts}, matrix slots {sorted(slots)}")
    else:
        report = export(data, OUT / f"monster-{args.index:03d}.obj")
        print(report)
    return 0


if __name__ == "__main__":
    sys.exit(main())
