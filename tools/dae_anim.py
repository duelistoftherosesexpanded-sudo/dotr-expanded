#!/usr/bin/env python3
"""Reads a skinned Collada (.dae) model and its motions for tools/model_skinned.py, in the form
tools/fbx_anim.load gives (see there): bones, skin, bind, mesh, clips.

Made for rips whose joints are <node type="JOINT"> with translate, rotateZ, rotateY, rotateX and
scale (so T * Rz * Ry * Rx * S, as DotR builds a node) and whose motions come as separate files
next to the model: "<model stem>_Motion<X>.dae" (or any .dae in the folder with animations), each
one clip named after the file ("MotionA" -> clip letter "a"). Motion channels target
"<joint id>/translate", "<joint id>/rotateX.ANGLE" (Y, Z) and "<joint id>/scale".

Several materials: their images are packed side by side into one texture (atlas()), and the
mesh's texture coordinates are moved into each image's column; model_skinned takes one texture.

  dae_anim.py MODEL.dae                    prints the bones and clips
  dae_anim.py MODEL.dae --atlas OUT.png    writes the packed texture
"""

import math
import re
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))


def _tag(e):
    return e.tag.split("}")[-1]


def _kids(e, name):
    return [c for c in e if _tag(c) == name]


def _kid(e, name):
    return next((c for c in e if _tag(c) == name), None)


def _all(e, name):
    return [c for c in e.iter() if _tag(c) == name]


def _floats(text):
    return [float(x) for x in (text or "").split()]


def _invert(m):
    a = [list(row) + [float(i == j) for j in range(4)] for i, row in enumerate(m)]
    for c in range(4):
        p = max(range(c, 4), key=lambda r: abs(a[r][c]))
        a[c], a[p] = a[p], a[c]
        d = a[c][c]
        a[c] = [x / d for x in a[c]]
        for r in range(4):
            if r != c:
                f = a[r][c]
                a[r] = [x - f * y for x, y in zip(a[r], a[c])]
    return [row[4:] for row in a]


def _matrix16(values):
    """Collada matrices are row-major (column vectors)."""
    return [values[r * 4:r * 4 + 4] for r in range(4)]


def _sources(parent):
    out = {}
    for s in _kids(parent, "source"):
        fa, na = _kid(s, "float_array"), _kid(s, "Name_array")
        acc = next(iter(_all(s, "accessor")), None)
        stride = int(acc.get("stride", "1")) if acc is not None else 1
        out[s.get("id")] = (_floats(fa.text) if fa is not None else (na.text or "").split(), stride)
    return out


def _joint_transform(node):
    t, r, s = [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [1.0, 1.0, 1.0]
    order = []
    for c in node:
        kind = _tag(c)
        v = _floats(c.text)
        if kind == "translate":
            t = v[:3]
        elif kind == "scale":
            s = v[:3]
        elif kind == "rotate":
            axis = max(range(3), key=lambda i: abs(v[i]))
            r[axis] = v[3] * (1 if v[axis] > 0 else -1)
            order.append(axis)
        elif kind == "matrix":
            raise ValueError(f"joint {node.get('id')} has a matrix transform; not supported")
    if order and order != [2, 1, 0]:
        raise ValueError(f"joint {node.get('id')} rotates in the order {order}; only Z, Y, X is supported")
    return t, r, s


def _materials(root, base):
    """{material id: image path} through effect -> surface -> image."""
    images = {i.get("id"): base / (_kid(i, "init_from").text or "").strip() for i in _all(root, "image")}
    effects = {}
    for eff in _all(root, "effect"):
        img = None
        for np_ in _all(eff, "newparam"):
            surf = _kid(np_, "surface")
            if surf is not None and _kid(surf, "init_from") is not None:
                img = images.get(_kid(surf, "init_from").text.strip())
        if img is None:
            tex = next(iter(_all(eff, "texture")), None)
            if tex is not None:
                img = images.get(tex.get("texture"))
        effects[eff.get("id")] = img
    out = {}
    for m in _all(root, "material"):
        ie = _kid(m, "instance_effect")
        if ie is not None:
            out[m.get("id")] = effects.get(ie.get("url", "").lstrip("#"))
    return out


def _bound_materials(root):
    """{(geometry id, material symbol): material id} from the scene's instance_controller binds."""
    controllers = {c.get("id"): _kid(c, "skin").get("source", "").lstrip("#") for c in _all(root, "controller")}
    out = {}
    for ic in _all(root, "instance_controller"):
        geo = controllers.get(ic.get("url", "").lstrip("#"))
        for im in _all(ic, "instance_material"):
            out[(geo, im.get("symbol"))] = im.get("target", "").lstrip("#")
    for ig in _all(root, "instance_geometry"):
        for im in _all(ig, "instance_material"):
            out[(ig.get("url", "").lstrip("#"), im.get("symbol"))] = im.get("target", "").lstrip("#")
    return out


def images(path):
    """The model's texture images in atlas order."""
    path = Path(path)
    root = ET.parse(path).getroot()
    mats = _materials(root, path.parent)
    found = []
    for img in mats.values():
        if img is not None and img not in found:
            found.append(img)
    return sorted(found, key=lambda p: p.name)


def atlas(path, out):
    """Write the model's images side by side (each scaled to the tallest's height) to `out`."""
    import model_texture as mt
    pics = [mt.read_png(str(p)) for p in images(path)]
    if not pics:
        raise ValueError("the model names no texture images")
    h = max(p[1] for p in pics)
    w = max(p[0] for p in pics)
    columns = [mt.resize(pw, ph, rows, w, h) for pw, ph, rows in pics]
    rows = [sum((list(col[y]) for col in columns), []) for y in range(h)]
    mt.write_png(str(out), w * len(columns), h, rows)
    return out


def _clip(root, name, joint_by_id):
    tracks, end = {}, 0.0
    for anim in _all(root, "animation"):
        sources = _sources(anim)
        samplers = {s.get("id"): {i.get("semantic"): i.get("source", "").lstrip("#") for i in _kids(s, "input")}
                    for s in _kids(anim, "sampler")}
        for ch in _kids(anim, "channel"):
            target = ch.get("target", "")
            if "/" not in target:
                continue
            node, prop = target.split("/", 1)
            bone = joint_by_id.get(node)
            if bone is None:
                continue
            sampler = samplers.get(ch.get("source", "").lstrip("#"), {})
            times, _ = sources.get(sampler.get("INPUT"), ([], 1))
            values, stride = sources.get(sampler.get("OUTPUT"), ([], 1))
            if not times:
                continue
            end = max(end, times[-1])
            entry = tracks.setdefault(bone, {})
            m = re.match(r"(translate|scale|rotate([XYZ]))(\.(ANGLE|X|Y|Z))?$", prop)
            if not m:
                continue
            if m.group(1) in ("translate", "scale"):
                channel = "T" if m.group(1) == "translate" else "S"
                curves = entry.setdefault(channel, [None, None, None])
                if stride == 3:
                    for k in range(3):
                        curves[k] = (list(times), values[k::3])
                elif m.group(4) in ("X", "Y", "Z"):
                    curves["XYZ".index(m.group(4))] = (list(times), values)
            else:
                curves = entry.setdefault("R", [None, None, None])
                curves["XYZ".index(m.group(2))] = (list(times), values)
    return dict(name=name, seconds=end, tracks=tracks)


def load(path, motions=None):
    path = Path(path)
    root = ET.parse(path).getroot()

    # Bones: the JOINT nodes, parents before children.
    bones, joint_by_id, by_name = [], {}, {}

    def walk(node, parent):
        index = parent
        if node.get("type") == "JOINT":
            t, r, s = _joint_transform(node)
            index = len(bones)
            name = node.get("name") or node.get("id")
            bones.append(dict(id=node.get("id"), name=name, parent=parent, t=t, r=r, s=s))
            joint_by_id[node.get("id")] = index
            for key in (node.get("id"), node.get("sid"), name):
                if key:
                    by_name[key] = index
        for c in _kids(node, "node"):
            walk(c, index)

    for scene in _all(root, "visual_scene"):
        for n in _kids(scene, "node"):
            walk(n, -1)
    if not bones:
        raise ValueError("the Collada file has no joints")

    # Materials -> atlas columns.
    mats = _materials(root, path.parent)
    order = images(path)
    bound = _bound_materials(root)

    # Mesh and skin: each controller's geometry, positions through the bind shape matrix.
    positions, coords, triangles = [], [], []
    skin, bind = {}, {}
    geometries = {g.get("id"): g for g in _all(root, "geometry")}
    for ctrl in _all(root, "controller"):
        sk = _kid(ctrl, "skin")
        geo = geometries.get(sk.get("source", "").lstrip("#"))
        if geo is None:
            continue
        bsm = _matrix16(_floats(_kid(sk, "bind_shape_matrix").text)) if _kid(sk, "bind_shape_matrix") is not None \
            else [[float(i == j) for j in range(4)] for i in range(4)]
        sources = _sources(sk)
        joints_in = {i.get("semantic"): i.get("source", "").lstrip("#") for i in _kids(_kid(sk, "joints"), "input")}
        names = sources[joints_in["JOINT"]][0]
        inv = sources[joints_in["INV_BIND_MATRIX"]][0]
        joint_index = []
        for k, n in enumerate(names):
            b = by_name.get(n)
            if b is None:
                raise ValueError(f"skin joint {n} is not in the skeleton")
            joint_index.append(b)
            world = _invert(_matrix16(inv[16 * k:16 * k + 16]))
            bind.setdefault(b, world)

        mesh = _kid(geo, "mesh")
        msrc = _sources(mesh)
        verts = _kid(mesh, "vertices")
        pos_src = next(i.get("source").lstrip("#") for i in _kids(verts, "input") if i.get("semantic") == "POSITION")
        raw = msrc[pos_src][0]
        base = len(positions)
        for k in range(0, len(raw), 3):
            p = raw[k:k + 3] + [1.0]
            positions.append(tuple(sum(bsm[i][j] * p[j] for j in range(4)) for i in range(3)))

        vw = _kid(sk, "vertex_weights")
        vin = {i.get("semantic"): (int(i.get("offset")), i.get("source", "").lstrip("#")) for i in _kids(vw, "input")}
        weights = sources[vin["WEIGHT"][1]][0]
        vcount = [int(x) for x in _kid(vw, "vcount").text.split()]
        v = [int(x) for x in _kid(vw, "v").text.split()]
        width = len(vin)
        at = 0
        for vert, n in enumerate(vcount):
            for _ in range(n):
                j, w = v[at + vin["JOINT"][0]], weights[v[at + vin["WEIGHT"][0]]]
                at += width
                if j >= 0 and w > 0:
                    skin.setdefault(joint_index[j], []).append((base + vert, w))

        for tris in _kids(mesh, "triangles") + _kids(mesh, "polylist"):
            inputs = _kids(tris, "input")
            stride = max(int(i.get("offset")) for i in inputs) + 1
            offs = {i.get("semantic"): (int(i.get("offset")), i.get("source", "").lstrip("#")) for i in inputs}
            image = mats.get(bound.get((geo.get("id"), tris.get("material")), tris.get("material")))
            column = order.index(image) if image in order else 0
            uv_base = len(coords)
            if "TEXCOORD" in offs:
                uv, uv_stride = msrc[offs["TEXCOORD"][1]]
                for k in range(0, len(uv), uv_stride):
                    coords.append(((column + uv[k]) / max(1, len(order)), uv[k + 1]))
            idx = [int(x) for x in _kid(tris, "p").text.split()]
            if _tag(tris) == "polylist" and any(int(x) != 3 for x in _kid(tris, "vcount").text.split()):
                raise ValueError("only triangles are supported")
            for k in range(0, len(idx), 3 * stride):
                corner = []
                for c in range(3):
                    q = idx[k + c * stride:k + (c + 1) * stride]
                    corner.append((base + q[offs["VERTEX"][0]],
                                   uv_base + q[offs["TEXCOORD"][0]] if "TEXCOORD" in offs else -1))
                triangles.append(tuple(corner))

    # Clips: the motion files next to the model, else the model's own animations.
    if motions is None:
        motions = sorted(p for p in path.parent.glob(f"{path.stem}_Motion*.dae"))
    clips = []
    for m in motions:
        mroot = ET.parse(m).getroot()
        name = Path(m).stem[len(path.stem) + 1:] if Path(m).stem.startswith(path.stem + "_") else Path(m).stem
        clips.append(_clip(mroot, name, joint_by_id))
    if not motions and _all(root, "animation"):
        clips.append(_clip(root, path.stem, joint_by_id))
    return dict(bones=bones, skin=skin, bind=bind, mesh=(positions, coords, triangles), clips=clips)


def main(argv=None):
    argv = argv or sys.argv[1:]
    if "--atlas" in argv:
        i = argv.index("--atlas")
        print("wrote", atlas(argv[0], argv[i + 1]))
        return 0
    model = load(argv[0])
    print(len(model["bones"]), "bones,", len(model["skin"]), "skinned,", len(model["mesh"][2]), "triangles")
    print("images", [p.name for p in images(argv[0])])
    for c in model["clips"]:
        print(f"clip {c['name']}: {c['seconds']:.2f} s, {len(c['tracks'])} bones animated")
    return 0


if __name__ == "__main__":
    sys.exit(main())
