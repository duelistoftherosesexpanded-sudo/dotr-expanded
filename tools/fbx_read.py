#!/usr/bin/env python3
"""Reads the mesh of a binary FBX file (versions 7.x) for tools/model_import.py: positions, texture
coordinates and triangles, as load_obj gives them. Optionally lowers the arms of a model stored in a
T-pose ("arms_down", in degrees), using the file's skeleton and skin weights.

  fbx_read.py FILE [--arms-down DEGREES] [--obj OUT.obj]   prints a summary, optionally writes OBJ

Only what a static, single-texture import needs is read: the first mesh, its first UV set, and the
skin clusters (bones, their bind positions and the weights of each vertex).
"""

import argparse
import math
import re
import struct
import sys
import zlib
from pathlib import Path


def parse(path):
    """(version, top-level nodes) of a binary FBX. A node is (name, properties, children)."""
    data = Path(path).read_bytes()
    if not data.startswith(b"Kaydara FBX Binary"):
        raise ValueError(f"{path} is not a binary FBX file (export it as binary FBX, or as OBJ)")
    version = struct.unpack_from("<I", data, 23)[0]
    wide = version >= 7500

    def node(at):
        if wide:
            end, count, _ = struct.unpack_from("<QQQ", data, at)
            at += 24
        else:
            end, count, _ = struct.unpack_from("<III", data, at)
            at += 12
        length = data[at]
        at += 1
        if end == 0:
            return None, at
        name = data[at:at + length].decode("ascii", "replace")
        at += length
        props = []
        for _ in range(count):
            kind = chr(data[at])
            at += 1
            if kind in "YCIFDL":
                fmt = {"Y": "<h", "C": "<?", "I": "<i", "F": "<f", "D": "<d", "L": "<q"}[kind]
                props.append(struct.unpack_from(fmt, data, at)[0])
                at += struct.calcsize(fmt)
            elif kind in "fdlib":
                n, encoding, size = struct.unpack_from("<III", data, at)
                at += 12
                raw = data[at:at + size]
                at += size
                if encoding == 1:
                    raw = zlib.decompress(raw)
                fmt = {"f": "f", "d": "d", "l": "q", "i": "i", "b": "?"}[kind]
                props.append(list(struct.unpack(f"<{n}{fmt}", raw[:n * struct.calcsize(fmt)])))
            elif kind in "SR":
                n = struct.unpack_from("<I", data, at)[0]
                at += 4
                raw = data[at:at + n]
                at += n
                props.append(raw.decode("utf-8", "replace") if kind == "S" else raw)
            else:
                raise ValueError(f"unknown FBX property type {kind!r}")
        children = []
        while at < end:
            child, at = node(at)
            if child is None:
                break
            children.append(child)
        return (name, props, children), end

    roots, at = [], 27
    while at < len(data) - 160:
        n, at = node(at)
        if n is None:
            break
        roots.append(n)
    return version, roots


def find(nodes, name):
    for n in nodes:
        if n[0] == name:
            yield n
        yield from find(n[2], name)


def child(n, name):
    return next((c for c in n[2] if c[0] == name), None)


def short(name):
    return name.split("\x00")[0]


def rotation(axis, angle):
    x, y, z = axis
    c, s = math.cos(angle), math.sin(angle)
    t = 1 - c
    return [[t * x * x + c, t * x * y - s * z, t * x * z + s * y],
            [t * x * y + s * z, t * y * y + c, t * y * z - s * x],
            [t * x * z - s * y, t * y * z + s * x, t * z * z + c]]


def load_fbx(path, arms_down=0.0, report=None):
    """(positions, coords, triangles) like model_import.load_obj: triangles are three
    (position index, coordinate index) corners."""
    _, roots = parse(path)
    objects = next(find(roots, "Objects"))
    geometry = next((n for n in objects[2] if n[0] == "Geometry" and child(n, "Vertices")), None)
    if geometry is None:
        raise ValueError("the FBX has no mesh")
    flat = child(geometry, "Vertices")[1][0]
    positions = [[flat[i], flat[i + 1], flat[i + 2]] for i in range(0, len(flat), 3)]
    polygon = child(geometry, "PolygonVertexIndex")[1][0]
    layer = child(geometry, "LayerElementUV")
    if layer is None:
        raise ValueError("the FBX mesh has no texture coordinates")
    uv = child(layer, "UV")[1][0]
    coords = [(uv[i], uv[i + 1]) for i in range(0, len(uv), 2)]
    uv_index = child(layer, "UVIndex")
    mapping = short(child(layer, "MappingInformationType")[1][0])
    triangles, corners, corner = [], [], 0
    for value in polygon:
        vertex = value if value >= 0 else ~value
        if mapping.startswith("ByPolygonVertex"):
            c = uv_index[1][0][corner] if uv_index else corner
        else:
            c = uv_index[1][0][vertex] if uv_index else vertex
        corners.append((vertex, c))
        corner += 1
        if value < 0:
            for k in range(1, len(corners) - 1):
                triangles.append((corners[0], corners[k], corners[k + 1]))
            corners = []
    info = dict(vertices=len(positions), triangles=len(triangles))
    if arms_down:
        info["posed"] = lower_arms(roots, objects, positions, math.radians(arms_down))
    if report is not None:
        report.update(info)
    return [tuple(p) for p in positions], coords, triangles


def lower_arms(roots, objects, positions, angle):
    """Turns each upper-arm bone (and every bone below it) about its joint so the arm points down
    by `angle` from where it was, moving each vertex by the weights of its bones. Returns the bones
    turned."""
    kinds = {n[1][0]: n[1][2] for n in objects[2] if n[0] in ("Model", "Deformer")}
    names = {n[1][0]: short(n[1][1]) for n in objects[2] if n[0] in ("Model", "Deformer")}
    parent, links = {}, []
    for c in next(find(roots, "Connections"))[2]:
        if c[1][0] != "OO":
            continue
        links.append((c[1][1], c[1][2]))
        if kinds.get(c[1][1]) in ("LimbNode", "Null") and kinds.get(c[1][2]) in ("LimbNode", "Null"):
            parent[c[1][1]] = c[1][2]
    clusters = {n[1][0]: n for n in objects[2] if n[0] == "Deformer" and n[1][2] == "Cluster"}
    bone_of = {par: ch for ch, par in links if par in clusters and kinds.get(ch) in ("LimbNode", "Null")}
    joint = {}
    for cid, c in clusters.items():
        link = child(c, "TransformLink")
        if link and cid in bone_of:
            m = link[1][0]
            joint[bone_of[cid]] = (m[12], m[13], m[14])
    upper = [b for b, n in names.items() if b in joint and
             re.fullmatch(r"(?i)(l|r|left|right)[_ ]?(\d+[_ ])?(upper)?arm0?1?|(upper)?arm[_ ]?(l|r|left|right)0?1?", n)]
    # Two arms, or pairs of them (L_01_Arm01 and L_02_Arm01 on a four-armed model).
    if len(upper) < 2 or len(upper) % 2:
        raise ValueError("could not find the two upper-arm bones (L_Arm01 / R_Arm01) to lower the arms; "
                         "turn off arms down, or pose the model in Blender")

    def below(bone):
        while bone is not None:
            if bone in turn:
                return turn[bone]
            bone = parent.get(bone)
        return None

    turn = {}
    for bone in upper:
        # The arm's direction: from the joint to the furthest joint below it (the hand).
        tips = [b for b in joint if bone in iter_up(b, parent)]
        if not tips:
            raise ValueError(f"the bone {names[bone]} has no bones below it")
        tip = max(tips, key=lambda b: sum((joint[b][i] - joint[bone][i]) ** 2 for i in range(3)))
        d = [joint[tip][i] - joint[bone][i] for i in range(3)]
        length = math.sqrt(sum(v * v for v in d))
        d = [v / length for v in d]
        # Turning about d x (0, -1, 0) moves d towards straight down.
        axis = [d[2], 0.0, -d[0]]
        norm = math.sqrt(sum(v * v for v in axis)) or 1.0
        turn[bone] = (rotation([v / norm for v in axis], angle), joint[bone])
    weights = [[] for _ in positions]
    for cid, c in clusters.items():
        indexes, values = child(c, "Indexes"), child(c, "Weights")
        if cid in bone_of and indexes and values:
            for i, w in zip(indexes[1][0], values[1][0]):
                weights[i].append((bone_of[cid], w))
    for i, p in enumerate(positions):
        total = sum(w for _, w in weights[i])
        if not total:
            continue
        moved = [0.0, 0.0, 0.0]
        for bone, w in weights[i]:
            t = below(bone)
            if t is None:
                q = p
            else:
                r, j = t
                v = [p[k] - j[k] for k in range(3)]
                q = [sum(r[a][b] * v[b] for b in range(3)) + j[a] for a in range(3)]
            for k in range(3):
                moved[k] += q[k] * w / total
        positions[i] = moved
    return [names[b] for b in upper]


def iter_up(bone, parent):
    while bone in parent:
        bone = parent[bone]
        yield bone


def write_obj(path, positions, coords, triangles):
    lines = [f"v {x:.6f} {y:.6f} {z:.6f}" for x, y, z in positions]
    lines += [f"vt {u:.6f} {v:.6f}" for u, v in coords]
    lines += ["f " + " ".join(f"{p + 1}/{c + 1}" for p, c in t) for t in triangles]
    Path(path).write_text("\n".join(lines) + "\n")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("fbx")
    parser.add_argument("--arms-down", type=float, default=0.0)
    parser.add_argument("--obj")
    args = parser.parse_args(argv)
    report = {}
    positions, coords, triangles = load_fbx(args.fbx, args.arms_down, report)
    print(report)
    if args.obj:
        write_obj(args.obj, positions, coords, triangles)
    return 0


if __name__ == "__main__":
    sys.exit(main())
