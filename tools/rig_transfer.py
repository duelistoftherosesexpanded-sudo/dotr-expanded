#!/usr/bin/env python3
"""Puts a static mesh (OBJ/FBX/PMX) on another model's skeleton and animations, for a mesh whose
pose differs from that skeleton's: the skeleton is laid out to match the mesh (each listed joint
pointing a given way, the rest keeping their own pose relative to their parent), the mesh is
skinned to it (each point to its nearest bones), and the other model's clips drive it as they are.
model_skinned.build reads a config file named *.rig.json through load().

Config (JSON):
  {"skeleton": "slot162.dat",                 the animated model (HSD .dat, FBX, DAE)
   "skeleton_options": {"nested_roots": false},
   "mesh": "Slifer.obj",                      the static mesh (model_import.load_mesh)
   "directions": {"3": [0, 0, 1], ...},        joint: the way its bone points in the mesh (its local x)
   "lateral": [1, 0, 0],                       for listed joints: their local z, as near as can be
   "lateral_flip": [35, 36],                  joints whose local z is the opposite
   "offset": [0, 0, 0],                        added to the laid-out skeleton (moves it onto the mesh)
   "skin_joints": [3, 4, ...] or omitted,      joints the mesh may be weighted to (default: those
                                               the animated model's own mesh uses)
   "falloff": 4}                              weight ~ 1 / distance^falloff over the two nearest bones
Paths are relative to the config file.

  rig_transfer.py CONFIG.rig.json [--preview OUT.png TEXTURE.png]
"""

import json
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import fbx_anim as fa  # noqa: E402
import model_import as mi  # noqa: E402


def _load_skeleton(path, options):
    p = str(path).lower()
    if p.endswith(".dat"):
        import hsd_anim
        return hsd_anim.load(path, **options)
    if p.endswith(".dae"):
        import dae_anim
        return dae_anim.load(path)
    return fa.load(path)


def _norm(v):
    length = math.sqrt(sum(x * x for x in v)) or 1.0
    return [x / length for x in v]


def _cross(a, b):
    return [a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]]


def _dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def _frame(x, lateral):
    """A rotation (columns x, y, z) with its x along `x` and its z as near `lateral` as can be."""
    x = _norm(x)
    z = [lateral[i] - _dot(lateral, x) * x[i] for i in range(3)]
    if math.sqrt(_dot(z, z)) < 1e-6:
        z = _cross(x, [0, 1, 0]) if abs(x[1]) < 0.9 else _cross(x, [1, 0, 0])
    z = _norm(z)
    y = _cross(z, x)
    return [[x[i], y[i], z[i]] for i in range(3)]


def _segment_distance(p, a, b):
    ab = [b[i] - a[i] for i in range(3)]
    t = _dot([p[i] - a[i] for i in range(3)], ab) / (_dot(ab, ab) or 1.0)
    t = max(0.0, min(1.0, t))
    q = [a[i] + ab[i] * t for i in range(3)]
    return math.dist(p, q)


def layout(model, config):
    """World matrices of the skeleton laid out on the mesh."""
    bones = model["bones"]
    directions = {int(k): v for k, v in config.get("directions", {}).items()}
    lateral = config.get("lateral", [1, 0, 0])
    flip = set(config.get("lateral_flip", []))
    offset = config.get("offset", [0, 0, 0])
    worlds = []
    for i, b in enumerate(bones):
        local = fa.local_matrix(b["t"], b["r"], b["s"])
        parent = worlds[b["parent"]] if b["parent"] >= 0 else \
            [[1, 0, 0, offset[0]], [0, 1, 0, offset[1]], [0, 0, 1, offset[2]], [0, 0, 0, 1]]
        w = fa.multiply(parent, local)
        if i in directions:
            lat = [-x for x in lateral] if i in flip else lateral
            rot = _frame(directions[i], lat)
            # keep the joint's scale (the length of its axes)
            sc = [math.sqrt(sum(w[r][c] ** 2 for r in range(3))) for c in range(3)]
            w = [[rot[r][c] * sc[c] for c in range(3)] + [w[r][3]] for r in range(3)] + [[0, 0, 0, 1]]
        worlds.append(w)
    return worlds


def load(path):
    path = Path(path)
    config = json.loads(path.read_text())
    folder = path.parent
    model = _load_skeleton(folder / config["skeleton"], config.get("skeleton_options", {}))
    bones = model["bones"]
    worlds = layout(model, config)
    positions, coords, triangles = mi.load_mesh(folder / config["mesh"])

    allowed = set(config.get("skin_joints") or model["skin"].keys())
    # Bones as segments from each allowed joint to its children (or a stub along its x).
    kids = {}
    for i, b in enumerate(bones):
        kids.setdefault(b["parent"], []).append(i)
    segments = []
    for j in allowed:
        a = [worlds[j][r][3] for r in range(3)]
        ends = [[worlds[c][r][3] for r in range(3)] for c in kids.get(j, [])]
        ends = [e for e in ends if math.dist(a, e) > 1e-3] or \
            [[a[r] + worlds[j][r][0] * 10.0 for r in range(3)]]
        for e in ends:
            segments.append((j, a, e))
    falloff = float(config.get("falloff", 4))
    skin = {}
    for v, p in enumerate(positions):
        best = {}
        for j, a, e in segments:
            dist = _segment_distance(p, a, e)
            if j not in best or dist < best[j]:
                best[j] = dist
        near = sorted(best.items(), key=lambda kv: kv[1])[:2]
        ws = [(j, 1.0 / max(d, 1e-3) ** falloff) for j, d in near]
        total = sum(w for _, w in ws)
        for j, w in ws:
            skin.setdefault(j, []).append((v, w / total))

    return dict(bones=bones, skin=skin, bind={i: worlds[i] for i in range(len(bones))},
                mesh=(positions, coords, triangles), clips=model["clips"])


def main(argv=None):
    argv = argv or sys.argv[1:]
    model = load(argv[0])
    print(len(model["bones"]), "joints,", len(model["skin"]), "used,", len(model["mesh"][2]), "triangles,",
          [(c["name"], round(c["seconds"], 2)) for c in model["clips"]])
    if "--preview" in argv:
        import model_preview
        import model_texture as mt
        out, tex = argv[argv.index("--preview") + 1], argv[argv.index("--preview") + 2]
        texture = mt.read_png(tex)
        size = 240
        panels = []
        for clip, t in ((None, 0.0), (model["clips"][0], 0.0), (model["clips"][1], model["clips"][1]["seconds"] * 0.5),
                        (model["clips"][2], 0.3)):
            pos, uv, tri = fa.posed_mesh(model, clip, t)
            for yaw in (-30, 90):
                panels.append(model_preview.draw(pos, uv, tri, texture, size, yaw, 10))
        rows = []
        for r in range(0, len(panels), 4):
            rows += [sum((list(p[y]) for p in panels[r:r + 4]), []) for y in range(size)]
        mt.write_png(out, size * 4, len(rows), rows)
        print("wrote", out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
