#!/usr/bin/env python3
"""Reads a HAL "HSD" .dat model (Yu-Gi-Oh! The Falsebound Kingdom's MONSTER.MRG slots; the format
of Super Smash Bros. Melee) for tools/model_skinned.py, in the form tools/fbx_anim.load gives:
bones, skin, bind, mesh, clips.

The file: a 0x20-byte header (file size, data size, relocation count, root count, reference
count), the data, the relocation table, the roots ("scene_data" here), the string table; pointers
are offsets from the data's start, big-endian.

  scene_data -> model sets -> (JObjDesc tree, AnimJointDescs, MatAnim, ShapeAnim)
  JObjDesc (0x40): name, flags, child, next, DObj, rotation (radians, X Y Z), scale, translation,
                   inverse bind matrix (3x4) or null, RObj
  DObj: name, next, MObj (-> TObj -> image, palette), PObj chain
  PObj: name, next, vertex attribute list, flags (bits 12-13: 0 rigid, 2 envelope), display
        list size (32-byte blocks), display list, envelope list (envelope: (JObj, weight) pairs)
  Envelope vertices name their envelope with PNMTXIDX / 3 and go through the sum of weight *
  world * inverse bind of its joints; a joint with no inverse bind matrix counts as identity,
  except alone in an envelope, where the vertices are in that joint's space. On a nested skeleton
  root (a JObj with flag 2 inside the tree), envelope vertices also go through the inverse of
  that joint's world matrix (its envelopes' inverse binds include it).
  AnimJointDesc: child, next, AObjDesc (flags, end frame, FObjDesc chain), ...
  FObjDesc: next, data length, start frame, track (1-3 rotation X-Z, 5-7 translation, 8-10 scale),
            value format, slope format, data: packed (interpolation, count) then per key a value,
            a slope (spline), and the frames to the next key. Key values and slopes are
            little-endian in Falsebound Kingdom's files.

Motions run at 60 frames a second. Several textures are packed into one image (atlas()), each in
its own cell, a triangle's coordinates moved into its texture's cell (wrapped first).

  hsd_anim.py MODEL.dat                 prints joints, animations
  hsd_anim.py MODEL.dat --atlas OUT.png writes the packed texture
"""

import math
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

FPS = 60.0
CELL = 128          # atlas cell size


class Dat:
    def __init__(self, raw):
        size, data_size, nrel, nroot, nref = struct.unpack_from(">5I", raw, 0)
        self.data = raw[0x20:0x20 + data_size]
        rel_at = 0x20 + data_size
        self.relocs = set(struct.unpack_from(f">{nrel}I", raw, rel_at))
        root_at = rel_at + 4 * nrel
        str_at = root_at + 8 * (nroot + nref)
        self.roots = {}
        for i in range(nroot):
            off, so = struct.unpack_from(">2I", raw, root_at + 8 * i)
            self.roots[raw[str_at + so:raw.index(b"\0", str_at + so)].decode("latin1")] = off

    def u8(self, a): return self.data[a]
    def u16(self, a): return struct.unpack_from(">H", self.data, a)[0]
    def u32(self, a): return struct.unpack_from(">I", self.data, a)[0]
    def f32(self, a): return struct.unpack_from(">f", self.data, a)[0]
    def ptr(self, a): return self.u32(a) if a in self.relocs else None

    def ptr_array(self, a):
        out = []
        while a is not None:
            p = self.ptr(a)
            if p is None:
                break
            out.append(p)
            a += 4
        return out


# ---- matrices ----------------------------------------------------------------------------------
def identity():
    return [[float(i == j) for j in range(4)] for i in range(4)]


def mat_mul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


def local(rot, scale, trans):
    cx, sx = math.cos(rot[0]), math.sin(rot[0])
    cy, sy = math.cos(rot[1]), math.sin(rot[1])
    cz, sz = math.cos(rot[2]), math.sin(rot[2])
    r = [[cy * cz, sx * sy * cz - cx * sz, cx * sy * cz + sx * sz],
         [cy * sz, sx * sy * sz + cx * cz, cx * sy * sz - sx * cz],
         [-sy, sx * cy, cx * cy]]
    return [[r[i][0] * scale[0], r[i][1] * scale[1], r[i][2] * scale[2], trans[i]] for i in range(3)] + [[0, 0, 0, 1]]


def invert(m):
    a = [list(row) + [float(i == j) for j in range(4)] for i, row in enumerate(m)]
    for c in range(4):
        p = max(range(c, 4), key=lambda r: abs(a[r][c]))
        a[c], a[p] = a[p], a[c]
        dv = a[c][c]
        a[c] = [x / dv for x in a[c]]
        for r in range(4):
            if r != c:
                f = a[r][c]
                a[r] = [x - f * y for x, y in zip(a[r], a[c])]
    return [row[4:] for row in a]


def apply(m, p):
    return tuple(m[i][0] * p[0] + m[i][1] * p[1] + m[i][2] * p[2] + m[i][3] for i in range(3))


# ---- GX textures -------------------------------------------------------------------------------
def _rgb565(v):
    return ((v >> 11) & 31) * 255 // 31, ((v >> 5) & 63) * 255 // 63, (v & 31) * 255 // 31, 255


def _rgb5a3(v):
    if v & 0x8000:
        return ((v >> 10) & 31) * 255 // 31, ((v >> 5) & 31) * 255 // 31, (v & 31) * 255 // 31, 255
    return ((v >> 8) & 15) * 17, ((v >> 4) & 15) * 17, (v & 15) * 17, ((v >> 12) & 7) * 255 // 7


def _ia8(v):
    return (v & 255, v & 255, v & 255, v >> 8)


def decode_texture(data, w, h, fmt, tlut=None):
    """Rows of (r, g, b, a) for GX formats I4, I8, IA4, IA8, RGB565, RGB5A3, RGBA8, CI4, CI8, CMPR."""
    pix = [[(255, 0, 255, 255)] * w for _ in range(h)]
    if fmt == 14:   # CMPR: 8x8 tiles of four DXT1 blocks
        at = 0
        for ty in range(0, h, 8):
            for tx in range(0, w, 8):
                for sub in range(4):
                    bx, by = tx + (sub & 1) * 4, ty + (sub >> 1) * 4
                    c0, c1, bits = struct.unpack_from(">HHI", data, at)
                    at += 8
                    a, b = _rgb565(c0), _rgb565(c1)
                    if c0 > c1:
                        pal = [a, b, tuple((2 * x + y) // 3 for x, y in zip(a, b)),
                               tuple((x + 2 * y) // 3 for x, y in zip(a, b))]
                    else:
                        pal = [a, b, tuple((x + y) // 2 for x, y in zip(a, b)), (0, 0, 0, 0)]
                    for k in range(16):
                        px, py = bx + (k & 3), by + (k >> 2)
                        if px < w and py < h:
                            pix[py][px] = pal[(bits >> (30 - 2 * k)) & 3]
        return pix
    tiles = {0: (8, 8, 4), 1: (8, 4, 8), 2: (8, 4, 8), 3: (4, 4, 16), 4: (4, 4, 16), 5: (4, 4, 16),
             6: (4, 4, 32), 8: (8, 8, 4), 9: (8, 4, 8)}
    if fmt not in tiles:
        raise ValueError(f"texture format {fmt} not supported")
    tw, th, bpp = tiles[fmt]
    at = 0
    for ty in range(0, h, th):
        for tx in range(0, w, tw):
            if fmt == 6:   # RGBA8: AR then GB halves
                for k in range(16):
                    px, py = tx + (k & 3), ty + (k >> 2)
                    if px < w and py < h:
                        pix[py][px] = (data[at + 2 * k + 1], data[at + 32 + 2 * k], data[at + 33 + 2 * k],
                                       data[at + 2 * k])
                at += 64
                continue
            for k in range(tw * th):
                px, py = tx + k % tw, ty + k // tw
                if bpp == 4:
                    byte = data[at + k // 2]
                    v = byte >> 4 if k % 2 == 0 else byte & 15
                elif bpp == 8:
                    v = data[at + k]
                else:
                    v = struct.unpack_from(">H", data, at + 2 * k)[0]
                c = {0: lambda: (v * 17,) * 3 + (255,), 1: lambda: (v,) * 3 + (255,),
                     2: lambda: ((v & 15) * 17,) * 3 + ((v >> 4) * 17,), 3: lambda: _ia8(v),
                     4: lambda: _rgb565(v), 5: lambda: _rgb5a3(v)}.get(fmt)
                c = c() if c else (tlut[v] if tlut and v < len(tlut) else (255, 0, 255, 255))
                if px < w and py < h:
                    pix[py][px] = c
            at += tw * th * bpp // 8
    return pix


def _tlut(d, at):
    data_at, fmt, count = d.ptr(at), d.u32(at + 4), d.u16(at + 12)
    out = []
    for i in range(count):
        v = d.u16(data_at + 2 * i)
        out.append(_ia8(v) if fmt == 0 else _rgb565(v) if fmt == 1 else _rgb5a3(v))
    return out


# ---- animation tracks --------------------------------------------------------------------------
def _number(d, at, fmt):
    """A key value or slope. Falsebound Kingdom stores these little-endian (unlike the rest of
    the file)."""
    kind, frac = fmt >> 5, fmt & 31
    if kind == 0:
        return struct.unpack_from("<f", d.data, at)[0], at + 4
    size, code = {1: (2, "<h"), 2: (2, "<H"), 3: (1, "<b"), 4: (1, "<B")}[kind]
    return struct.unpack_from(code, d.data, at)[0] / (1 << frac), at + size


def _uleb(d, at):
    b = d.u8(at)
    v, shift = b & 0x7F, 7
    at += 1
    while b & 0x80:
        b = d.u8(at)
        v |= (b & 0x7F) << shift
        shift += 7
        at += 1
    return v, at


def fobj_keys(d, fobj):
    """[(frame, value, tangent, interpolation)] of one FObjDesc track."""
    length, start = d.u32(fobj + 4), d.f32(fobj + 8)
    vfmt, sfmt = d.u8(fobj + 0xD), d.u8(fobj + 0xE)
    at = d.ptr(fobj + 0x10)
    end = at + length
    keys, frame = [], start
    pending_slope = None
    while at < end:
        b = d.u8(at)
        op, count, shift = b & 0x0F, (b >> 4) & 7, 3
        at += 1
        while b & 0x80:
            b = d.u8(at)
            count |= (b & 0x7F) << shift
            shift += 7
            at += 1
        count += 1
        if op == 0:
            break
        for _ in range(count):
            if op in (1, 2, 3):            # CON, LIN, SPL0: value, wait
                value, at = _number(d, at, vfmt)
                wait, at = _uleb(d, at)
                keys.append([frame, value, 0.0 if pending_slope is None else pending_slope, op])
                pending_slope = None
                frame += wait
            elif op == 4:                   # SPL: value, slope, wait
                value, at = _number(d, at, vfmt)
                slope, at = _number(d, at, sfmt)
                wait, at = _uleb(d, at)
                keys.append([frame, value, slope, op])
                frame += wait
            elif op == 5:                   # SLP: a slope for the next key
                pending_slope, at = _number(d, at, sfmt)
            elif op == 6:                   # KEY: a value at the same frame
                value, at = _number(d, at, vfmt)
                keys.append([frame, value, 0.0, 2])
            else:
                return keys
    return keys


def sample(keys, f):
    if not keys:
        return None
    if f <= keys[0][0]:
        return keys[0][1]
    for a, b in zip(keys, keys[1:]):
        if a[0] <= f < b[0]:
            span = b[0] - a[0]
            t = (f - a[0]) / span if span else 0.0
            if a[3] == 1:
                return a[1]
            if a[3] == 2 or span == 0:
                return a[1] + (b[1] - a[1]) * t
            t2, t3 = t * t, t * t * t      # Hermite with slopes per frame
            return ((2 * t3 - 3 * t2 + 1) * a[1] + (t3 - 2 * t2 + t) * a[2] * span
                    + (-2 * t3 + 3 * t2) * b[1] + (t3 - t2) * b[2] * span)
    return keys[-1][1]


# ---- the model ---------------------------------------------------------------------------------
def _model_set(d):
    scene = d.roots.get("scene_data")
    if scene is None:
        raise ValueError("no scene_data root")
    return d.ptr_array(d.ptr(scene))[0]


def load(path, nested_roots=True, fit=None):
    """`nested_roots` False leaves out the nested skeleton root correction (see above). `fit`: a
    JSON file of tools/hsd_fit.py ({joint index: 4x4 bind-space matrix}) moving the vertices each
    joint drives most."""
    raw = Path(path).read_bytes()
    d = Dat(raw)
    mset = _model_set(d)
    root = d.ptr(mset)

    # Joints in walk order (parents first).
    bones, index_of, worlds = [], {}, {}

    def walk(j, parent):
        while j is not None:
            rot = [d.f32(j + 0x14 + 4 * i) for i in range(3)]
            sc = [d.f32(j + 0x20 + 4 * i) for i in range(3)]
            tr = [d.f32(j + 0x2C + 4 * i) for i in range(3)]
            worlds[j] = mat_mul(worlds[bones[parent]["id"]] if parent >= 0 else identity(), local(rot, sc, tr))
            index_of[j] = len(bones)
            bones.append(dict(id=j, name=f"joint{len(bones)}", parent=parent, t=tr,
                              r=[math.degrees(x) for x in rot], s=sc))
            me = index_of[j]
            child = d.ptr(j + 8)
            if child is not None:
                walk(child, me)
            j = d.ptr(j + 0xC)

    walk(root, -1)
    bind = {index_of[j]: worlds[j] for j in worlds}

    def inv_bind(j):
        at = d.ptr(j + 0x38)
        if at is None:
            return None
        v = [d.f32(at + 4 * i) for i in range(12)]
        return [v[0:4], v[4:8], v[8:12], [0.0, 0.0, 0.0, 1.0]]

    # Textures (by image), then geometry.
    textures, tex_index = [], {}

    def texture_of(mobj):
        tobj = d.ptr(mobj + 8) if mobj is not None else None
        img = d.ptr(tobj + 0x4C) if tobj is not None else None
        if img is None:
            return -1
        if img not in tex_index:
            w, h, fmt = d.u16(img + 4), d.u16(img + 6), d.u32(img + 8)
            tl = d.ptr(tobj + 0x50)
            try:
                pix = decode_texture(d.data[d.ptr(img):], w, h, fmt, _tlut(d, tl) if tl is not None else None)
            except (ValueError, struct.error, IndexError):
                pix = [[(128, 128, 128, 255)] * w for _ in range(h)]
            textures.append((w, h, pix, (d.f32(tobj + 0x1C) or 1.0, d.f32(tobj + 0x20) or 1.0)))
            tex_index[img] = len(textures) - 1
        return tex_index[img]

    positions, coords, triangles, skin = [], [], [], {}
    corner_tex = []      # texture of each coordinate (for the atlas)

    def add_vertex(pos, uv, ti, weights):
        positions.append(pos)
        v = len(positions) - 1
        for b, w in weights:
            skin.setdefault(b, []).append((v, w))
        if uv is None:
            coords.append((0.0, 0.0))
        else:
            su, sv = textures[ti][3] if ti >= 0 else (1.0, 1.0)
            coords.append((uv[0] * su, uv[1] * sv))
        corner_tex.append(ti)
        return v

    def read_value(desc, idx):
        attr, atype, cnt, ctype, frac, stride, dat = desc
        n = {9: (2 if cnt == 0 else 3), 13: (1 if cnt == 0 else 2)}.get(attr, 3)
        fmt = {0: "B", 1: "b", 2: "H", 3: "h", 4: "f"}[ctype]
        vals = struct.unpack_from(">" + fmt * n, d.data, dat + idx * stride)
        return vals if ctype == 4 else tuple(v / (1 << frac) for v in vals)

    def add_pobj(pobj, owner, ti):
        flags, nblocks = d.u16(pobj + 0xC), d.u16(pobj + 0xE)
        kind = (flags >> 12) & 3
        if kind == 1:
            return                          # shape animation: not supported
        vd, descs = d.ptr(pobj + 8), []
        while vd is not None and d.u32(vd) != 0xFF:
            descs.append((d.u32(vd), d.u32(vd + 4), d.u32(vd + 8), d.u32(vd + 0xC), d.u8(vd + 0x10),
                          d.u16(vd + 0x12), d.ptr(vd + 0x14) or 0))
            vd += 0x18
        envelopes = []
        if kind == 2:
            for env in d.ptr_array(d.ptr(pobj + 0x14)):
                items, at = [], env
                while d.ptr(at) is not None:
                    items.append((d.ptr(at), d.f32(at + 4)))
                    at += 8
                envelopes.append(items)
        # A DObj on a nested skeleton root (JObj flag 2, not the model's root): its envelopes'
        # inverse bind matrices include that joint's bind, so its inverse is applied after.
        nested_root = invert(worlds[owner]) if nested_roots and kind == 2 and d.u32(owner + 4) & 2 \
            and owner != root else None
        dl = d.ptr(pobj + 0x10)
        if dl is None:
            return
        at, end = dl, dl + nblocks * 32
        while at < end:
            cmd = d.u8(at)
            if cmd not in (0x80, 0x90, 0x98, 0xA0):
                break
            count = d.u16(at + 1)
            at += 3
            ids = []
            for _ in range(count):
                pos = uv = None
                env = None
                for desc in descs:
                    attr, atype = desc[0], desc[1]
                    if atype == 1:      # direct: matrix indices (one byte), colours
                        if attr == 0 and envelopes:
                            k = d.u8(at) // 3
                            env = envelopes[k] if k < len(envelopes) else None
                        at += 1 if attr <= 8 else 4 if attr in (11, 12) else 0
                        continue
                    if atype == 2:
                        idx = d.u8(at)
                        at += 1
                    elif atype == 3:
                        idx = d.u16(at)
                        at += 2
                    else:
                        continue
                    if attr == 9:
                        pos = read_value(desc, idx)
                    elif attr == 13:
                        uv = read_value(desc, idx)
                pos = tuple(pos or (0.0, 0.0, 0.0))
                if env:
                    if len(env) == 1 and inv_bind(env[0][0]) is None:
                        # a one-joint envelope on a joint with no inverse bind matrix: joint space
                        pos = apply(worlds[env[0][0]], pos)
                    else:
                        m = [[0.0] * 4 for _ in range(4)]
                        for j, w in env:
                            ib = inv_bind(j)
                            jm = mat_mul(worlds[j], ib) if ib else identity()
                            for r in range(4):
                                for c in range(4):
                                    m[r][c] += w * jm[r][c]
                        if nested_root:
                            m = mat_mul(m, nested_root)
                        pos = apply(m, pos)
                    weights = [(index_of[j], w) for j, w in env if j in index_of and w > 0]
                else:
                    pos = apply(worlds[owner], pos)
                    weights = [(index_of[owner], 1.0)]
                ids.append(add_vertex(pos, uv, ti, weights))
            corners = []
            if cmd == 0x90:
                corners = [ids[k:k + 3] for k in range(0, len(ids) - 2, 3)]
            elif cmd == 0x98:
                corners = [(ids[k], ids[k + 1], ids[k + 2]) if k % 2 == 0 else (ids[k + 1], ids[k], ids[k + 2])
                           for k in range(len(ids) - 2)]
            elif cmd == 0xA0:
                corners = [(ids[0], ids[k], ids[k + 1]) for k in range(1, len(ids) - 1)]
            elif cmd == 0x80:
                for k in range(0, len(ids) - 3, 4):
                    corners += [(ids[k], ids[k + 1], ids[k + 2]), (ids[k], ids[k + 2], ids[k + 3])]
            for a, b, c in corners:
                triangles.append(((a, a), (b, b), (c, c)))

    for j in list(worlds):
        if d.u32(j + 4) & (1 << 4):          # HIDDEN
            continue
        if d.u32(j + 4) & (1 << 12):         # INSTANCE: the pointer is not a DObj
            continue
        dobj = d.ptr(j + 0x10)
        while dobj is not None:
            ti = texture_of(d.ptr(dobj + 8))
            pobj = d.ptr(dobj + 0xC)
            while pobj is not None:
                add_pobj(pobj, j, ti)
                pobj = d.ptr(pobj + 4)
            dobj = d.ptr(dobj + 4)

    if fit:
        import json
        moves = {int(k): v for k, v in json.loads(Path(fit).read_text()).items()}
        owner = {}
        for b, pairs in skin.items():
            for v, w in pairs:
                if v not in owner or w > owner[v][1]:
                    owner[v] = (b, w)
        for v, (b, _) in owner.items():
            if b in moves:
                positions[v] = apply(moves[b], positions[v])

    # Atlas cells: each triangle's coordinates wrapped into 0..1, then into its texture's cell,
    # v turned up (HSD's v runs down the image).
    n = max(1, len(textures))
    cols = math.ceil(math.sqrt(n))
    rows = math.ceil(n / cols)
    new_coords = list(coords)
    for tri in triangles:
        cs = [c[1] for c in tri]
        ti = corner_tex[cs[0]]
        fu = math.floor(min(coords[c][0] for c in cs))
        fv = math.floor(min(coords[c][1] for c in cs))
        col, row = (ti % cols, ti // cols) if ti >= 0 else (0, 0)
        for c in cs:
            u = min(1.0, max(0.0, coords[c][0] - fu))
            v = min(1.0, max(0.0, coords[c][1] - fv))
            u = (col + 0.01 + u * 0.98) / cols
            v = (row + 0.01 + v * 0.98) / rows
            new_coords[c] = (u, 1.0 - v)
    coords = new_coords

    # Clips: each AnimJointDesc tree beside the joint tree.
    clips = []
    for number, anim in enumerate(d.ptr_array(d.ptr(mset + 4))):
        tracks, end = {}, 0.0

        def walk_anim(a, j):
            nonlocal end
            while a is not None and j is not None:
                aobj = d.ptr(a + 8)
                if aobj is not None:
                    end = max(end, d.f32(aobj + 4))
                    fobj = d.ptr(aobj + 8)
                    b = index_of[j]
                    while fobj is not None:
                        track = d.u8(fobj + 0xC)
                        keys = fobj_keys(d, fobj)
                        if 1 <= track <= 3 or 5 <= track <= 10:
                            channel, axis = ("R", track - 1) if track <= 3 else \
                                ("T", track - 5) if track <= 7 else ("S", track - 8)
                            tracks.setdefault(b, {}).setdefault(channel, [None, None, None])
                            tracks[b][channel][axis] = keys
                        fobj = d.ptr(fobj)
                walk_anim(d.ptr(a), d.ptr(j + 8))
                a, j = d.ptr(a + 4), d.ptr(j + 0xC)

        walk_anim(anim, root)
        frames = max(1, int(math.ceil(end)))
        out = {}
        for b, channels in tracks.items():
            out[b] = {}
            for ch, axes in channels.items():
                curves = []
                for keys in axes:
                    if not keys:
                        curves.append(None)
                        continue
                    times = [f / FPS for f in range(frames + 1)]
                    vals = [sample(keys, f) for f in range(frames + 1)]
                    if ch == "R":
                        vals = [math.degrees(v) for v in vals]
                    curves.append((times, vals))
                out[b][ch] = curves
        clips.append(dict(name=f"anim{number}", seconds=frames / FPS, tracks=out))

    model = dict(bones=bones, skin=skin, bind=bind, mesh=(positions, coords, triangles), clips=clips)
    model["textures"] = textures
    model["atlas_grid"] = (cols, rows)
    return model


def atlas(path, out, model=None):
    import model_texture as mt
    model = model or load(path)
    cols, rows = model["atlas_grid"]
    sheet = [[(128, 128, 128)] * (CELL * cols) for _ in range(CELL * rows)]
    for k, (w, h, pix, _) in enumerate(model["textures"]):
        r0, c0 = (k // cols) * CELL, (k % cols) * CELL
        for y in range(CELL):
            src = pix[min(h - 1, y * h // CELL)]
            for x in range(CELL):
                sheet[r0 + y][c0 + x] = src[min(w - 1, x * w // CELL)][:3]
    mt.write_png(str(out), CELL * cols, CELL * rows, sheet)
    return out


def main(argv=None):
    argv = argv or sys.argv[1:]
    model = load(argv[0])
    if "--atlas" in argv:
        print("wrote", atlas(argv[0], argv[argv.index("--atlas") + 1], model))
    print(len(model["bones"]), "joints,", len(model["skin"]), "skinned,", len(model["mesh"][2]), "triangles,",
          len(model["textures"]), "textures")
    for c in model["clips"]:
        print(f"clip {c['name']}: {c['seconds']:.2f} s, {len(c['tracks'])} joints animated")
    return 0


if __name__ == "__main__":
    sys.exit(main())
