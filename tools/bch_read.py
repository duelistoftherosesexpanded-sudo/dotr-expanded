"""Reads Nintendo 3DS H3D models (.bch), as Yu-Gi-Oh! Saikyo Card Battle stores its monsters
(3D/Mo_*.bch): the model's meshes, skeleton, textures and skeletal animations.

The layout follows the H3D format as SPICA (gdkchan/SPICA) documents it, checked against these
files (version 0x22):
  header       sections contents / strings / commands / raw data / raw ext / relocation
  pointers     a pointer is an offset into the section its field points into: contents for
               structures, strings for names, commands for GPU command lists, raw data for the
               buffers a command names. (The relocation table also lists them, but marks some
               plain floats too, so it is used only for what a command's buffer is: 16- or
               8-bit indices.)
  contents     15 dictionaries (models, materials, shaders, textures, LUTs, lights, cameras,
               fogs, skeletal / material / visibility / light / camera / fog animations,
               scenes): (pointer table, count, name tree); the table holds pointers
Inside a model, materials and bones are arrays of records (44 and 100 bytes), meshes 56 bytes,
sub-meshes 52. Meshes and textures describe their buffers with PICA200 GPU register writes.
"""
import math
import struct
from pathlib import Path

DICTS = ["models", "materials", "shaders", "textures", "luts", "lights", "cameras", "fogs",
         "skel_anims", "mat_anims", "vis_anims", "light_anims", "cam_anims", "fog_anims", "scenes"]
# PICA200 registers
ATTR_FORMAT_LO, ATTR_FORMAT_HI, ATTR_OFFSET, ATTR_CONFIG1, ATTR_CONFIG2 = 0x201, 0x202, 0x203, 0x204, 0x205
INDEX_CONFIG, NUM_VERTICES, PRIMITIVE_CONFIG = 0x227, 0x228, 0x25E
FIXED_INDEX, FIXED_DATA = 0x232, 0x233
VSH_NUM_ATTR, VSH_PERM_LO, VSH_PERM_HI = 0x2B9, 0x2BB, 0x2BC
VSH_FLOAT_INDEX, VSH_FLOAT_DATA = 0x2C0, 0x2C1
TEX_DIM, TEX_ADDR = 0x82, 0x85
# Vertex attribute names (the permutation's numbers)
POSITION, NORMAL, TANGENT, COLOR, TEX0, TEX1, TEX2, BONE_INDEX, BONE_WEIGHT = range(9)
INDEX16_TARGETS = (7, 12)


class Bch:
    def __init__(self, path, data=None):
        """path: the .bch file, or a name when its bytes are given as data."""
        self.path = Path(path)
        self.d = data if data is not None else Path(path).read_bytes()
        d = self.d
        if d[:4] != b"BCH\0":
            raise ValueError(f"{path} is not a BCH file")
        self.version = d[4]
        names = ["contents", "strings", "commands", "raw"] + (["rawext"] if self.version > 0x20 else []) + ["reloc"]
        n = len(names)
        addr = struct.unpack_from(f"<{n}I", d, 8)
        lens = struct.unpack_from(f"<{n}I", d, 8 + 4 * n)
        self.sec = dict(zip(names, addr))
        self.seclen = dict(zip(names, lens))
        # The relocation entries of command words (which buffer kind a command's address is).
        self.command_targets = {}
        ra, rl = self.sec["reloc"], self.seclen["reloc"]
        for i in range(rl // 4):
            v = struct.unpack_from("<I", d, ra + 4 * i)[0]
            if v >> 29 == 2:
                self.command_targets[self.sec["commands"] + (v & 0x1FFFFFF) * 4] = (v >> 25) & 0xF

    # ---------------------------------------------------------------- primitives
    def u8(self, at): return self.d[at]
    def u16(self, at): return struct.unpack_from("<H", self.d, at)[0]
    def s16(self, at): return struct.unpack_from("<h", self.d, at)[0]
    def u32(self, at): return struct.unpack_from("<I", self.d, at)[0]
    def f32(self, at): return struct.unpack_from("<f", self.d, at)[0]
    def fs(self, at, n): return list(struct.unpack_from(f"<{n}f", self.d, at))

    def c(self, at):
        """A contents pointer field -> file offset."""
        return self.u32(at) + self.sec["contents"]

    def string(self, at):
        """A string pointer field -> the string."""
        o = self.sec["strings"] + self.u32(at)
        return self.d[o:self.d.index(b"\0", o)].decode("ascii", "replace")

    def dict(self, kind):
        """Offsets of a contents dictionary's entries."""
        at = self.sec["contents"] + 12 * DICTS.index(kind)
        table, count = self.c(at), self.u32(at + 4)
        return [self.c(table + 4 * i) for i in range(count)]

    def commands(self, at):
        """A command list field (pointer, word count) -> [(register, value, word offset)] in order."""
        start, count = self.u32(at) + self.sec["commands"], self.u32(at + 4)
        out, i = [], 0
        while i + 1 < count:
            param, header = self.u32(start + 4 * i), self.u32(start + 4 * i + 4)
            reg, extra, consecutive = header & 0xFFFF, (header >> 20) & 0xFF, header >> 31
            out.append((reg, param, start + 4 * i))
            for k in range(extra):
                out.append((reg + k + 1 if consecutive else reg, self.u32(start + 4 * (i + 2 + k)), start + 4 * (i + 2 + k)))
            i += 2 + extra
            i += i & 1
        return out

    # ---------------------------------------------------------------- model
    def model(self, at=None):
        at = at if at is not None else self.dict("models")[0]
        m = dict(name=None)
        materials = self.c(at + 0x34)
        m["materials"] = []
        for i in range(self.u32(at + 0x38)):
            r = materials + 44 * i
            m["materials"].append(dict(name=self.string(r + 40), texture=self.string(r + 28) if self.u32(r + 28) else None))
        bones = self.c(at + 0x70)
        m["bones"] = []
        for i in range(self.u32(at + 0x74)):
            r = bones + 100 * i
            m["bones"].append(dict(name=self.string(r + 0x5C), parent=self.s16(r + 4), scale=self.fs(r + 8, 3),
                                   rotation=self.fs(r + 0x14, 3), translation=self.fs(r + 0x20, 3)))
        meshes = self.c(at + 0x40)
        m["meshes"] = [self.mesh(meshes + 56 * i) for i in range(self.u32(at + 0x44))]
        return m

    def mesh(self, at):
        material, flags, node = self.u16(at), self.u8(at + 2), self.u16(at + 4)
        fmt = perm = cfg1 = 0
        stride = total = 0
        base = None
        uniforms, uidx, ubuf = {}, 0, []
        fixed, fidx, fbuf = {}, 0, []
        for reg, v, _ in self.commands(at + 8):
            if reg == ATTR_FORMAT_LO:
                fmt = (fmt & ~0xFFFFFFFF) | v
            elif reg == ATTR_FORMAT_HI:
                fmt = (fmt & 0xFFFFFFFF) | (v << 32)
            elif reg == ATTR_OFFSET:
                base = v + self.sec["raw"]
            elif reg == ATTR_CONFIG1:
                cfg1 = (cfg1 & ~0xFFFFFFFF) | v
            elif reg == ATTR_CONFIG2:
                cfg1 = (cfg1 & 0xFFFFFFFF) | ((v & 0xFFFF) << 32)
                stride = (v >> 16) & 0xFF
            elif reg == VSH_NUM_ATTR:
                total = (v & 0xF) + 1
            elif reg == VSH_PERM_LO:
                perm = (perm & ~0xFFFFFFFF) | v
            elif reg == VSH_PERM_HI:
                perm = (perm & 0xFFFFFFFF) | (v << 32)
            elif reg == VSH_FLOAT_INDEX:
                uidx, ubuf = v & 0xFF, []
            elif VSH_FLOAT_DATA <= reg <= VSH_FLOAT_DATA + 7:
                ubuf.append(v)
                if len(ubuf) == 4:            # w, z, y, x
                    uniforms[uidx] = [struct.unpack("<f", struct.pack("<I", x))[0] for x in reversed(ubuf)]
                    uidx, ubuf = uidx + 1, []
            elif reg == FIXED_INDEX:
                fidx, fbuf = v, []
            elif FIXED_DATA <= reg <= FIXED_DATA + 2:
                fbuf.append(v)
                if len(fbuf) == 3:
                    fixed[fidx] = f24_vector(fbuf)
        scales = uniforms.get(7, [1, 1, 1, 1]), uniforms.get(8, [1, 1, 1, 1])
        scale_of = {POSITION: scales[0][0], NORMAL: scales[0][1], TANGENT: scales[0][2], COLOR: scales[0][3],
                    TEX0: scales[1][0], TEX1: scales[1][1], TEX2: scales[1][2], BONE_WEIGHT: scales[1][3]}
        offset = uniforms.get(6, [0, 0, 0, 0])
        attrs, fixed_attrs, pos = [], {}, 0
        for i in range(total):
            if (fmt >> (48 + i)) & 1:
                name = (perm >> (i * 4)) & 0xF
                fixed_attrs[name] = fixed.get(i, [0, 0, 0, 0])
                continue
            p = (cfg1 >> (i * 4)) & 0xF
            name = (perm >> (p * 4)) & 0xF
            f = (fmt >> (p * 4)) & 0xF
            kind, count = f & 3, (f >> 2) + 1
            size = (1, 1, 2, 4)[kind]
            pos = (pos + size - 1) // size * size
            attrs.append(dict(name=name, kind=kind, count=count, offset=pos, scale=scale_of.get(name, 1.0)))
            pos += size * count
        subs = []
        sub_at, sub_n = self.c(at + 0x10), self.u32(at + 0x14)
        for s in range(sub_n):
            r = sub_at + 52 * s
            skinning, nb = self.u8(r), self.u16(r + 2)
            bone_list = [self.u16(r + 4 + 2 * k) for k in range(nb)]
            ibase = icount = None
            wide, mode = False, 0
            for reg, v, word in self.commands(r + 0x2C):
                if reg == INDEX_CONFIG:
                    ibase = (v & 0x7FFFFFFF) + self.sec["raw"]
                    wide = self.command_targets.get(word) in INDEX16_TARGETS or bool(v >> 31)
                elif reg == NUM_VERTICES:
                    icount = v
                elif reg == PRIMITIVE_CONFIG:
                    mode = v >> 8
            fmt_i = "<%dH" % icount if wide else "<%dB" % icount
            indices = list(struct.unpack_from(fmt_i, self.d, ibase))
            subs.append(dict(skinning=skinning, bones=bone_list, indices=indices, mode=mode))
        count = max(max(s["indices"]) for s in subs) + 1 if subs else 0
        verts = {a["name"]: [] for a in attrs}
        kinds = {0: "b", 1: "B", 2: "h", 3: "f"}
        for v in range(count):
            o = base + v * stride
            for a in attrs:
                vals = struct.unpack_from("<%d%s" % (a["count"], kinds[a["kind"]]), self.d, o + a["offset"])
                if a["name"] not in (BONE_INDEX,):
                    vals = [x * a["scale"] for x in vals]
                verts[a["name"]].append(list(vals))
        if POSITION in verts:
            verts[POSITION] = [[p[k] + offset[k] for k in range(3)] for p in verts[POSITION]]
        return dict(material=material, skinning=(flags >> 2) & 3, node=node, vertices=verts, fixed=fixed_attrs,
                    submeshes=subs, count=count)

    # ---------------------------------------------------------------- textures
    def textures(self):
        """{name: (width, height, rows of RGBA)}."""
        out = {}
        for at in self.dict("textures"):
            fmt = self.u8(at + 24)
            name = self.string(at + 28)
            w = h = addr = None
            for reg, v, _ in self.commands(at):
                if reg == TEX_DIM:
                    h, w = v & 0x7FF, (v >> 16) & 0x7FF
                elif reg == TEX_ADDR:
                    addr = v + self.sec["raw"]
            out[name] = (w, h, decode_texture(self.d, addr, w, h, fmt), fmt)
        return out

    # ---------------------------------------------------------------- animations
    def skeletal_animations(self):
        """[{name, frames, loop, bones: {bone name: {channel: curve}}}]; channel is 'sx'..'tz',
        a curve [(frame, value, in slope, out slope)] (one key: constant)."""
        anims = []
        for at in self.dict("skel_anims"):
            a = dict(name=self.string(at), loop=bool(self.u8(at + 4) & 1), frames=self.f32(at + 8), bones={})
            elements, n = self.c(at + 12), self.u32(at + 16)
            for k in range(n):
                e = self.c(elements + 4 * k)
                target, prim = self.u16(e + 4), self.u16(e + 6)
                if prim != 4:                  # Transform only (the monsters use nothing else)
                    continue
                flags = self.u32(e + 8)
                curves = {}
                for slot, ch in enumerate(("sx", "sy", "sz", "rx", "ry", "rz", "tx", "ty", "tz")):
                    if (flags >> (16 + slot)) & 1:
                        continue
                    if (flags >> (6 + slot + (1 if slot >= 6 else 0))) & 1:
                        curves[ch] = [(0.0, self.f32(e + 12 + 4 * slot), 0.0, 0.0)]
                    else:
                        curves[ch] = self.key_group(self.c(e + 12 + 4 * slot))
                a["bones"][self.string(e)] = curves
            anims.append(a)
        return anims

    def key_group(self, at):
        start, end = self.f32(at), self.f32(at + 4)
        interp, quant, count = self.u8(at + 12), self.u8(at + 13), self.u16(at + 14)
        vscale, voffset, fscale = self.f32(at + 16), self.f32(at + 20), self.f32(at + 24)
        p = self.c(at + 32)
        keys = []
        for _ in range(count):
            if quant == 0:      # Hermite128
                f, v, i, o = struct.unpack_from("<4f", self.d, p); p += 16
            elif quant == 1:    # Hermite64
                fv, i, o = struct.unpack_from("<Ihh", self.d, p); p += 8
                f, v, i, o = fv & 0xFFF, fv >> 12, i / 256, o / 256
            elif quant == 2:    # Hermite48
                f, v = self.u8(p), self.u16(p + 1)
                sl = self.d[p + 3] | self.d[p + 4] << 8 | self.d[p + 5] << 16
                i, o = sext(sl & 0xFFF, 12) / 32, sext(sl >> 12, 12) / 32
                p += 6
            elif quant == 3:    # UnifiedHermite96
                f, v, i = struct.unpack_from("<3f", self.d, p); o = i; p += 12
            elif quant == 4:    # UnifiedHermite48
                f, v, i = struct.unpack_from("<HHh", self.d, p); f, i = f / 32, i / 256; o = i; p += 6
            elif quant == 5:    # UnifiedHermite32
                f = self.u8(p)
                vs = self.d[p + 1] | self.d[p + 2] << 8 | self.d[p + 3] << 16
                v, i = vs & 0xFFF, sext(vs >> 12, 12) / 32; o = i; p += 4
            elif quant == 6:    # StepLinear64
                f, v = struct.unpack_from("<2f", self.d, p); i = o = 0.0; p += 8
            elif quant == 7:    # StepLinear32
                fv = self.u32(p); f, v, i, o = fv & 0xFFF, fv >> 12, 0.0, 0.0; p += 4
            else:
                raise ValueError(f"key quantization {quant}")
            keys.append((f * fscale, v * vscale + voffset, i, o))
        return dict(keys=keys, interp=interp, start=start, end=end)


def sext(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def f24(v):
    if v & 0x7FFFFF == 0:
        return 0.0
    sign = -1.0 if v & 0x800000 else 1.0
    exp = (v >> 16) & 0x7F
    mant = v & 0xFFFF
    return sign * (1 + mant / 65536.0) * 2.0 ** (exp - 63)


def f24_vector(words):
    v = (words[0] << 64) | (words[1] << 32) | words[2]
    return [f24((v >> s) & 0xFFFFFF) for s in (0, 24, 48, 72)]


def evaluate(curve, frame):
    """A curve's value at a frame (Hermite between keys, held outside them)."""
    if isinstance(curve, list):              # constant
        return curve[0][1]
    keys, interp = curve["keys"], curve["interp"]
    if not keys:
        return 0.0
    if frame <= keys[0][0]:
        return keys[0][1]
    if frame >= keys[-1][0]:
        return keys[-1][1]
    for k in range(len(keys) - 1):
        f0, v0, _, o0 = keys[k]
        f1, v1, i1, _ = keys[k + 1]
        if f0 <= frame <= f1:
            if interp == 0 or f1 == f0:
                return v0
            t = (frame - f0) / (f1 - f0)
            if interp == 1:
                return v0 + (v1 - v0) * t
            dt = f1 - f0
            t2, t3 = t * t, t * t * t
            return ((2 * t3 - 3 * t2 + 1) * v0 + (t3 - 2 * t2 + t) * o0 * dt
                    + (-2 * t3 + 3 * t2) * v1 + (t3 - t2) * i1 * dt)
    return keys[-1][1]


# ---------------------------------------------------------------- textures
ETC_MODIFIERS = [(2, 8), (5, 17), (9, 29), (13, 42), (18, 60), (24, 80), (33, 106), (47, 183)]
FORMAT_BITS = {0: 32, 1: 24, 2: 16, 3: 16, 4: 16, 5: 16, 6: 16, 7: 8, 8: 8, 9: 8, 10: 4, 11: 4, 12: 4, 13: 8}


def morton(x, y):
    v = 0
    for b in range(3):
        v |= ((x >> b) & 1) << (2 * b) | ((y >> b) & 1) << (2 * b + 1)
    return v


def etc1_block(word, alpha):
    """16 RGBA pixels [y][x] of a 3DS ETC1 block (64-bit little-endian word)."""
    diff, flip = (word >> 33) & 1, (word >> 32) & 1
    t1, t2 = (word >> 37) & 7, (word >> 34) & 7
    if diff:
        def c5(hi, lo):
            b = (word >> hi) & 0x1F
            d = sext((word >> lo) & 7, 3)
            return b, b + d
        r1, r2 = c5(59, 56)
        g1, g2 = c5(51, 48)
        b1, b2 = c5(43, 40)
        ex = lambda x: (x << 3) | (x >> 2)
        base = [(ex(r1), ex(g1), ex(b1)), (ex(r2 & 0x1F), ex(g2 & 0x1F), ex(b2 & 0x1F))]
    else:
        n = lambda s: ((word >> s) & 0xF) * 17
        base = [(n(60), n(52), n(44)), (n(56), n(48), n(40))]
    tables = [ETC_MODIFIERS[t1], ETC_MODIFIERS[t2]]
    px = [[None] * 4 for _ in range(4)]
    for x in range(4):
        for y in range(4):
            i = x * 4 + y
            idx = ((word >> (16 + i)) & 1) << 1 | ((word >> i) & 1)
            sub = (y >= 2) if flip else (x >= 2)
            a_, b_ = tables[sub]
            mod = (a_, b_, -a_, -b_)[idx]
            r, g, b = (max(0, min(255, c + mod)) for c in base[sub])
            a = 255 if alpha is None else ((alpha >> (i * 4)) & 0xF) * 17
            px[y][x] = (r, g, b, a)
    return px


def decode_texture(d, at, w, h, fmt):
    """Rows (top first) of RGBA tuples of a 3DS texture (8x8 tiles; stored bottom row first)."""
    img = [[(0, 0, 0, 255)] * w for _ in range(h)]
    p = at
    if fmt in (12, 13):
        for ty in range(0, h, 8):
            for tx in range(0, w, 8):
                for by, bx in ((0, 0), (0, 4), (4, 0), (4, 4)):
                    alpha = None
                    if fmt == 13:
                        alpha = struct.unpack_from("<Q", d, p)[0]; p += 8
                    word = struct.unpack_from("<Q", d, p)[0]; p += 8
                    blk = etc1_block(word, alpha)
                    for y in range(4):
                        for x in range(4):
                            img[ty + by + y][tx + bx + x] = blk[y][x]
    else:
        bits = FORMAT_BITS[fmt]
        for ty in range(0, h, 8):
            for tx in range(0, w, 8):
                for i in range(64):
                    x = y = 0
                    for b in range(3):
                        x |= ((i >> (2 * b)) & 1) << b
                        y |= ((i >> (2 * b + 1)) & 1) << b
                    if bits == 4:
                        v = (d[p + i // 2] >> (4 * (i & 1))) & 0xF
                    else:
                        o = p + i * bits // 8
                        v = int.from_bytes(d[o:o + bits // 8], "little")
                    img[ty + y][tx + x] = pixel(fmt, v)
                p += 64 * bits // 8
    return img[::-1]


def pixel(fmt, v):
    if fmt == 0:
        return (v >> 24 & 255, v >> 16 & 255, v >> 8 & 255, v & 255)
    if fmt == 1:
        return (v >> 16 & 255, v >> 8 & 255, v & 255, 255)
    if fmt == 2:
        e = lambda x: (x << 3) | (x >> 2)
        return (e(v >> 11 & 31), e(v >> 6 & 31), e(v >> 1 & 31), 255 if v & 1 else 0)
    if fmt == 3:
        return ((v >> 11 & 31) * 255 // 31, (v >> 5 & 63) * 255 // 63, (v & 31) * 255 // 31, 255)
    if fmt == 4:
        return ((v >> 12 & 15) * 17, (v >> 8 & 15) * 17, (v >> 4 & 15) * 17, (v & 15) * 17)
    if fmt == 5:
        return (v >> 8, v >> 8, v >> 8, v & 255)
    if fmt == 6:
        return (v >> 8, v & 255, 0, 255)
    if fmt == 7:
        return (v, v, v, 255)
    if fmt == 8:
        return (255, 255, 255, v)
    if fmt == 9:
        return ((v >> 4) * 17, (v >> 4) * 17, (v >> 4) * 17, (v & 15) * 17)
    if fmt == 10:
        return (v * 17, v * 17, v * 17, 255)
    if fmt == 11:
        return (255, 255, 255, v * 17)
    raise ValueError(f"texture format {fmt}")


# ---------------------------------------------------------------- transforms
def euler_matrix(r):
    """Rotation matrix (row-major 3x3) of Euler angles applied X, then Y, then Z."""
    cx, sx = math.cos(r[0]), math.sin(r[0])
    cy, sy = math.cos(r[1]), math.sin(r[1])
    cz, sz = math.cos(r[2]), math.sin(r[2])
    rx = [[1, 0, 0], [0, cx, -sx], [0, sx, cx]]
    ry = [[cy, 0, sy], [0, 1, 0], [-sy, 0, cy]]
    rz = [[cz, -sz, 0], [sz, cz, 0], [0, 0, 1]]
    mm = lambda a, b: [[sum(a[i][k] * b[k][j] for k in range(3)) for j in range(3)] for i in range(3)]
    return mm(rz, mm(ry, rx))


def euler_quat(r):
    """(x, y, z, w) of Euler angles applied X, then Y, then Z."""
    hx, hy, hz = r[0] / 2, r[1] / 2, r[2] / 2
    qx = (math.sin(hx), 0.0, 0.0, math.cos(hx))
    qy = (0.0, math.sin(hy), 0.0, math.cos(hy))
    qz = (0.0, 0.0, math.sin(hz), math.cos(hz))

    def mul(a, b):
        ax, ay, az, aw = a
        bx, by, bz, bw = b
        return (aw * bx + ax * bw + ay * bz - az * by, aw * by - ax * bz + ay * bw + az * bx,
                aw * bz + ax * by - ay * bx + az * bw, aw * bw - ax * bx - ay * by - az * bz)
    return mul(qz, mul(qy, qx))


def local_matrix(t, r, s):
    """4x4 row-major (column vectors) of translation, Euler rotation, scale."""
    m = euler_matrix(r)
    return [[m[0][0] * s[0], m[0][1] * s[1], m[0][2] * s[2], t[0]],
            [m[1][0] * s[0], m[1][1] * s[1], m[1][2] * s[2], t[1]],
            [m[2][0] * s[0], m[2][1] * s[1], m[2][2] * s[2], t[2]],
            [0, 0, 0, 1]]


def mat_mul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


def mat_inverse(m):
    """Inverse of an affine 4x4 (row-major)."""
    a = [row[:3] for row in m[:3]]
    det = (a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
           + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]))
    inv = [[(a[(j + 1) % 3][(i + 1) % 3] * a[(j + 2) % 3][(i + 2) % 3]
             - a[(j + 1) % 3][(i + 2) % 3] * a[(j + 2) % 3][(i + 1) % 3]) / det for j in range(3)] for i in range(3)]
    t = [m[0][3], m[1][3], m[2][3]]
    it = [-sum(inv[i][k] * t[k] for k in range(3)) for i in range(3)]
    return [inv[0] + [it[0]], inv[1] + [it[1]], inv[2] + [it[2]], [0, 0, 0, 1]]
