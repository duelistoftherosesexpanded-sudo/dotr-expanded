#!/usr/bin/env python3
"""Replace a monster's mesh with a Wavefront OBJ, as one rigid piece on one bone (experimental).

The monster's file keeps its bones, animations, draw list and textures. Its first strip reference
is pointed at new strips written over its strip data, all bound to the first matrix of the draw
list, and the other strip references are emptied. The new mesh is scaled and placed to fill the
original model's bind-pose bounding box. It uses the model's first texture, which --texture
replaces with a PNG (scaled to that texture's size and reduced to 256 colours).

Reads your own disc image; writes the changed 1 MiB slot into an existing COPY of the image that
you name with --into (never the image in baserom/), and a copy of the slot to out/research/.

  --up, --front   which OBJ axes point up and towards the viewer, e.g. --up +z --front -y
  --turn          flip the facing by half a turn if the result looks backwards in game
"""

import argparse
from pathlib import Path
import re
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_research as mr  # noqa: E402
import model_texture as mt  # noqa: E402
import project  # noqa: E402
from runtime import ROOT  # noqa: E402

BATCH_QUADWORDS = 216   # the original lists restart their VU buffer before this many quadwords
STRIP_POINTS = 54       # the longest strip seen in an original model


def axis(name):
    sign = -1.0 if name[0] == "-" else 1.0
    return sign, "xyz".index(name[-1])


def load_obj(path):
    positions, coords, triangles = [], [], []
    for line in Path(path).read_text(encoding="latin-1").splitlines():
        parts = line.split()
        if not parts:
            continue
        if parts[0] == "v":
            positions.append(tuple(map(float, parts[1:4])))
        elif parts[0] == "vt":
            coords.append(tuple(map(float, parts[1:3])))
        elif parts[0] == "f":
            corners = []
            for token in parts[1:]:
                fields = token.split("/")
                corners.append((int(fields[0]) - 1, int(fields[1]) - 1 if len(fields) > 1 and fields[1] else -1))
            for i in range(1, len(corners) - 1):
                triangles.append((corners[0], corners[i], corners[i + 1]))
    return positions, coords, triangles


def cross(a, b):
    return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


def normals(positions, triangles):
    """Smooth normals: the sum of the face normals around each position."""
    result = [[0.0, 0.0, 0.0] for _ in positions]
    for a, b, c in triangles:
        p, q, r = positions[a[0]], positions[b[0]], positions[c[0]]
        n = cross([q[i] - p[i] for i in range(3)], [r[i] - p[i] for i in range(3)])
        for corner in (a, b, c):
            for i in range(3):
                result[corner[0]][i] += n[i]
    for n in result:
        length = sum(v * v for v in n) ** 0.5 or 1.0
        for i in range(3):
            n[i] /= length
    return result


def stripify(triangles):
    """Greedy triangle strips over corners; each triangle keeps its winding (even ones as listed)."""
    edges = {}
    for index, (a, b, c) in enumerate(triangles):
        for edge in ((a, b), (b, c), (c, a)):
            edges.setdefault(edge, []).append(index)
    used = [False] * len(triangles)

    def grow(index, turn):
        """The strip from a triangle entered at one of its three corners, and the triangles it takes."""
        a, b, c = triangles[index]
        strip = [(a, b, c), (b, c, a), (c, a, b)][turn]
        strip, taken = list(strip), [index]
        while len(strip) < STRIP_POINTS:
            even = (len(strip) - 3) % 2 == 0
            # The next triangle shares the last edge, seen the other way round.
            want = (strip[-1], strip[-2]) if even else (strip[-2], strip[-1])
            following = next((t for t in edges.get(want, []) if not used[t] and t not in taken), None)
            if following is None:
                break
            corner = next((c for c in triangles[following] if c not in want), None)
            if corner is None:          # a degenerate triangle (two corners the same point)
                break
            taken.append(following)
            strip.append(corner)
        # Then backwards from the first triangle. An odd number of points in front would turn
        # every triangle over, so the last one added is given back in that case.
        front = []
        while len(strip) + len(front) < STRIP_POINTS:
            head = (front[::-1] + strip)[:2]
            want = (head[1], head[0]) if len(front) % 2 == 0 else (head[0], head[1])
            previous = next((t for t in edges.get(want, []) if not used[t] and t not in taken), None)
            if previous is None:
                break
            corner = next((c for c in triangles[previous] if c not in want), None)
            if corner is None:          # a degenerate triangle (two corners the same point)
                break
            taken.append(previous)
            front.append(corner)
        if len(front) % 2:
            front.pop()
            taken.pop()
        return front[::-1] + strip, taken

    def free_neighbours(index):
        a, b, c = triangles[index]
        return sum(1 for edge in ((b, a), (c, b), (a, c)) for t in edges.get(edge, []) if not used[t])

    result = []
    # Triangles with the fewest free neighbours start strips first, so few are left stranded.
    order = sorted(range(len(triangles)), key=free_neighbours)
    for index in order:
        if used[index]:
            continue
        strip, taken = max((grow(index, turn) for turn in range(3)), key=lambda pair: len(pair[0]))
        for t in taken:
            used[t] = True
        result.append(strip)
    return result


def invert(m):
    """Inverse of an affine 4x4 given as rows."""
    a = [row[:3] for row in m[:3]]
    det = (a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
           + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]))
    inverse = [[(a[(j + 1) % 3][(i + 1) % 3] * a[(j + 2) % 3][(i + 2) % 3]
                 - a[(j + 1) % 3][(i + 2) % 3] * a[(j + 2) % 3][(i + 1) % 3]) / det for j in range(3)] for i in range(3)]
    t = [m[i][3] for i in range(3)]
    return [inverse[i] + [-sum(inverse[i][k] * t[k] for k in range(3))] for i in range(3)]


def clamp16(value):
    return max(-32767, min(32767, int(round(value))))


def data_end(data):
    """Where a monster slot's data ends, walking the blocks after the header's motion offset the way
    lSzModel_BindModelByAddr does (by the flag word at +4): motions 0-7 (size at +4), a 0x8000-byte
    block (0x100), a texture file (0x200, size at +8), another 0x8000-byte block (0x400), textures
    11-18 (size at +8), two blocks rounded to 16 (0x80000, 0x100000) and the sound cue list
    (0x200000: at most 64 four-byte cues). Never before the last non-zero byte."""
    flags, = struct.unpack_from("<I", data, 4)
    at, = struct.unpack_from("<I", data, 0xc)
    word = lambda offset: struct.unpack_from("<I", data, offset)[0]
    for i in range(8):
        if flags >> i & 1:
            at += word(at + 4)
    if flags & 0x100:
        at += 0x8000
    if flags & 0x200:
        at += word(at + 8)
    if flags & 0x400:
        at += 0x8000
    for i in range(8):
        if flags >> (i + 11) & 1:
            at += word(at + 8)
    for bit in (0x80000, 0x100000):
        if flags & bit:
            at += (word(at + 8) + 15) & ~15
    if flags & 0x200000:
        at += 64 * 4
    segments_end = max(o + s for o, s in mr.segments(data))
    return max(at, segments_end, len(data.rstrip(b"\0")))


def load_mesh(path, arms_down=0.0, hide_uv=None):
    """(positions, coords, triangles) from an OBJ, a binary FBX (tools/fbx_read.py) or an MMD PMX
    (tools/pmx_read.py; its textures packed into one image by pmx_read.atlas). `hide_uv` lists
    boxes [left, top, right, bottom] of the texture picture (0-1, top down): triangles whose texture
    lies inside one are left out (a second body some models keep in the same mesh, such as the
    angry Mokey Mokey)."""
    if str(path).lower().endswith(".fbx"):
        import fbx_read
        positions, coords, triangles = fbx_read.load_fbx(path, arms_down)
    elif str(path).lower().endswith(".pmx"):
        import pmx_read
        positions, coords, triangles = pmx_read.mesh(path)
    elif arms_down:
        raise ValueError("lowering the arms needs the skeleton of an FBX file")
    else:
        positions, coords, triangles = load_obj(path)
    if hide_uv:
        def hidden(t):
            u = sum(coords[c][0] for _, c in t if c >= 0) / 3 % 1.0
            v = 1.0 - sum(coords[c][1] for _, c in t if c >= 0) / 3 % 1.0
            return any(b[0] <= u <= b[2] and b[1] <= v <= b[3] for b in hide_uv)
        triangles = [t for t in triangles if not hidden(t)]
    return positions, coords, triangles


def decimate(positions, coords, triangles, target, mapping=None):
    """At most `target` triangles from the mesh, by edge collapse (Garland and Heckbert's quadric
    error): the edge whose removal changes the surface least goes first, its two ends becoming one
    point at the better of the ends and their middle. Open borders (capes, wings, hair cards) are
    held by extra planes along them, and a collapse that would turn a triangle over is skipped.
    Each corner keeps its own texture coordinate, so the texture stays on what remains. Vertex
    clustering (cluster_decimate) finishes the job if collapsing alone cannot get there.
    With `mapping` (a list), it is filled with the original index of each new position (the point
    that stayed), so data per position such as skin weights can follow."""
    if len(triangles) <= target:
        if mapping is not None:
            mapping[:] = list(range(len(positions)))
        return positions, coords, triangles
    import heapq

    pos = [list(p) for p in positions]
    tris = [list(t) for t in triangles]
    alive = [True] * len(tris)
    live = len(tris)
    around = [set() for _ in pos]
    for t, corners in enumerate(tris):
        for corner in corners:
            around[corner[0]].add(t)
    quadric = [[0.0] * 10 for _ in pos]

    def add_plane(q, n, d, w):
        a, b, c = n
        for i, v in enumerate((a * a, a * b, a * c, a * d, b * b, b * c, b * d, c * c, c * d, d * d)):
            q[i] += w * v

    def normal(p, q, r):
        return cross([q[i] - p[i] for i in range(3)], [r[i] - p[i] for i in range(3)])

    edge_faces = {}
    for t, (a, b, c) in enumerate(tris):
        p, q, r = pos[a[0]], pos[b[0]], pos[c[0]]
        n = normal(p, q, r)
        length = sum(v * v for v in n) ** 0.5
        if length == 0:
            continue
        n = [v / length for v in n]
        d = -sum(n[i] * p[i] for i in range(3))
        for corner in (a, b, c):
            add_plane(quadric[corner[0]], n, d, length / 2)
        for u, v in ((a[0], b[0]), (b[0], c[0]), (c[0], a[0])):
            edge_faces.setdefault((min(u, v), max(u, v)), []).append((t, n))
    # Borders: a plane through each open edge, upright to its triangle, keeps the outline in place.
    for (u, v), faces in edge_faces.items():
        if len(faces) != 1:
            continue
        e = [pos[v][i] - pos[u][i] for i in range(3)]
        n = cross(e, faces[0][1])
        length = sum(x * x for x in n) ** 0.5
        if length == 0:
            continue
        n = [x / length for x in n]
        d = -sum(n[i] * pos[u][i] for i in range(3))
        weight = 10.0 * sum(x * x for x in e)
        add_plane(quadric[u], n, d, weight)
        add_plane(quadric[v], n, d, weight)

    def error(q, p):
        x, y, z = p
        return (q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y + 2 * q[5] * y * z
                + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9])

    version = [0] * len(pos)
    removed = [False] * len(pos)
    heap = []

    def push(u, v):
        q = [quadric[u][i] + quadric[v][i] for i in range(10)]
        middle = [(pos[u][i] + pos[v][i]) / 2 for i in range(3)]
        cost, point = min((error(q, p), p) for p in (pos[u], pos[v], middle))
        heapq.heappush(heap, (cost, u, v, version[u], version[v], list(point)))

    for u, v in edge_faces:
        push(u, v)

    def flips(u, v, point):
        for t in around[u] | around[v]:
            ids = [c[0] for c in tris[t]]
            if u in ids and v in ids:
                continue
            before = normal(*(pos[i] for i in ids))
            after = normal(*(point if i in (u, v) else pos[i] for i in ids))
            lb = sum(x * x for x in before) ** 0.5
            la = sum(x * x for x in after) ** 0.5
            if la == 0 or (lb and sum(before[i] * after[i] for i in range(3)) < 0.2 * la * lb):
                return True
        return False

    while live > target and heap:
        cost, u, v, vu, vv, point = heapq.heappop(heap)
        if removed[u] or removed[v] or version[u] != vu or version[v] != vv:
            continue
        if flips(u, v, point):
            continue
        pos[u] = point
        quadric[u] = [quadric[u][i] + quadric[v][i] for i in range(10)]
        removed[v] = True
        for t in list(around[v]):
            ids = [c[0] for c in tris[t]]
            if u in ids:
                alive[t] = False
                live -= 1
                for i in ids:
                    around[i].discard(t)
            else:
                tris[t] = [(u, c[1]) if c[0] == v else c for c in tris[t]]
                around[u].add(t)
        around[v] = set()
        version[u] += 1
        neighbours = {c[0] for t in around[u] for c in tris[t]} - {u}
        for w in neighbours:
            push(min(u, w), max(u, w))

    keep = {}
    out = []
    for t, corners in enumerate(tris):
        if not alive[t]:
            continue
        ids = [c[0] for c in corners]
        if len(set(ids)) < 3:
            continue
        out.append(tuple((keep.setdefault(c[0], len(keep)), c[1]) for c in corners))
    merged = [None] * len(keep)
    origin = [0] * len(keep)
    for old, new in keep.items():
        merged[new] = tuple(pos[old])
        origin[new] = old
    if len(out) > target:
        second = []
        result = cluster_decimate(merged, coords, out, target, second)
        if mapping is not None:
            mapping[:] = [origin[i] for i in second]
        return result
    if mapping is not None:
        mapping[:] = origin
    return merged, coords, out


def cluster_decimate(positions, coords, triangles, target, mapping=None):
    """About `target` triangles from the mesh, by vertex clustering: positions in the same cell of a
    grid are merged into their average, and triangles that collapse are dropped. Each corner keeps
    its own texture coordinate, so the texture stays where it was on what remains. The cell size is
    searched so the result is as close below the target as the grid allows. mapping: as decimate."""
    if len(triangles) <= target:
        if mapping is not None:
            mapping[:] = list(range(len(positions)))
        return positions, coords, triangles
    lo = [min(p[i] for p in positions) for i in range(3)]
    hi = [max(p[i] for p in positions) for i in range(3)]
    extent = max(hi[i] - lo[i] for i in range(3)) or 1.0

    def cluster(cells):
        size = extent / cells
        index, sums, out = {}, [], []
        remap, first = [], []
        for p in positions:
            key = tuple(int((p[i] - lo[i]) / size) for i in range(3))
            if key not in index:
                index[key] = len(sums)
                sums.append([0.0, 0.0, 0.0, 0])
                first.append(len(remap))
            k = index[key]
            s = sums[k]
            s[0] += p[0]; s[1] += p[1]; s[2] += p[2]; s[3] += 1
            remap.append(k)
        seen = set()
        for a, b, c in triangles:
            ka, kb, kc = remap[a[0]], remap[b[0]], remap[c[0]]
            if ka == kb or kb == kc or ka == kc:
                continue
            key = (ka, kb, kc)
            if key in seen:
                continue
            seen.add(key)
            out.append(((ka, a[1]), (kb, b[1]), (kc, c[1])))
        merged = [(s[0] / s[3], s[1] / s[3], s[2] / s[3]) for s in sums]
        return merged, out, first

    low, high, best = 8, 2048, None
    while low <= high:
        cells = (low + high) // 2
        merged, out, first = cluster(cells)
        if len(out) <= target:
            best = (merged, out, first)
            low = cells + 1
        else:
            high = cells - 1
    if best is None:
        best = cluster(8)
    if mapping is not None:
        mapping[:] = best[2]
    return best[0], coords, best[1]


def build(data, obj, up, front, turn, flip_v, fill, grow=False, arms_down=0.0, max_triangles=None, hide_uv=None):
    """The slot with its mesh replaced. With `grow`, strips that do not fit in the model's own strip
    data continue in the free space at the end of the 1 MiB slot, through a second strip reference
    (strip references address the strip data segment, so any later offset in the file works)."""
    segs = mr.segments(data)
    start, size = segs[1]
    entries = list(mr.draw_list(data, start, size, segs))
    if any(kind == "unknown" for kind, _, _ in entries):
        raise ValueError("the draw list has a record this tool does not know")
    first_matrix = next((a, b) for kind, a, b in entries if kind == "matrix")
    vu_address, node_slot = first_matrix[0], (first_matrix[1] - start) >> 4
    _, matrices = mr.nodes(data)
    to_local = invert(matrices[node_slot])

    # Where the new mesh goes: the original's bind-pose bounding box (+y is up in a model).
    old_vertices = mr.mesh(data)[0]
    lo = [min(v[i] for v in old_vertices) for i in range(3)]
    hi = [max(v[i] for v in old_vertices) for i in range(3)]

    positions, coords, triangles = load_mesh(obj, arms_down, hide_uv)
    original_triangles = len(triangles)
    if max_triangles:
        positions, coords, triangles = decimate(positions, coords, triangles, max_triangles)
    (us, ua), (fs, fa) = axis(up), axis(front)
    if ua == fa:
        raise ValueError("--up and --front must name different axes")
    side_axis = 3 - ua - fa
    if turn:
        fs = -fs
    # Model axes: x side, y up, z front; the side axis is chosen to keep the mesh right-handed.
    basis_up = [us if i == ua else 0.0 for i in range(3)]
    basis_front = [fs if i == fa else 0.0 for i in range(3)]
    basis_side = cross(basis_up, basis_front)
    del side_axis

    def rotate(p):
        return (sum(basis_side[i] * p[i] for i in range(3)), sum(basis_up[i] * p[i] for i in range(3)),
                sum(basis_front[i] * p[i] for i in range(3)))

    turned = [rotate(p) for p in positions]
    nlo = [min(v[i] for v in turned) for i in range(3)]
    nhi = [max(v[i] for v in turned) for i in range(3)]
    scale = fill * (hi[1] - lo[1]) / (nhi[1] - nlo[1])
    centre = [(lo[0] + hi[0]) / 2, 0.0, (lo[2] + hi[2]) / 2]

    def place(p):   # feet on the original's lowest point, centred on its footprint
        return (centre[0] + (p[0] - (nlo[0] + nhi[0]) / 2) * scale, lo[1] + (p[1] - nlo[1]) * scale,
                centre[2] + (p[2] - (nlo[2] + nhi[2]) / 2) * scale)

    world = [place(p) for p in turned]
    world_normals = [rotate(n) for n in normals(positions, triangles)]

    # Positions are 16-bit in the bone's space. When the mesh reaches further from the bone than
    # that allows, the bone's own scale is raised (and its "no scaling" flag cleared) to cover it.
    reach = max(abs(sum(to_local[i][k] * p[k] for k in range(3)) + to_local[i][3]) for p in world for i in range(3))
    bone_scale = 1.0
    if reach > 32000:
        bone_scale = reach / 32000.0
        tree, _ = mr.nodes(data)
        node = next(n for n in tree if n["matrix"] == node_slot)
        at = segs[0][0] + node["offset"]
        data = bytearray(data)
        old = node["scale"] if not node["flags"] & 4 else (1.0, 1.0, 1.0)
        struct.pack_into("<3f", data, at, *(v * bone_scale for v in old))
        struct.pack_into("<H", data, at + 12, node["flags"] & ~4)
        data = bytes(data)
        _, matrices = mr.nodes(data)
        to_local = invert(matrices[node_slot])

    def local_point(p):
        return tuple(clamp16(sum(to_local[i][k] * p[k] for k in range(3)) + to_local[i][3]) for i in range(3))

    def local_normal(n):
        v = [sum(to_local[i][k] * n[k] for k in range(3)) for i in range(3)]
        length = sum(x * x for x in v) ** 0.5 or 1.0
        return tuple(clamp16(32767 * x / length) for x in v)

    out = bytearray()
    batch_ends = []   # where each VU batch ends in `out`; parts may only be split there
    address, points, strips = 0, 0, stripify(triangles)
    for number, strip in enumerate(strips):
        n = len(strip)
        following = len(strips[number + 1]) if number + 1 < len(strips) else None
        used_after = address + 1 + 3 * n
        last = following is None or used_after + 1 + 3 * following > BATCH_QUADWORDS
        out += struct.pack("<6I", 0x6c018000 | address, n | (0x8000 if last else 0), 0x301e4000, 0x412,
                           vu_address, 0x69008000 | (3 * n << 16) | (address + 1))
        for position, coord in strip:
            u, v = coords[coord] if coord >= 0 else (0.0, 0.0)
            if flip_v:
                v = 1.0 - v
            out += struct.pack("<3h3h3h", clamp16(u * 4096), clamp16(v * 4096), 1,
                               *local_normal(world_normals[position]), *local_point(world[position]))
        out += bytes(-len(out) % 4)
        if last:
            out += struct.pack("<I", 0x15000000)   # run the VU program on what was sent
            out += bytes(-len(out) % 16)           # a part starts on a quadword
            batch_ends.append(len(out))
            address = 0
        else:
            address = used_after
        points += n
    out += bytes(-len(out) % 16)
    room = segs[2][1]

    # Where the strips go: the strip data, then (grow) the free end of the slot. Each region gets
    # whole batches and becomes one strip reference.
    # A strip reference is one DMA transfer, whose length (in quadwords) has 16 bits: at most
    # 0xffff0 bytes a part, so a longer free end is cut into several regions (each needing a
    # reference of its own).
    regions = [(segs[2][0], min(room, 0xffff0))]
    if grow:
        tail = (data_end(data) + 0xff) & ~0xff
        while tail < len(data):
            size = min(len(data) - tail, 0xffff0)
            regions.append((tail, size))
            tail += size
    # One region that takes everything is best (one strip reference); otherwise split.
    whole = next(((o, s) for o, s in regions if s >= len(out)), None)
    if whole is not None:
        regions = [whole]
    parts, start = [], 0
    for offset, size in regions:
        end = start
        for b in batch_ends:
            if b - start <= size:
                end = b
        if end > start:
            parts.append((offset, out[start:end]))
            start = end
        if start >= len(out):
            break
    if start < len(out):
        have = sum(size for _, size in regions)
        raise ValueError(f"the new strips need {len(out)} bytes; the model has {have}"
                         + ("" if grow else " (a slot of its own has more)")
                         + f". Reduce the model to about {len(triangles) * have // len(out)} triangles.")

    result = bytearray(data)
    result[segs[2][0]:segs[2][0] + room] = bytes(room)
    for offset, chunk in parts:
        result[offset:offset + len(chunk)] = chunk
    # Strip references, with the texture record and the matrix in the VU slot the new strips use
    # at each: a later part may only go where both are those of the first reference.
    references = []
    for part in (1, 3):
        pstart, psize = segs[part]
        at, end = pstart, pstart + psize
        if psize and struct.unpack_from("<I", data, at)[0] == 0x20000000:
            at = pstart + (struct.unpack_from("<I", data, at + 4)[0] & 0x0fffffff)
        texture, bound = None, None
        while at < end:
            command, where, _, extra = struct.unpack_from("<4I", data, at)
            if command == 0x10000008:
                texture = (part, at)
                at += 0x90
            elif command == 0x10000003:
                at += 0x40
            elif command & 0xf0000000 == 0x30000000 and extra == 0:
                references.append((at, texture, bound))
                at += 0x10
            else:
                if command & 0xf0000000 == 0x30000000 and extra & 0x3ff == vu_address:
                    bound = where
                at += 0x10
    usable = [references[0]] + [r for r in references[1:] if r[1] == references[0][1] and r[2] == references[0][2]]
    if len(usable) < len(parts):
        raise ValueError(f"the model can take {len(usable)} strip references for the new mesh; it needs "
                         f"{len(parts)}. Reduce the model, or use another donor.")
    chosen = [r[0] for r in usable[:len(parts)]]
    for at, _, _ in references:
        if at in chosen:
            number = chosen.index(at)
            # Tag 3: an offset from the strip data segment, which the game makes absolute.
            offset, chunk = parts[number]
            struct.pack_into("<4I", result, at, 0x30000000 | (len(chunk) >> 4),
                             0x30000000 | (offset - segs[2][0]), 0, 0)
        else:          # an empty record; its address word no longer carries a segment tag
            struct.pack_into("<4I", result, at, 0x10000000, 0, 0, 0)
    return bytes(result), dict(triangles=len(triangles), source_triangles=original_triangles,
                               strips=len(strips), points=points,
                               strip_bytes=len(out), room=sum(size for _, size in regions),
                               parts=[len(c) for _, c in parts], bone_node_slot=node_slot,
                               scale=round(scale, 4), bone_scale=round(bone_scale, 3), box_low=[round(v, 2) for v in lo],
                               box_high=[round(v, 2) for v in hi])


def import_slot(data, mesh, texture=None, up="+y", front="+z", turn=False, flip_v=True, fill=1.0, grow=False,
                arms_down=0.0, hide_uv=None):
    """A monster slot (1 MiB) with its mesh replaced by `mesh` and, optionally, its first texture by
    `texture` (a PNG); returns (slot, report). A mesh with more triangles than the slot holds is
    reduced (decimate) until it fits."""
    limit = None
    for _ in range(12):
        try:
            data_out, report = build(data, mesh, up, front, turn, flip_v, fill, grow, arms_down, limit, hide_uv)
            break
        except ValueError as error:
            found = re.search(r"about (\d+) triangles", str(error))
            if found:
                suggested = int(found.group(1))
            elif "strip references" in str(error):
                # More parts than references: fewer triangles make fewer parts.
                suggested = int((limit or len(load_mesh(mesh, arms_down, hide_uv)[2])) * 0.75)
            else:
                raise
            limit = int(min(suggested, limit or suggested) * 0.92)
    else:
        raise ValueError("the mesh could not be reduced enough to fit")
    data = data_out
    if texture:
        # The new strips follow the first texture record, which takes the first texture.
        data = bytearray(data)
        at, = struct.unpack_from("<I", data, mr.segments(data)[5][0] + 16)
        size = mt.info(data, at)
        width, height, rows = mt.read_png(texture)
        mt.encode(data, at, mt.resize(width, height, rows, size["width"], size["height"]))
        report["texture"] = f"{width}x{height} -> {size['width']}x{size['height']}, 256 colours"
        data = bytes(data)
    return data, report


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("index", type=int, help="monster slot (card number)")
    parser.add_argument("obj", help="Wavefront OBJ with triangles and texture coordinates")
    parser.add_argument("--into", required=True, help="a copy of the disc image to change")
    parser.add_argument("--iso", help="clean disc image to read the original model from")
    parser.add_argument("--up", default="+y")
    parser.add_argument("--front", default="+z")
    parser.add_argument("--turn", action="store_true")
    parser.add_argument("--keep-v", action="store_true", help="do not flip the texture's v coordinate")
    parser.add_argument("--texture", help="8-bit RGB or RGBA PNG for the model's first texture")
    parser.add_argument("--fill", type=float, default=1.0, help="height relative to the original model")
    args = parser.parse_args(argv)
    try:
        source = project.find_iso(args.iso)
        target = Path(args.into).resolve()
        if target == source.resolve() or (ROOT / "baserom") in target.parents:
            raise ValueError("--into must be a copy, not the image in baserom/")
        # A copy may have grown (tools/iso_files.py adds files at the end).
        if not target.is_file() or target.stat().st_size < source.stat().st_size:
            raise ValueError("--into must be an existing copy of the disc image")
        data, report = import_slot(mr.slot(source, args.index), args.obj, args.texture, args.up, args.front,
                                   args.turn, not args.keep_v, args.fill)
        mr.OUT.mkdir(parents=True, exist_ok=True)
        (mr.OUT / f"monster-{args.index:03d}.imported.bin").write_bytes(data)
        with target.open("r+b") as stream:
            stream.seek(mr.PACK_LBA * 2048 + args.index * mr.SLOT)
            stream.write(data)
        print(report)
        return 0
    except (OSError, ValueError, StopIteration, KeyError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
