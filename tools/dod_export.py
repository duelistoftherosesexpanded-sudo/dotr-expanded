#!/usr/bin/env python3
"""Exports Yu-Gi-Oh! The Dawn of Destiny (Xbox) monster models to glTF (.glb), as
tools/crossduel_export.py does for Cross Duel, so tools/glb_anim.py and tools/model_skinned.py
take them (cards.json "model": the .glb, "model_animated": true).

The player's own disc is read directly (tools/igb_read.py: XDVDFS, Alchemy .igb). A monster is
monster/<hundreds>/m<NNN>.igb; it holds:

  scene      igSceneInfo -> groups -> a root igTransform ("mNNN_all") -> igJoints and helper
             igTransforms ("chnNN"); every node's local matrix is row-major with row vectors, the
             same 16 floats a glTF column-major matrix takes
  meshes     igGeometry under igAttrSets (material: igTextureBindAttr -> igTextureAttr -> igImage;
             skin: igVertexBlendMatrixListAttr, a palette of igMatrixObjects, each a joint's);
             igGeometryAttr1_5 -> igVertexArray1_1 (a table of 19 component arrays: 0 positions,
             1 normals, 11 the first texture coordinates; then the blend weights, packed floats,
             and the blend indices into the piece's palette, packed bytes: as many a vertex as
             bits 4-7 and 8-11 of the vertex format word say) drawn as triangle strips
             (igPrimLengthArray1_1) or lists
  textures   igImage: width, height, components (3: RGB 8-8-8, 1: one channel), its pixel block
  animation  per animated node an igTransformSequence1_5: translation, rotation (quaternion x y z
             w, stored conjugated) and scale key lists and their times (nanoseconds); the
             monster's one loop (2 s at 30 keys a second on the ones seen), one clip "<model>_a"
  skin       a joint's stored bind matrix is the inverse of its bind pose relative to the
             skeleton's parent node, the mesh's own space; each piece has its own palette

Model mNNN is the card at place NNN of the game's card list (default.xbe, 1,138 Konami card
numbers from 0x5d0c44: m000 Blue-Eyes White Dragon, m007 Mushroom Man).

  dod_export.py ISO MODEL OUT_DIR [--check]     MODEL like m007 (or a disc path)
"""

import argparse
import math
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import igb_read  # noqa: E402
import model_texture as mt  # noqa: E402

STRIP = 4                    # igGeometryAttr primitive type: triangle strip
SLOT_POSITION, SLOT_NORMAL, SLOT_UV = 0, 1, 11
WEIGHTS = 3


def model_path(iso, model):
    if model.startswith("/"):
        return model
    n = int(model.lstrip("mM"))
    return f"/monster/{n // 100 * 100:03d}/m{n:03d}.igb"


# ------------------------------------------------------------------------------- matrices

def mat_mul(a, b):
    """Column-major 4x4 product a * b (column vectors)."""
    return [sum(a[k * 4 + r] * b[c * 4 + k] for k in range(4)) for c in range(4) for r in range(4)]


def mat_inverse(m):
    """Inverse of an affine column-major 4x4."""
    r = [[m[c * 4 + rr] for c in range(3)] for rr in range(3)]
    det = (r[0][0] * (r[1][1] * r[2][2] - r[1][2] * r[2][1]) - r[0][1] * (r[1][0] * r[2][2] - r[1][2] * r[2][0])
           + r[0][2] * (r[1][0] * r[2][1] - r[1][1] * r[2][0]))
    inv = [[(r[(j + 1) % 3][(i + 1) % 3] * r[(j + 2) % 3][(i + 2) % 3]
             - r[(j + 1) % 3][(i + 2) % 3] * r[(j + 2) % 3][(i + 1) % 3]) / det for j in range(3)] for i in range(3)]
    t = m[12:15]
    ti = [-sum(inv[i][k] * t[k] for k in range(3)) for i in range(3)]
    return [inv[0][0], inv[1][0], inv[2][0], 0, inv[0][1], inv[1][1], inv[2][1], 0,
            inv[0][2], inv[1][2], inv[2][2], 0, ti[0], ti[1], ti[2], 1]


def decompose(m):
    """(translation, rotation quaternion xyzw, scale) of a column-major affine matrix."""
    t = list(m[12:15])
    cols = [m[0:3], m[4:7], m[8:11]]
    s = [math.sqrt(sum(v * v for v in c)) or 1.0 for c in cols]
    r = [[cols[c][rr] / s[c] for c in range(3)] for rr in range(3)]
    det = (r[0][0] * (r[1][1] * r[2][2] - r[1][2] * r[2][1]) - r[0][1] * (r[1][0] * r[2][2] - r[1][2] * r[2][0])
           + r[0][2] * (r[1][0] * r[2][1] - r[1][1] * r[2][0]))
    if det < 0:
        s[0] = -s[0]
        r = [[-r[rr][0], r[rr][1], r[rr][2]] for rr in range(3)]
    return t, matrix_quat(r), s


def matrix_quat(r):
    tr = r[0][0] + r[1][1] + r[2][2]
    if tr > 0:
        k = math.sqrt(tr + 1.0) * 2
        q = [(r[2][1] - r[1][2]) / k, (r[0][2] - r[2][0]) / k, (r[1][0] - r[0][1]) / k, 0.25 * k]
    elif r[0][0] > r[1][1] and r[0][0] > r[2][2]:
        k = math.sqrt(1.0 + r[0][0] - r[1][1] - r[2][2]) * 2
        q = [0.25 * k, (r[0][1] + r[1][0]) / k, (r[0][2] + r[2][0]) / k, (r[2][1] - r[1][2]) / k]
    elif r[1][1] > r[2][2]:
        k = math.sqrt(1.0 + r[1][1] - r[0][0] - r[2][2]) * 2
        q = [(r[0][1] + r[1][0]) / k, 0.25 * k, (r[1][2] + r[2][1]) / k, (r[0][2] - r[2][0]) / k]
    else:
        k = math.sqrt(1.0 + r[2][2] - r[0][0] - r[1][1]) * 2
        q = [(r[0][2] + r[2][0]) / k, (r[1][2] + r[2][1]) / k, 0.25 * k, (r[1][0] - r[0][1]) / k]
    n = math.sqrt(sum(v * v for v in q)) or 1.0
    return [v / n for v in q]


def quat_dot(a, b):
    return abs(sum(x * y for x, y in zip(a, b)))


# -------------------------------------------------------------------------------- reading

class Monster:
    def __init__(self, igb):
        self.g = g = igb
        info = next(i for i in g.objects if g.tname(i) == "igSceneInfo")
        self.nodes = []          # dict(id, name, matrix, children (node indices), seq)
        self.node_of = {}
        self.geoms = []          # (geometry attr, texture image or None, blend palette or None)
        self._walk(g.f(info)[2], None, None, None)

    def _walk(self, i, parent, image, palette):
        g = self.g
        t = g.tname(i)
        f = g.f(i)
        node = parent
        if t in ("igGroup", "igTransform", "igJoint"):
            if t == "igGroup":
                matrix, seq = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1], -1
            else:
                matrix, seq = list(f[4]), f[6]
            node = len(self.nodes)
            self.nodes.append(dict(id=i, name=f[0] or f"{t}{i}", matrix=matrix, children=[], seq=seq,
                                   joint=t == "igJoint", bind=list(f[7]) if t == "igJoint" else None,
                                   blend=f[8] if t == "igJoint" else -1))
            self.node_of[i] = node
            if parent is not None:
                self.nodes[parent]["children"].append(node)
        if t == "igAttrSet":
            for a in g.list_items(f[4]):
                at = g.tname(a)
                if at == "igTextureBindAttr" and g.f(a)[2] == 0 and image is None:
                    tex = g.f(a)[1]
                    if tex >= 0:
                        image = g.f(tex)[8]
                elif at == "igVertexBlendMatrixListAttr":
                    palette = g.list_items(g.f(a)[1])
        if t == "igGeometry":
            for a in g.list_items(f[4]):
                if g.tname(a).startswith("igGeometryAttr"):
                    self.geoms.append((a, image, palette, node))
            return
        if t in ("igGroup", "igAttrSet", "igTransform", "igJoint", "igSimpleShader", "igShader2"):
            for c in g.list_items(f[3]):
                self._walk(c, node, image, palette)

    def image_rows(self, i):
        g = self.g
        f = g.f(i)
        w, h, comps = f[0], f[1], f[2]
        data = g.memory[f[11]]
        # An 8-bit paletted image (format 65536, "..._CI8"): each byte indexes the colour table
        # of the igClut in field 15 (entries of 4 bytes, RGBA, in its memory at field 3).
        palette = None
        if f[9] == 65536 and f[15] >= 0 and g.tname(f[15]) == "igClut":
            cf = g.f(f[15])
            pal, stride = g.memory[cf[3]], max(3, cf[2])
            palette = [tuple(pal[stride * k:stride * k + 3]) for k in range(min(cf[1], len(pal) // stride))]
        rows = []
        for y in range(h):
            row = []
            for x in range(w):
                k = (y * w + x) * comps
                if palette is not None:
                    c = data[y * w + x]
                    row.append(palette[c] if c < len(palette) else (0, 0, 0))
                elif comps >= 3:
                    row.append((data[k], data[k + 1], data[k + 2]))
                else:
                    row.append((data[k],) * 3)
            rows.append(row)
        name = f[-1] if isinstance(f[-1], str) and f[-1] else f"image{i}.png"
        return w, h, rows, name

    def geometry(self, attr):
        """(positions, normals, uvs, joints (palette indices), weights, triangle indices)."""
        g = self.g
        fa = g.f(attr)
        va, prim, count, offset, lengths = fa[1], fa[3], fa[4], fa[5], fa[10]
        fv = g.f(va)
        n = fv[1]
        table = struct.unpack_from("<19i", g.memory[fv[0]])

        def comp(slot, fmt):
            if table[slot] < 0:
                return None
            d = g.memory[table[slot]]
            size = struct.calcsize("<" + fmt)
            return [struct.unpack_from("<" + fmt, d, k * size) for k in range(n)]

        pos = comp(SLOT_POSITION, "3f")
        nrm = comp(SLOT_NORMAL, "3f")
        uv = comp(SLOT_UV, "2f") or [(0.0, 0.0)] * n
        joints, weights = None, None
        if fv[5] >= 0 and fv[6] >= 0:
            # The vertex format word: bits 4-7 the weights a vertex (packed floats, no padding),
            # bits 8-11 its bone indices (bytes, in a slot of len / count bytes).
            wd, jd = g.memory[fv[5]], g.memory[fv[6]]
            wn = max(1, (fv[4] >> 4) & 0xF)
            jn = max(1, (fv[4] >> 8) & 0xF)
            weights, joints = [], []
            for k in range(n):
                w = list(struct.unpack_from(f"<{wn}f", wd, 4 * wn * k))
                j = list(jd[jn * k:jn * k + jn])
                pairs = list(zip(w + [0.0] * (len(j) - len(w)), j + [0] * (len(w) - len(j))))
                if len(pairs) > 4:      # glTF takes 4: keep the strongest
                    pairs = sorted(pairs, key=lambda p: -p[0])[:4]
                pairs += [(0.0, 0)] * (4 - len(pairs))
                weights.append(tuple(p[0] for p in pairs))
                joints.append(tuple(p[1] for p in pairs))
        # Triangle strips (or a list) into triangles.
        tris = []
        lens = g.list_items(lengths, "I") if lengths >= 0 and g.tname(lengths) != "igPrimLengthArray1_1" else None
        if lens is None and lengths >= 0:
            fl = g.f(lengths)
            lens = list(struct.unpack_from(f"<{fl[1]}I", g.memory[fl[0]]))
        start = offset
        if prim == STRIP:
            for L in (lens or [n]):
                for k in range(L - 2):
                    a, b, c = start + k, start + k + 1, start + k + 2
                    if k % 2:
                        a, b = b, a
                    if a != b and b != c and a != c and pos[a] != pos[b] and pos[b] != pos[c] and pos[a] != pos[c]:
                        tris.append((a, b, c))
                start += L
        else:
            tris = [(k, k + 1, k + 2) for k in range(offset, offset + 3 * count, 3)]
        return pos, nrm, uv, joints, weights, tris

    def sequence(self, seq):
        """(times s, translations, rotations xyzw, scales) of an igTransformSequence1_5."""
        g = self.g
        f = g.f(seq)
        times = [t / 1e9 for t in g.list_items(f[8], "q")]
        trans = g.list_items(f[0], "3f") if f[0] >= 0 else None
        # Stored conjugated (the first key of every track is the node's rest rotation only so).
        rots = [(-x, -y, -z, w) for x, y, z, w in g.list_items(f[1], "4f")] if f[1] >= 0 else None
        scales = g.list_items(f[2], "3f") if f[2] >= 0 else None
        return times, trans, rots, scales


# -------------------------------------------------------------------------------- writing

class Glb:
    def __init__(self):
        self.json = dict(asset=dict(version="2.0", generator="dotr dod_export"), buffers=[], bufferViews=[],
                         accessors=[])
        self.blob = bytearray()

    def view(self, data):
        while len(self.blob) % 4:
            self.blob += b"\0"
        self.json["bufferViews"].append(dict(buffer=0, byteOffset=len(self.blob), byteLength=len(data)))
        self.blob += data
        return len(self.json["bufferViews"]) - 1

    def accessor(self, values, kind, fmt, comp, minmax=False):
        n = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}[kind]
        flat = [v for row in values for v in (row if n > 1 else [row])]
        a = dict(bufferView=self.view(struct.pack(f"<{len(flat)}{fmt}", *flat)), componentType=comp,
                 count=len(values), type=kind)
        if minmax:
            a["min"] = [min(row[k] for row in values) for k in range(n)] if n > 1 else [min(values)]
            a["max"] = [max(row[k] for row in values) for k in range(n)] if n > 1 else [max(values)]
        self.json["accessors"].append(a)
        return len(self.json["accessors"]) - 1

    def write(self, path):
        while len(self.blob) % 4:
            self.blob += b"\0"
        self.json["buffers"] = [dict(byteLength=len(self.blob))]
        import json
        js = json.dumps(self.json, separators=(",", ":")).encode()
        while len(js) % 4:
            js += b" "
        out = struct.pack("<III", 0x46546C67, 2, 12 + 8 + len(js) + 8 + len(self.blob))
        out += struct.pack("<II", len(js), 0x4E4F534A) + js + struct.pack("<II", len(self.blob), 0x004E4942) + bytes(self.blob)
        Path(path).write_bytes(out)


def png_bytes(w, h, rows):
    with tempfile.NamedTemporaryFile(suffix=".png", delete=False) as f:
        name = f.name
    mt.write_png(name, w, h, rows)
    data = Path(name).read_bytes()
    Path(name).unlink()
    return data


def export(iso_path, model, out_dir, check=False):
    iso = igb_read.XIso(iso_path)
    path = model_path(iso, model)
    m = Monster(igb_read.Igb(iso.read(path)))
    base = Path(path).stem
    glb = Glb()
    j = glb.json
    # Nodes.
    j["nodes"] = []
    for n in m.nodes:
        t, r, s = decompose(n["matrix"])
        # Rounded: the last bits of these differ between computers' maths libraries, and the
        # player's setup tool checks the file byte for byte (tools/setup_core.py).
        t, r, s = ([round(v, 9) + 0.0 for v in x] for x in (t, r, s))
        node = dict(name=n["name"], translation=t, rotation=r, scale=s)
        if n["children"]:
            node["children"] = list(n["children"])
        j["nodes"].append(node)
    roots = [0]
    # Rest world matrices (to check the bind matrices).
    world = [None] * len(m.nodes)

    def walk(k, parent):
        world[k] = mat_mul(parent, m.nodes[k]["matrix"]) if parent else list(m.nodes[k]["matrix"])
        for c in m.nodes[k]["children"]:
            walk(c, world[k])
    walk(0, None)
    report = []
    # Skin: every joint any piece's palette names (each piece has its own palette of matrix objects,
    # each a joint's). A joint's stored bind matrix is the inverse of its bind pose relative to the
    # skeleton's parent node, and the mesh's vertices are in that same space: the stored matrix is
    # the glTF inverse bind matrix as it is (the skeleton's parent, with its scale and the root's
    # animation, then places the mesh).
    by_blend = {n["blend"]: k for k, n in enumerate(m.nodes) if n["joint"]}
    parent_of = {c: k for k, n in enumerate(m.nodes) for c in n["children"]}
    joints = []
    for _, _, pal, _ in m.geoms:
        for b in pal or []:
            if b not in by_blend:
                raise ValueError(f"{base}: a palette entry is not a joint's")
            if by_blend[b] not in joints:
                joints.append(by_blend[b])
    depth = {}
    for k in joints:
        d, p = 0, k
        while p in parent_of:
            p = parent_of[p]
            d += 1
        depth[k] = d
    ibms = [m.nodes[k]["bind"] for k in joints]
    if joints:
        top = min(joints, key=lambda k: depth[k])
        skel_parent = parent_of.get(top)
        report.append(f"{len(joints)} joints; skeleton parent {m.nodes[skel_parent]['name'] if skel_parent is not None else '-'}")
    # Textures.
    images, texture_of = [], {}
    j["images"], j["textures"], j["materials"], j["samplers"] = [], [], [], [dict(magFilter=9729, minFilter=9729)]
    for _, image, _, _ in m.geoms:
        if image is not None and image not in texture_of:
            w, h, rows, name = m.image_rows(image)
            j["images"].append(dict(name=name, mimeType="image/png", bufferView=glb.view(png_bytes(w, h, rows))))
            j["textures"].append(dict(source=len(j["images"]) - 1, sampler=0))
            j["materials"].append(dict(name=name, pbrMetallicRoughness=dict(baseColorTexture=dict(index=len(j["textures"]) - 1),
                                                                             metallicFactor=0.0)))
            texture_of[image] = len(j["materials"]) - 1
            if check:
                mt.write_png(str(Path(out_dir) / f"check_{name}"), w, h, rows)
    # A texture that repeats across a piece (UVs past 0-1; DotR draws one texture sheet, which
    # cannot repeat): the piece gets the image tiled that many times (scaled to at most twice its
    # size a side) and its UVs brought into 0-1.
    tiled_of = {}

    def tiled(image, tu, tv):
        if (image, tu, tv) not in tiled_of:
            w, h, rows, name = m.image_rows(image)
            big = [[rows[y % h][x % w] for x in range(w * tu)] for y in range(h * tv)]
            nw, nh = min(w * tu, 2 * w), min(h * tv, 2 * h)
            small = mt.resize(w * tu, h * tv, big, nw, nh)
            name = f"{Path(name).stem}_x{tu}x{tv}.png"
            j["images"].append(dict(name=name, mimeType="image/png", bufferView=glb.view(png_bytes(nw, nh, small))))
            j["textures"].append(dict(source=len(j["images"]) - 1, sampler=0))
            j["materials"].append(dict(name=name, pbrMetallicRoughness=dict(baseColorTexture=dict(index=len(j["textures"]) - 1),
                                                                             metallicFactor=0.0)))
            tiled_of[(image, tu, tv)] = len(j["materials"]) - 1
        return tiled_of[(image, tu, tv)]

    # One mesh: a primitive a geometry.
    prims = []
    for attr, image, pal, node in m.geoms:
        pos, nrm, uv, js, ws, tris = m.geometry(attr)
        material = texture_of.get(image)
        used = {i for t in tris for i in t}
        if image in texture_of and used:
            us, vs = [uv[i][0] for i in used], [uv[i][1] for i in used]
            if min(us) < -0.01 or max(us) > 1.01 or min(vs) < -0.01 or max(vs) > 1.01:
                ou, ov = math.floor(min(us) + 0.01), math.floor(min(vs) + 0.01)
                tu = max(1, min(8, math.ceil(max(us) - 0.01) - ou))
                tv = max(1, min(8, math.ceil(max(vs) - 0.01) - ov))
                material = tiled(image, tu, tv)
                uv = [((u - ou) / tu, (v - ov) / tv) for u, v in uv]
                report.append(f"geometry {attr}: texture repeated {tu} x {tv}")
        attrs = dict(POSITION=glb.accessor(pos, "VEC3", "f", 5126, minmax=True),
                     TEXCOORD_0=glb.accessor(uv, "VEC2", "f", 5126))
        if nrm:
            attrs["NORMAL"] = glb.accessor(nrm, "VEC3", "f", 5126)
        if js is not None:
            # The piece's palette numbers -> the skin's joint numbers.
            local = [joints.index(by_blend[b]) for b in (pal or [])]
            # Up to 4 weights a vertex; glTF takes 4 (the importer keeps the 3 largest).
            jj = [tuple(local[x] if x < len(local) else 0 for x in j4) for j4 in js]
            ww = []
            for w4 in ws:
                w = [max(0.0, x) for x in w4[:4]]
                total = sum(w) or 1.0
                ww.append(tuple(x / total for x in w))
            attrs["JOINTS_0"] = glb.accessor(jj, "VEC4", "B", 5121)
            attrs["WEIGHTS_0"] = glb.accessor(ww, "VEC4", "f", 5126)
        prim = dict(attributes=attrs, indices=glb.accessor([i for t in tris for i in t], "SCALAR", "I", 5125), mode=4)
        if material is not None:
            prim["material"] = material
        prims.append(prim)
        report.append(f"geometry {attr}: {len(pos)} vertices, {len(tris)} triangles")
    j["meshes"] = [dict(name=base, primitives=prims)]
    holder = len(j["nodes"])
    j["nodes"].append(dict(name=base + "_mesh", mesh=0, skin=0))
    j["skins"] = [dict(joints=joints, inverseBindMatrices=glb.accessor(ibms, "MAT4", "f", 5126), skeleton=0)]
    j["scenes"] = [dict(nodes=roots + [holder])]
    j["scene"] = 0
    # The loop.
    channels, samplers = [], []
    for k, n in enumerate(m.nodes):
        if n["seq"] < 0:
            continue
        times, tr, ro, sc = m.sequence(n["seq"])
        if not times:
            continue
        t_acc = glb.accessor(times, "SCALAR", "f", 5126, minmax=True)
        if check and ro:
            _, rest_q, _ = decompose(n["matrix"])
            report.append(f"{n['name']}: key 0 rotation vs rest {quat_dot(ro[0], rest_q):.4f} (as xyzw), "
                          f"{quat_dot((ro[0][1], ro[0][2], ro[0][3], ro[0][0]), rest_q):.4f} (as wxyz); "
                          f"translation key 0 {tuple(round(v, 3) for v in tr[0]) if tr else None} rest "
                          f"{tuple(round(v, 3) for v in n['matrix'][12:15])}")
        for path_, values, kind in (("translation", tr, "VEC3"), ("rotation", ro, "VEC4"), ("scale", sc, "VEC3")):
            if values:
                samplers.append(dict(input=t_acc, output=glb.accessor([tuple(v) for v in values], kind, "f", 5126),
                                     interpolation="LINEAR"))
                channels.append(dict(sampler=len(samplers) - 1, target=dict(node=k, path=path_)))
    if channels:
        j["animations"] = [dict(name=f"{base}_a", channels=channels, samplers=samplers)]
    out = Path(out_dir) / f"{base}.glb"
    glb.write(out)
    # The texture atlas cards.json's "model_texture" takes (its textures side by side, as
    # tools/glb_anim.py lays out the texture coordinates).
    import glb_anim
    if j["images"]:
        glb_anim.atlas(out, Path(out_dir) / f"{base}_atlas.png")
    return out, report


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("iso")
    p.add_argument("model")
    p.add_argument("out_dir")
    p.add_argument("--check", action="store_true", help="print the checks and write the textures as PNG too")
    a = p.parse_args()
    Path(a.out_dir).mkdir(parents=True, exist_ok=True)
    out, report = export(a.iso, a.model, a.out_dir, a.check)
    print("wrote", out)
    if a.check:
        print("\n".join(report))


if __name__ == "__main__":
    main()
