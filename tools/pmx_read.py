#!/usr/bin/env python3
"""Reads an MMD .pmx model (PMX 2.0/2.1): vertices with bone weights, faces, textures, materials
and bones, for tools/model_import.py (load_pmx: the mesh as one piece, several textures packed
into one image) and tools/model_skinned.py.

  pmx_read.py MODEL.pmx                 prints the model's parts
  pmx_read.py MODEL.pmx --atlas OUT.png writes the packed texture
"""

import math
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

CELL = 256


class Reader:
    def __init__(self, data):
        self.data, self.at = data, 0

    def take(self, fmt):
        v = struct.unpack_from("<" + fmt, self.data, self.at)
        self.at += struct.calcsize("<" + fmt)
        return v if len(v) > 1 else v[0]

    def text(self, utf8):
        n = self.take("i")
        raw = self.data[self.at:self.at + n]
        self.at += n
        return raw.decode("utf-8" if utf8 else "utf-16-le", "replace")

    def index(self, size, unsigned=False):
        code = {1: "b", 2: "h", 4: "i"}[size]
        return self.take(code.upper() if unsigned and size < 4 else code)


def load(path):
    r = Reader(Path(path).read_bytes())
    if r.data[:4] != b"PMX ":
        raise ValueError("not a PMX file")
    r.at = 4
    version = r.take("f")
    n = r.take("B")
    g = list(r.data[r.at:r.at + n])
    r.at += n
    utf8, extra_uv, vsize, tsize, msize, bsize = g[0] == 1, g[1], g[2], g[3], g[4], g[5]
    name = r.text(utf8)
    r.text(utf8)
    r.text(utf8)
    r.text(utf8)

    positions, normals, coords, weights = [], [], [], []
    for _ in range(r.take("i")):
        positions.append(r.take("3f"))
        normals.append(r.take("3f"))
        coords.append(r.take("2f"))
        r.at += 16 * extra_uv
        kind = r.take("B")
        if kind == 0:
            w = [(r.index(bsize), 1.0)]
        elif kind == 1:
            a, b = r.index(bsize), r.index(bsize)
            t = r.take("f")
            w = [(a, t), (b, 1.0 - t)]
        elif kind in (2, 4):
            ids = [r.index(bsize) for _ in range(4)]
            ws = r.take("4f")
            w = list(zip(ids, ws))
        elif kind == 3:
            a, b = r.index(bsize), r.index(bsize)
            t = r.take("f")
            r.at += 36
            w = [(a, t), (b, 1.0 - t)]
        else:
            raise ValueError(f"unknown weight type {kind}")
        weights.append([(b, x) for b, x in w if b >= 0 and x > 0])
        r.take("f")   # edge scale

    faces = [r.index(vsize, unsigned=True) for _ in range(r.take("i"))]
    textures = [r.text(utf8) for _ in range(r.take("i"))]

    materials = []
    for _ in range(r.take("i")):
        mname = r.text(utf8)
        r.text(utf8)
        diffuse = r.take("4f")
        r.take("3f")
        r.take("f")
        r.take("3f")
        r.take("B")
        r.take("4f")
        r.take("f")
        tex = r.index(tsize)
        r.index(tsize)
        r.take("B")
        toon_shared = r.take("B")
        if toon_shared == 0:
            r.index(tsize)
        else:
            r.take("B")
        r.text(utf8)
        count = r.take("i")
        materials.append(dict(name=mname, diffuse=diffuse, texture=tex, count=count))

    bones = []
    for _ in range(r.take("i")):
        bname = r.text(utf8)
        r.text(utf8)
        pos = r.take("3f")
        parent = r.index(bsize)
        r.take("i")
        flags = r.take("H")
        if flags & 0x1:
            r.index(bsize)
        else:
            r.take("3f")
        if flags & 0x300:
            r.index(bsize)
            r.take("f")
        if flags & 0x400:
            r.take("3f")
        if flags & 0x800:
            r.take("6f")
        if flags & 0x2000:
            r.take("i")
        if flags & 0x20:
            r.index(bsize)
            r.take("i")
            r.take("f")
            for _ in range(r.take("i")):
                r.index(bsize)
                if r.take("B"):
                    r.take("6f")
        bones.append(dict(name=bname, position=pos, parent=parent, flags=flags))

    return dict(name=name, version=version, positions=positions, normals=normals, coords=coords,
                weights=weights, faces=faces, textures=textures, materials=materials, bones=bones)


def texture_files(path, model=None):
    """[Path or None] of the model's textures, looked up next to the model (case-insensitive,
    also by file name alone)."""
    model = model or load(path)
    folder = Path(path).parent
    found = []
    for t in model["textures"]:
        rel = t.replace("\\", "/")
        p = folder / rel
        if not p.is_file():
            name = Path(rel).name.lower()
            p = next((q for q in folder.rglob("*") if q.name.lower() == name), None)
        found.append(p)
    return found


def used_textures(model):
    """Texture indices the materials use, in order (the atlas cells)."""
    out = []
    for m in model["materials"]:
        if m["texture"] >= 0 and m["texture"] not in out:
            out.append(m["texture"])
    return out


def mesh(path):
    """(positions, coords, triangles) as model_import.load_mesh gives them, with each material's
    texture coordinates moved into its texture's atlas cell (wrapped first)."""
    model = load(path)
    used = used_textures(model)
    n = max(1, len(used))
    cols = math.ceil(math.sqrt(n))
    rows = math.ceil(n / cols)
    # MMD is left-handed and its models face -z: z turned round makes it right-handed, facing +z.
    positions = [(p[0], p[1], -p[2]) for p in model["positions"]]
    coords, triangles = [], []
    at = 0
    for m in model["materials"]:
        cell = used.index(m["texture"]) if m["texture"] in used else 0
        col, row = cell % cols, cell // cols
        for k in range(at, at + m["count"], 3):
            corner = model["faces"][k:k + 3]
            us = [model["coords"][v][0] for v in corner]
            vs = [model["coords"][v][1] for v in corner]
            fu, fv = math.floor(min(us)), math.floor(min(vs))
            tri = []
            for v, u, vv in zip(corner, us, vs):
                u = min(1.0, max(0.0, u - fu))
                vv = min(1.0, max(0.0, vv - fv))
                # PMX's v runs down the image; model_import's runs up. One texture: no cells.
                if n == 1:
                    coords.append((u, 1.0 - vv))
                else:
                    coords.append(((col + 0.01 + u * 0.98) / cols, 1.0 - (row + 0.01 + vv * 0.98) / rows))
                tri.append((v, len(coords) - 1))
            triangles.append((tri[0], tri[2], tri[1]))   # PMX winds the other way round
        at += m["count"]
    return positions, coords, triangles


def atlas(path, out):
    import model_texture as mt
    model = load(path)
    files = texture_files(path, model)
    used = used_textures(model)
    n = max(1, len(used))
    cols = math.ceil(math.sqrt(n))
    rows = math.ceil(n / cols)
    sheet = [[(128, 128, 128)] * (CELL * cols) for _ in range(CELL * rows)]
    for cell, t in enumerate(used):
        p = files[t]
        if p is None:
            continue
        w, h, pix = mt.read_png(str(p))
        pix = mt.resize(w, h, pix, CELL, CELL)
        r0, c0 = (cell // cols) * CELL, (cell % cols) * CELL
        for y in range(CELL):
            for x in range(CELL):
                sheet[r0 + y][c0 + x] = tuple(pix[y][x][:3])
    mt.write_png(str(out), CELL * cols, CELL * rows, sheet)
    return out


def main(argv=None):
    argv = argv or sys.argv[1:]
    model = load(argv[0])
    files = texture_files(argv[0], model)
    print(model["name"], "PMX", model["version"], len(model["positions"]), "vertices,", len(model["faces"]) // 3,
          "triangles,", len(model["bones"]), "bones")
    print("textures:", [(t, str(f.name) if f else "MISSING") for t, f in zip(model["textures"], files)])
    for m in model["materials"]:
        print(f"  material {m['name']}: texture {m['texture']}, {m['count'] // 3} triangles")
    ys = [p[1] for p in model["positions"]]
    zs = [p[2] for p in model["positions"]]
    print("y", round(min(ys), 2), round(max(ys), 2), "z", round(min(zs), 2), round(max(zs), 2))
    if "--atlas" in argv:
        print("wrote", atlas(argv[0], argv[argv.index("--atlas") + 1]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
