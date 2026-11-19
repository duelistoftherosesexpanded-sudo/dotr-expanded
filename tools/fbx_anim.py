#!/usr/bin/env python3
"""Reads the skeleton, skin and animation clips of a binary FBX (Duel Links rips: one mesh, bones as
LimbNode and Null models, Euler XYZ rotations without pre-rotations, linear keys) for
tools/model_skinned.py.

  load(path) -> dict(
      bones:    [dict(id, name, parent (index or -1), t, r (degrees), s)]   rest transforms
      skin:     {bone index: [(vertex, weight)]}
      bind:     {bone index: 4x4 column-vector matrix}   the bone's world matrix at bind time
      mesh:     (positions, coords, triangles) as tools/model_import.load_mesh gives them
      clips:    [dict(name, seconds, tracks: {bone: {"T"|"R"|"S": [curve x, y, z]}})]
                 a curve is ([times in seconds], [values]) or None)
  pose(model, clip, seconds) -> [(t, r, s)] local transforms of every bone at that time
  world(model, locals) -> [4x4] world matrices

  fbx_anim.py FILE      prints the bones and clips
"""

import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import fbx_read as f  # noqa: E402

TICKS = 46186158000.0   # FBX time units a second


def _props(node):
    p70 = f.child(node, "Properties70")
    result = {}
    for q in (p70[2] if p70 else []):
        result[q[1][0]] = q[1][4:]
    return result


def _matrix(values):
    """A 4x4 column-vector matrix from FBX's 16 numbers (stored with the translation at 12-14)."""
    return [[values[c * 4 + r] for c in range(4)] for r in range(4)]


def load(path):
    _, roots = f.parse(path)
    objects = next(f.find(roots, "Objects"))
    connections = next(f.find(roots, "Connections"))[2]
    by_id = {n[1][0]: n for n in objects[2]}
    models = [n for n in objects[2] if n[0] == "Model" and n[1][2] in ("LimbNode", "Null", "Root")]
    index = {n[1][0]: i for i, n in enumerate(models)}
    parent = {}
    links = []
    for c in connections:
        kind, child_id, parent_id = c[1][0], c[1][1], c[1][2]
        prop = c[1][3] if len(c[1]) > 3 else None
        links.append((kind, child_id, parent_id, prop))
        if kind == "OO" and child_id in index and parent_id in index:
            parent[index[child_id]] = index[parent_id]
    bones = []
    for i, n in enumerate(models):
        p = _props(n)
        bones.append(dict(id=n[1][0], name=f.short(n[1][1]), parent=parent.get(i, -1),
                          t=list(p.get("Lcl Translation", [0.0, 0.0, 0.0])),
                          r=list(p.get("Lcl Rotation", [0.0, 0.0, 0.0])),
                          s=list(p.get("Lcl Scaling", [1.0, 1.0, 1.0]))))
        if any(k in p and any(abs(v) > 1e-6 for v in p[k]) for k in ("PreRotation", "PostRotation")):
            raise ValueError(f"bone {bones[-1]['name']} has a pre- or post-rotation; not supported")
    # Order the bones so every parent comes before its children (the DotR walk needs it later).
    order, seen = [], set()

    def visit(i):
        if i in seen:
            return
        if bones[i]["parent"] >= 0:
            visit(bones[i]["parent"])
        seen.add(i)
        order.append(i)

    for i in range(len(bones)):
        visit(i)
    remap = {old: new for new, old in enumerate(order)}
    bones = [dict(bones[old], parent=remap.get(bones[old]["parent"], -1)) for old in order]
    index = {b["id"]: i for i, b in enumerate(bones)}

    # Skin: clusters link a bone (child of the cluster) to vertex weights and its bind matrix.
    clusters = {n[1][0]: n for n in objects[2] if n[0] == "Deformer" and n[1][2] == "Cluster"}
    skin, bind = {}, {}
    for kind, child_id, parent_id, _ in links:
        if kind == "OO" and parent_id in clusters and child_id in index:
            c = clusters[parent_id]
            b = index[child_id]
            ix, w = f.child(c, "Indexes"), f.child(c, "Weights")
            if ix and w:
                skin.setdefault(b, []).extend(zip(ix[1][0], w[1][0]))
            link = f.child(c, "TransformLink")
            if link:
                bind[b] = _matrix(link[1][0])
    mesh = f.load_fbx(path)

    # Clips: stack -> layer -> curve node (connected to a bone's Lcl property) -> curves d|X/Y/Z.
    children = {}
    for kind, child_id, parent_id, prop in links:
        children.setdefault(parent_id, []).append((child_id, prop))
    target = {}
    for kind, child_id, parent_id, prop in links:
        if kind == "OP" and parent_id in index and by_id.get(child_id, ("",))[0] == "AnimationCurveNode":
            target[child_id] = (index[parent_id], {"Lcl Translation": "T", "Lcl Rotation": "R",
                                                   "Lcl Scaling": "S"}.get(prop))
    clips = []
    for n in objects[2]:
        if n[0] != "AnimationStack":
            continue
        tracks, end = {}, 0.0
        for layer, _ in children.get(n[1][0], []):
            if by_id.get(layer, ("",))[0] != "AnimationLayer":
                continue
            for node, _ in children.get(layer, []):
                if node not in target or target[node][1] is None:
                    continue
                bone, channel = target[node]
                curves = [None, None, None]
                for curve, prop in children.get(node, []):
                    if by_id.get(curve, ("",))[0] != "AnimationCurve" or prop not in ("d|X", "d|Y", "d|Z"):
                        continue
                    c = by_id[curve]
                    times = [t / TICKS for t in f.child(c, "KeyTime")[1][0]]
                    values = list(f.child(c, "KeyValueFloat")[1][0])
                    curves["XYZ".index(prop[-1])] = (times, values)
                    if times:
                        end = max(end, times[-1])
                tracks.setdefault(bone, {})[channel] = curves
        clips.append(dict(name=f.short(n[1][1]), seconds=end, tracks=tracks))
    return dict(bones=bones, skin=skin, bind=bind, mesh=mesh, clips=clips)


def _value(curve, t, default):
    if curve is None or not curve[0]:
        return default
    times, values = curve
    if t <= times[0]:
        return values[0]
    if t >= times[-1]:
        return values[-1]
    low, high = 0, len(times) - 1
    while high - low > 1:
        mid = (low + high) // 2
        if times[mid] <= t:
            low = mid
        else:
            high = mid
    span = times[high] - times[low] or 1.0
    return values[low] + (values[high] - values[low]) * (t - times[low]) / span


def pose(model, clip, seconds):
    result = []
    for i, b in enumerate(model["bones"]):
        tr = clip["tracks"].get(i, {}) if clip else {}
        out = []
        for channel, rest in (("T", b["t"]), ("R", b["r"]), ("S", b["s"])):
            curves = tr.get(channel) or [None, None, None]
            out.append([_value(curves[k], seconds, rest[k]) for k in range(3)])
        result.append(tuple(out))
    return result


def local_matrix(t, r, s):
    """T * Rz * Ry * Rx * S (FBX Euler XYZ, degrees), as DotR nodes are built."""
    cx, sx = math.cos(math.radians(r[0])), math.sin(math.radians(r[0]))
    cy, sy = math.cos(math.radians(r[1])), math.sin(math.radians(r[1]))
    cz, sz = math.cos(math.radians(r[2])), math.sin(math.radians(r[2]))
    rot = [[cy * cz, sx * sy * cz - cx * sz, cx * sy * cz + sx * sz],
           [cy * sz, sx * sy * sz + cx * cz, cx * sy * sz - sx * cz],
           [-sy, sx * cy, cx * cy]]
    return [[rot[i][0] * s[0], rot[i][1] * s[1], rot[i][2] * s[2], t[i]] for i in range(3)] + [[0, 0, 0, 1]]


def multiply(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


def world(model, locals_):
    out = []
    for i, b in enumerate(model["bones"]):
        m = local_matrix(*locals_[i])
        out.append(multiply(out[b["parent"]], m) if b["parent"] >= 0 else m)
    return out


def _invert(m):
    a = [list(row) + [float(i == j) for j in range(4)] for i, row in enumerate(m)]
    for c in range(4):
        p = max(range(c, 4), key=lambda r: abs(a[r][c]))
        a[c], a[p] = a[p], a[c]
        d = a[c][c]
        a[c] = [x / d for x in a[c]]
        for r in range(4):
            if r != c:
                f_ = a[r][c]
                a[r] = [x - f_ * y for x, y in zip(a[r], a[c])]
    return [row[4:] for row in a]


def posed_mesh(model, clip, seconds):
    """(positions, coords, triangles) of the skinned mesh posed by `clip` at `seconds` (None: rest)."""
    worlds = world(model, pose(model, clip, seconds))
    skin = {b: multiply(worlds[b], _invert(model["bind"][b])) for b in model["bind"]}
    positions, coords, triangles = model["mesh"]
    acc = [[0.0, 0.0, 0.0, 0.0] for _ in positions]
    for b, pairs in model["skin"].items():
        m = skin.get(b)
        if m is None:
            continue
        for v, w in pairs:
            p = positions[v]
            for i in range(3):
                acc[v][i] += w * (m[i][0] * p[0] + m[i][1] * p[1] + m[i][2] * p[2] + m[i][3])
            acc[v][3] += w
    out = [tuple(a[i] / a[3] for i in range(3)) if a[3] else tuple(p) for a, p in zip(acc, positions)]
    return out, coords, triangles


def main(argv=None):
    path = (argv or sys.argv[1:])[0]
    model = load(path)
    print(len(model["bones"]), "bones,", len(model["skin"]), "skinned,", len(model["mesh"][2]), "triangles")
    for c in model["clips"]:
        print(f"clip {c['name']}: {c['seconds']:.2f} s, {len(c['tracks'])} bones animated")
    return 0


if __name__ == "__main__":
    sys.exit(main())
