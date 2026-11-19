#!/usr/bin/env python3
"""Reads a skinned, animated glTF binary (.glb, as tools/crossduel_export.py writes them) into the
model dict tools/model_skinned.py takes (the same shape as tools/fbx_anim.load):

  bones  [dict(id, name, parent, t, r (Euler XYZ degrees, T*Rz*Ry*Rx*S), s)]   every scene node
  skin   {bone: [(vertex, weight)]}   bind {bone: world matrix at bind time}
  mesh   (positions, coords (v up), triangles of (point, coord) corners)
  clips  [dict(name, seconds, tracks: {bone: {"T"|"R"|"S": [curve x, y, z]}})]

Every primitive's material texture becomes one column of an atlas (atlas()); a mesh on a node
without a skin is tied rigidly to that node.

  glb_anim.py FILE.glb [--atlas OUT.png]
"""

import json
import math
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

SIZES = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}
FORMATS = {5126: "f", 5125: "I", 5123: "H", 5121: "B", 5122: "h", 5120: "b"}


def _read(path):
    data = Path(path).read_bytes()
    magic, _, _ = struct.unpack_from("<III", data, 0)
    if magic != 0x46546C67:
        raise ValueError(f"{path} is not a .glb file")
    jl = struct.unpack_from("<I", data, 12)[0]
    j = json.loads(data[20:20 + jl])
    bl = struct.unpack_from("<I", data, 20 + jl)[0]
    blob = data[28 + jl:28 + jl + bl]
    return j, blob


def _accessor(j, blob, i):
    a = j["accessors"][i]
    v = j["bufferViews"][a["bufferView"]]
    n, fmt = SIZES[a["type"]], FORMATS[a["componentType"]]
    size = struct.calcsize("<" + fmt)
    stride = v.get("byteStride") or n * size
    start = v.get("byteOffset", 0) + a.get("byteOffset", 0)
    out = []
    for k in range(a["count"]):
        vals = struct.unpack_from(f"<{n}{fmt}", blob, start + k * stride)
        out.append(vals if n > 1 else vals[0])
    return out


def quat_matrix(q):
    x, y, z, w = q
    return [[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
            [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
            [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]]


def quat_euler(q):
    """Euler XYZ degrees with R = Rz * Ry * Rx (fbx_anim.local_matrix)."""
    m = quat_matrix(q)
    sy = max(-1.0, min(1.0, -m[2][0]))
    ry = math.asin(sy)
    if abs(sy) < 0.99999:
        rx = math.atan2(m[2][1], m[2][2])
        rz = math.atan2(m[1][0], m[0][0])
    else:
        rx = math.atan2(-m[1][2], m[1][1])
        rz = 0.0
    return [math.degrees(rx), math.degrees(ry), math.degrees(rz)]


def _unwrap(values):
    out = [values[0]]
    for v in values[1:]:
        out.append(out[-1] + ((v - out[-1] + 180.0) % 360.0 - 180.0))
    return out


def _matrix_from_column_major(m):
    return [[m[c * 4 + r] for c in range(4)] for r in range(4)]


def images(path):
    """[(name, png bytes)] of the textures the meshes use, in atlas column order."""
    j, blob = _read(path)
    out = []
    for t in _used_textures(j):
        img = j["images"][j["textures"][t]["source"]]
        v = j["bufferViews"][img["bufferView"]]
        out.append((img.get("name", f"image{t}"), blob[v.get("byteOffset", 0):v.get("byteOffset", 0) + v["byteLength"]]))
    return out


def _used_textures(j):
    found = []
    for mesh in j.get("meshes", []):
        for p in mesh["primitives"]:
            t = _texture_of(j, p)
            if t is not None and t not in found:
                found.append(t)
    return found


def _texture_of(j, prim):
    if "material" not in prim:
        return None
    pbr = j["materials"][prim["material"]].get("pbrMetallicRoughness", {})
    tex = pbr.get("baseColorTexture")
    return tex["index"] if tex else None


def atlas(path, out):
    """Write the model's textures side by side (each scaled to the largest) to `out`."""
    import tempfile
    import model_texture as mt
    pics = []
    for name, png in images(path):
        with tempfile.NamedTemporaryFile(suffix=".png", delete=False) as f:
            f.write(png)
        pics.append(mt.read_png(f.name))
        Path(f.name).unlink()
    if not pics:
        raise ValueError("the model has no textures")
    if len(pics) == 1:
        w, h, rows = pics[0]
        mt.write_png(str(out), w, h, rows)
        return out
    h = max(p[1] for p in pics)
    w = max(p[0] for p in pics)
    columns = [mt.resize(pw, ph, rows, w, h) for pw, ph, rows in pics]
    rows = [sum((list(col[y]) for col in columns), []) for y in range(h)]
    mt.write_png(str(out), w * len(columns), h, rows)
    return out


def load(path):
    import fbx_anim as fa
    j, blob = _read(path)
    nodes = j["nodes"]
    parent = {}
    for i, n in enumerate(nodes):
        for c in n.get("children", []):
            parent[c] = i
    # Bones: the scene nodes that are not mesh holders, parents first.
    holders = {i for i, n in enumerate(nodes) if "skin" in n}
    order, seen = [], set()

    def visit(i):
        if i in seen or i in holders:
            return
        seen.add(i)
        order.append(i)
        for c in nodes[i].get("children", []):
            visit(c)

    for r in j["scenes"][j.get("scene", 0)]["nodes"]:
        visit(r)
    index = {n: k for k, n in enumerate(order)}
    bones = []
    for n in order:
        node = nodes[n]
        q = node.get("rotation", [0, 0, 0, 1])
        bones.append(dict(id=n, name=node.get("name", f"node{n}"), parent=index.get(parent.get(n), -1),
                          t=list(node.get("translation", [0.0, 0.0, 0.0])), r=quat_euler(q),
                          s=list(node.get("scale", [1.0, 1.0, 1.0]))))
    rest = fa.world({"bones": bones}, [(b["t"], b["r"], b["s"]) for b in bones])

    # Meshes: skinned (on holder nodes) and rigid (on bones).
    textures = _used_textures(j)
    columns = max(1, len(textures))
    positions, coords, triangles = [], [], []
    skin, bind = {}, {}

    def add_mesh(mesh_index, joints, rigid_bone=None, node_bone=None):
        for p in j["meshes"][mesh_index]["primitives"]:
            attr = p["attributes"]
            pos = _accessor(j, blob, attr["POSITION"])
            uv = _accessor(j, blob, attr["TEXCOORD_0"]) if "TEXCOORD_0" in attr else [(0.0, 0.0)] * len(pos)
            t = _texture_of(j, p)
            col = textures.index(t) if t in textures else 0
            base, uv_base = len(positions), len(coords)
            if rigid_bone is not None:
                m = rest[rigid_bone]
                positions.extend(tuple(m[r][0] * x + m[r][1] * y + m[r][2] * z + m[r][3] for r in range(3))
                                 for x, y, z in pos)
                skin.setdefault(rigid_bone, []).extend((base + k, 1.0) for k in range(len(pos)))
                bind.setdefault(rigid_bone, rest[rigid_bone])
            elif "JOINTS_0" not in attr:
                # A part without weights in a skinned mesh (one Cross Duel rig has one): it moves
                # with the mesh's own node, or the skin's first joint.
                b = node_bone if node_bone is not None else joints[0]
                positions.extend(tuple(p_) for p_ in pos)
                skin.setdefault(b, []).extend((base + k, 1.0) for k in range(len(pos)))
            else:
                positions.extend(tuple(p_) for p_ in pos)
                js = _accessor(j, blob, attr["JOINTS_0"])
                ws = _accessor(j, blob, attr["WEIGHTS_0"])
                for k, (jj, ww) in enumerate(zip(js, ws)):
                    merged = {}
                    for a, w in zip(jj, ww):
                        if w > 0:
                            b = joints[a]
                            merged[b] = merged.get(b, 0.0) + w
                    for b, w in merged.items():
                        skin.setdefault(b, []).append((base + k, w))
            coords.extend(((col + u) / columns, 1.0 - v) for u, v in uv)
            idx = _accessor(j, blob, p["indices"])
            for k in range(0, len(idx) - 2, 3):
                triangles.append(tuple((base + idx[k + c], uv_base + idx[k + c]) for c in range(3)))

    for i, n in enumerate(nodes):
        if "mesh" not in n:
            continue
        if "skin" in n:
            s = j["skins"][n["skin"]]
            joints = [index[x] for x in s["joints"]]
            ibm = _accessor(j, blob, s["inverseBindMatrices"])
            for b, m in zip(joints, ibm):
                if b not in bind:
                    bind[b] = fa._invert(_matrix_from_column_major(m))
            add_mesh(n["mesh"], joints, node_bone=index.get(i))
        elif i in index:
            add_mesh(n["mesh"], None, rigid_bone=index[i])

    # Clips.
    clips = []
    for anim in j.get("animations", []):
        tracks, end = {}, 0.0
        for ch in anim["channels"]:
            node = ch["target"]["node"]
            if node not in index:
                continue
            b = index[node]
            s = anim["samplers"][ch["sampler"]]
            times = list(_accessor(j, blob, s["input"]))
            values = _accessor(j, blob, s["output"])
            if not times:
                continue
            end = max(end, times[-1])
            path_ = ch["target"]["path"]
            if path_ == "rotation":
                eulers = [quat_euler(q) for q in values]
                # Keep each angle continuous; flip to the equivalent (x+180, 180-y, z+180) where
                # it is nearer the previous key (avoids jumps at the asin branch).
                fixed = [eulers[0]]
                for e in eulers[1:]:
                    prev = fixed[-1]
                    alt = [e[0] + 180.0, 180.0 - e[1], e[2] + 180.0]
                    def dist(a):
                        return sum(abs((a[k] - prev[k] + 180.0) % 360.0 - 180.0) for k in range(3))
                    fixed.append(alt if dist(alt) < dist(e) else e)
                curves = [(times, _unwrap([e[k] for e in fixed])) for k in range(3)]
                tracks.setdefault(b, {})["R"] = curves
            elif path_ in ("translation", "scale"):
                curves = [(times, [v[k] for v in values]) for k in range(3)]
                tracks.setdefault(b, {})["T" if path_ == "translation" else "S"] = curves
        name = anim.get("name", "").split(" ")[0]
        clips.append(dict(name=name, seconds=end, tracks=tracks))
    # Clips whose name ends in something other than a single clip letter (e.g. "_b_down") go
    # first, so a lookup by last letter finds the plain clip.
    clips.sort(key=lambda c: 1 if re.search(r"_[a-z]$", c["name"]) else 0)
    return dict(bones=bones, skin=skin, bind=bind, mesh=(positions, coords, triangles), clips=clips)


def main(argv=None):
    argv = argv or sys.argv[1:]
    if "--atlas" in argv:
        print("wrote", atlas(argv[0], argv[argv.index("--atlas") + 1]))
        return 0
    model = load(argv[0])
    print(len(model["bones"]), "bones,", len(model["skin"]), "skinned,", len(model["mesh"][2]), "triangles,",
          len(images(argv[0])), "textures")
    for c in model["clips"]:
        print(f"clip {c['name']}: {c['seconds']:.2f} s, {len(c['tracks'])} bones animated")
    return 0


if __name__ == "__main__":
    sys.exit(main())
