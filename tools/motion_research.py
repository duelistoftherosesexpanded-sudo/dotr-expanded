#!/usr/bin/env python3
"""DotR monster motions (the "MOT" files after the model in its slot): read them, pose a model with
one, and draw the pose (tools/model_preview.py), to learn which motion slot is which action.

A motion (Sega Ninja NJS_MOTION, relocated by NjaReloc_Adjust_M3DMOT):

  +0x00  "MOT\\x1a"   +0x04 size of the file   +0x10 node count (16 bits)   +0x13 bit 7: relocate
  +0x14  offset of the motion record            +0x18 offset of a second record (0x1c)
  motion record: +0 offset of the node table, +4 frame count, +8 kind (3: position and rotation;
                 7: also scale), +10 interpolation
  node table: per node in walk order, the offsets of its position, rotation (and scale) keys, then
              one key count per track (16 or 24 bytes a node)
  keys: 16 bytes, a frame number then three floats (position, scale) or three 16-bit angles held
        in ints (rotation, 65536 to a turn)

Node order: the motion's nodes are the model's node tree in walk order (child before sibling). A
node's local matrix is T * Rz * Ry * Rx (checked by posing Black Luster Soldier, card 143: every
motion gives coherent poses), y up.

How a skinned model reaches the VU (read from card 143's draw list, segment 1):
  the list opens with a jump (0x20000000, 0x2000xxxx) over its matrix area: one matrix every 7
  quadwords from quadword 1; a node's "matrix" field is its quadword index there (8, 15, 22, ...),
  and the engine writes the node's matrix into it each frame.
  a batch: up to 8 matrix records (VU addresses 4, 11, 18, 25, 32, 39, 46, 53; the record's
  fourth word is the VU address, its second the matrix area address), then strip records, the
  last with bit 15 of its count, then 0x15000000 (MSCNT).
  a weighted strip (words[3] bit 15, 0x8412): 0x68018000 | address, count, 0x301e4000, 0x8412,
  0x69008000 | (units << 16) | (address + 1), then per point: u, v (16 bits each, 4096 = 1.0),
  the number of weights n (1-3), and n entries of 18 bytes: VU matrix address, weight (4096 =
  1.0), 1, normal (3 x 16 bits, 32767 = 1.0), position (3 x 16 bits) in that matrix's space;
  units = the sum over points of 1 + 3n. (The plain strip tools/model_import.py writes has the VU
  address as a fifth header word and three units a point.)
  Unpack addresses carry bit 15 (relative to the VU's double buffer); a batch stays under about
  216 quadwords.

  motion_research.py CARD [--motion N --frame F --out OUT.png] [--yaw ..] [--pitch ..]
"""

import argparse
import math
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_research as mr  # noqa: E402
import model_texture as mt  # noqa: E402
import project  # noqa: E402


def motions(data):
    """[(slot, offset)] of the motions in a model slot (header flags bits 0-7)."""
    flags, = struct.unpack_from("<I", data, 4)
    at, = struct.unpack_from("<I", data, 0xc)
    result = []
    for i in range(8):
        if flags >> i & 1:
            result.append((i, at))
            at += struct.unpack_from("<I", data, at + 4)[0]
    return result


def read_motion(data, at):
    """{frames, kind, nodes: [(position keys, rotation keys, scale keys)]} of the motion at `at`."""
    if struct.unpack_from("<I", data, at)[0] != 0x1a544f4d:
        raise ValueError("not a motion")
    count, = struct.unpack_from("<H", data, at + 0x10)
    record, = struct.unpack_from("<I", data, at + 0x14)
    table, frames, kind = struct.unpack_from("<IIH", data, at + record)
    tracks = 3 if kind & 0x3f == 7 else 2
    nodes = []
    for n in range(count):
        base = at + table + 8 * tracks * n
        offsets = struct.unpack_from(f"<{tracks}I", data, base)
        counts = struct.unpack_from(f"<{tracks}I", data, base + 4 * tracks)
        entry = []
        for t in range(3):
            if t >= tracks or not offsets[t]:
                entry.append([])
                continue
            fmt = "<I3i" if t == 1 else "<I3f"
            entry.append([struct.unpack_from(fmt, data, at + offsets[t] + 16 * k) for k in range(counts[t])])
        nodes.append(entry)
    return dict(frames=frames, kind=kind, nodes=nodes)


def sample(keys, frame, angle=False):
    if not keys:
        return None
    if frame <= keys[0][0] or len(keys) < 3:     # the game takes the first of one or two keys
        return keys[0][1:]
    for a, b in zip(keys, keys[1:]):
        if a[0] <= frame <= b[0]:
            t = (frame - a[0]) / ((b[0] - a[0]) or 1)
            if angle:   # the shorter way round, as RhGeom_RotationInterpolate
                return tuple(x + (((y - x + 0x8000) % 0x10000) - 0x8000) * t for x, y in zip(a[1:], b[1:]))
            return tuple(x + (y - x) * t for x, y in zip(a[1:], b[1:]))
    return keys[-1][1:]


def posed_matrices(data, motion=None, frame=0.0):
    """Node matrices (by matrix slot) of the model, posed by `motion` at `frame` (None: bind pose)."""
    base, _ = mr.segments(data)[0]
    rate, = struct.unpack_from("<f", data, 0x64)
    matrices = {}
    identity = [[float(i == j) for j in range(4)] for i in range(4)]
    index = [0]

    def walk(offset, parent, parent_scale):
        while True:
            at = base + offset
            scale = list(struct.unpack_from("<3f", data, at))
            flags, matrix = struct.unpack_from("<HH", data, at + 12)
            raw = list(struct.unpack_from("<3i", data, at + 16))
            child, = struct.unpack_from("<I", data, at + 28)
            trans = list(struct.unpack_from("<3f", data, at + 32))
            nxt, = struct.unpack_from("<I", data, at + 44)
            n = index[0]
            index[0] += 1
            if motion is not None and n < len(motion["nodes"]):
                p, r, s = motion["nodes"][n]
                # Position keys times the header's rate (+0x64, 1 / root scale), but not the
                # root's (lRhModel_SetupPulseMotionRecursive).
                key = sample(p, frame)
                if key is not None:
                    trans = [v * (1.0 if n == 0 else rate) for v in key]
                raw = list(sample(r, frame, True) or raw)
                scale = list(sample(s, frame) or scale)
            rot = [math.radians(v * 360.0 / 65536.0) for v in raw]
            world, local_scale = parent, list(parent_scale)
            if flags & 3 != 3:
                t = [0.0, 0.0, 0.0] if flags & 1 else [trans[i] * parent_scale[i] for i in range(3)]
                world = mr.multiply(parent, mr.local_matrix((0, 0, 0) if flags & 2 else rot, t))
            if not flags & 4:
                local_scale = [local_scale[i] * scale[i] for i in range(3)]
            if matrix:
                matrices[matrix] = [[world[i][j] * (local_scale[j] if j < 3 else 1.0) for j in range(4)]
                                    for i in range(4)]
            if child:
                walk(child & 0x0fffffff, world, local_scale)
            if not nxt:
                return
            offset = nxt & 0x0fffffff

    walk(0, identity, [1.0, 1.0, 1.0])
    return matrices


def posed_mesh(data, matrices):
    """(positions, coords, triangles) of the model with the given node matrices, in the form
    model_import.load_mesh gives."""
    segs = mr.segments(data)
    positions, coords, triangles = [], [], []
    for part in (1, 3):
        start, size = segs[part]
        bound = {}
        for kind, a, b in mr.draw_list(data, start, size, segs):
            if kind == "matrix":
                bound[a] = (b - segs[1][0]) >> 4
            if kind != "strips":
                continue
            for _, _, points in mr.strips(data, a, b):
                first = len(positions)
                for u, v, weights in points:
                    p = [0.0, 0.0, 0.0]
                    for slot, weight, xyz in weights:
                        m = matrices.get(bound.get(slot)) or [[float(i == j) for j in range(4)] for i in range(4)]
                        for i in range(3):
                            p[i] += weight * (m[i][0] * xyz[0] + m[i][1] * xyz[1] + m[i][2] * xyz[2] + m[i][3])
                    positions.append(tuple(p))
                    coords.append((u / 4096.0, v / 4096.0))
                for i in range(len(points) - 2):
                    a0, b0, c0 = first + i, first + i + 1, first + i + 2
                    tri = (a0, b0, c0) if i % 2 == 0 else (b0, a0, c0)
                    triangles.append(tuple((k, k) for k in tri))
    return positions, coords, triangles


def first_texture(data):
    at, = struct.unpack_from("<I", data, mr.segments(data)[5][0] + 16)
    return mt.decode(data, at)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("card", type=int)
    parser.add_argument("--motion", type=int)
    parser.add_argument("--frame", type=float, default=0.0)
    parser.add_argument("--out")
    parser.add_argument("--yaw", type=float, default=-25.0)
    parser.add_argument("--pitch", type=float, default=12.0)
    args = parser.parse_args(argv)
    data = mr.slot(project.find_iso(None), args.card)
    found = motions(data)
    for slot, at in found:
        m = read_motion(data, at)
        print(f"motion {slot}: {m['frames']} frames, kind {m['kind']:#x}, {len(m['nodes'])} nodes")
    if args.out:
        motion = None
        if args.motion is not None:
            motion = read_motion(data, dict(found)[args.motion])
        import model_preview
        positions, coords, triangles = posed_mesh(data, posed_matrices(data, motion, args.frame))
        texture = first_texture(data)
        pixels = model_preview.draw(positions, coords, triangles, texture, 256, args.yaw, args.pitch, flip_v=False)
        mt.write_png(args.out, 256, 256, pixels)
    return 0


if __name__ == "__main__":
    sys.exit(main())
