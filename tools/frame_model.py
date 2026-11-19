#!/usr/bin/env python3
"""A stand-in 3D model for a monster card without one: the card's art in a picture frame, art on
the front and on the back (turned so it reads the right way from behind), with motions of its
own for every DotR motion slot (clip letters as tools/model_skinned.py CLIP_SLOTS takes them):

  a summon       rises from below while it grows and turns once
  b idle         bobs and sways gently (loops)
  c attack       leans back, lunges forward, comes back
  d set / flip   turns once
  e damage       shudders and rocks back
  f wind-up      leans back
  g return       comes back from leaning

The model is one joint ("frame", under "frame_root") that carries every point, so the frame
moves as one piece. Its texture is one sheet, shaped like the texture the model gets in the game
(its donor's) and made at twice its size: the art over seven eighths of the width, the gilded
frame in the last eighth; the sheet is written next to the model (cards.json "model_texture").

  frame_model.py ART.png OUT_FOLDER [--name NAME]
"""
import argparse
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import bch_read as br  # noqa: E402  (euler_quat)
import model_texture as mt  # noqa: E402
from dod_export import Glb, png_bytes  # noqa: E402

FPS = 30.0
SHEET = 256                 # each half of the texture sheet
BORDER = 0.14               # frame width, the picture's width being 1
DEPTH = 0.10                # frame thickness
INSET = 0.025               # how far the picture sits inside the frame


def r6(v):
    return round(v, 6) + 0.0


def frame_texture(width, height):
    """The gilded frame: shading across its width (u, inner to outer edge) for a bevel."""
    rows = []
    for y in range(height):
        row = []
        for x in range(width):
            t = x / max(1, width - 1)
            bevel = 0.55 + 0.45 * math.sin(math.pi * t) ** 0.6       # bright ridge in the middle
            grain = 0.92 + 0.08 * math.sin(y * 0.35 * 256 / height + 3 * math.sin(t * 12.8))
            edge = 0.7 if t < 0.06 or t > 0.94 else 1.0
            k = bevel * grain * edge
            row.append((min(255, int(212 * k)), min(255, int(168 * k)), min(255, int(72 * k))))
        rows.append(row)
    return rows


def sheet(art_png, size):
    """(rows, width, height, art share of the width, art aspect): the art on the left, the frame
    strip (an eighth of the width) on the right. `size` is the texture the model gets in the game
    (its donor's, which the sheet is shrunk to): the sheet is made twice that, so the art keeps as
    many of the texture's pixels as it can."""
    w, h, rows = mt.read_png(str(art_png))
    width, height = 2 * size[0], 2 * size[1]
    strip = max(8, width // 8)
    art = mt.resize(w, h, [[p[:3] for p in r] for r in rows], width - strip, height)
    frame = frame_texture(strip, height)
    return [list(a) + list(f) for a, f in zip(art, frame)], width, height, (width - strip) / width, w / h


class Mesh:
    def __init__(self):
        self.pos, self.nrm, self.uv, self.idx = [], [], [], []

    def quad(self, corners, normal, uvs):
        """corners counter-clockwise seen from the side the normal points to."""
        base = len(self.pos)
        for c, t in zip(corners, uvs):
            self.pos.append([r6(v) for v in c])
            self.nrm.append(list(normal))
            self.uv.append([r6(t[0]), r6(t[1])])
        self.idx += [base, base + 1, base + 2, base, base + 2, base + 3]


def build_mesh(aspect, split=0.5):
    """The frame standing on y = 0, facing +z; the picture 1 wide. `split`: where the art ends
    and the frame strip starts across the texture sheet."""
    pw, ph = 1.0, 1.0 / aspect
    b, d, s = BORDER, DEPTH / 2, DEPTH / 2 - INSET
    x0, x1 = -pw / 2, pw / 2
    y0, y1 = b, b + ph                      # the picture's bottom and top
    X0, X1, Y0, Y1 = x0 - b, x1 + b, 0.0, y1 + b
    m = Mesh()
    A = lambda u, v: (u * split, v)                  # the art (v from the top)
    F = lambda u, v: (split + u * (1 - split), v)    # the frame strip: u across the frame, inner (0) to outer (1)
    # The picture, front and back (the back's art turned so it reads from behind).
    m.quad([(x0, y0, s), (x1, y0, s), (x1, y1, s), (x0, y1, s)], (0, 0, 1),
           [A(0, 1), A(1, 1), A(1, 0), A(0, 0)])
    m.quad([(x1, y0, -s), (x0, y0, -s), (x0, y1, -s), (x1, y1, -s)], (0, 0, -1),
           [A(0, 1), A(1, 1), A(1, 0), A(0, 0)])
    # The frame's faces, front and back: four bevelled strips each (trapezoids, inner edge u=0).
    inner = [(x0, y0), (x1, y0), (x1, y1), (x0, y1)]
    outer = [(X0, Y0), (X1, Y0), (X1, Y1), (X0, Y1)]
    for z, n in ((d, 1), (-d, -1)):
        for k in range(4):
            i0, i1 = inner[k], inner[(k + 1) % 4]
            o0, o1 = outer[k], outer[(k + 1) % 4]
            pts = [(o0[0], o0[1], z), (o1[0], o1[1], z), (i1[0], i1[1], z), (i0[0], i0[1], z)]
            uvs = [F(1, 0), F(1, 1), F(0, 1), F(0, 0)]
            if n < 0:
                pts, uvs = pts[::-1], uvs[::-1]
            m.quad(pts, (0, 0, n), uvs)
    # The outside edges and the inside walls down to the picture.
    for k in range(4):
        o0, o1 = outer[k], outer[(k + 1) % 4]
        ex, ey = o1[0] - o0[0], o1[1] - o0[1]
        ln = math.hypot(ex, ey)
        nx, ny = ey / ln, -ex / ln                       # pointing out
        m.quad([(o0[0], o0[1], -d), (o1[0], o1[1], -d), (o1[0], o1[1], d), (o0[0], o0[1], d)], (nx, ny, 0),
               [F(1, 0), F(1, 1), F(0.6, 1), F(0.6, 0)])
        i0, i1 = inner[k], inner[(k + 1) % 4]
        for z0, z1 in ((s, d), (-d, -s)):
            m.quad([(i1[0], i1[1], z0), (i0[0], i0[1], z0), (i0[0], i0[1], z1), (i1[0], i1[1], z1)], (-nx, -ny, 0),
                   [F(0, 1), F(0, 0), F(0.15, 0), F(0.15, 1)])
    return m, Y1


def motions(height):
    """{letter: (seconds, f(t) -> (translation, euler rotation, scale))}."""
    ease = lambda x: 0.5 - 0.5 * math.cos(math.pi * max(0.0, min(1.0, x)))
    deg = math.radians

    def summon(t):
        g = ease(t / 0.7)
        return ([0, -height * (1 - ease(t / 0.9)), 0], [0, 2 * math.pi * ease(t / 1.3), 0], [max(g, 0.01)] * 3)

    def idle(t):
        w = 2 * math.pi * t / 2.0
        return ([0, height * 0.03 * (1 + math.sin(w)), 0], [0, deg(8) * math.sin(w), deg(2) * math.sin(2 * w)], [1, 1, 1])

    def attack(t):
        if t < 0.4:
            k = ease(t / 0.4)
            return ([0, 0, -0.1 * height * k], [-deg(15) * k, 0, 0], [1, 1, 1])
        if t < 0.7:
            k = ease((t - 0.4) / 0.3)
            return ([0, 0, -0.1 * height + 0.9 * height * k], [-deg(15) + deg(35) * k, 0, 0], [1, 1, 1])
        k = ease((t - 0.7) / 0.7)
        return ([0, 0, 0.8 * height * (1 - k)], [deg(20) * (1 - k), 0, 0], [1, 1, 1])

    def flip(t):
        return ([0, 0, 0], [0, 2 * math.pi * ease(t / 1.0), 0], [1, 1, 1])

    def damage(t):
        k = max(0.0, 1 - t / 1.0)
        return ([0, 0, -0.2 * height * math.sin(math.pi * min(1, t / 1.0))],
                [-deg(10) * k, 0, deg(12) * k * math.sin(t * 40)], [1, 1, 1])

    def windup(t):
        k = ease(t / 0.6)
        return ([0, 0, -0.1 * height * k], [-deg(15) * k, 0, 0], [1, 1, 1])

    def back(t):
        k = 1 - ease(t / 0.6)
        return ([0, 0, -0.1 * height * k], [-deg(15) * k, 0, 0], [1, 1, 1])
    return {"a": (1.4, summon), "b": (2.0, idle), "c": (1.4, attack), "d": (1.0, flip), "e": (1.0, damage),
            "f": (0.6, windup), "g": (0.6, back)}


def make(art_png, out_dir, name="frame", size=(SHEET, SHEET // 2)):
    """size: the texture the model gets in the game (its donor's; tools/frame_cards.py)."""
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    rows, sw, sh, split, aspect = sheet(art_png, size)
    atlas = out_dir / f"{name}_atlas.png"
    mt.write_png(str(atlas), sw, sh, rows)
    mesh, height = build_mesh(aspect, split)
    glb = Glb()
    j = glb.json
    j["nodes"] = [dict(name="frame_root", children=[1]), dict(name="frame", translation=[0, 0, 0])]
    j["images"] = [dict(name=atlas.name, mimeType="image/png", bufferView=glb.view(png_bytes(sw, sh, rows)))]
    j["samplers"] = [dict(magFilter=9729, minFilter=9729)]
    j["textures"] = [dict(source=0, sampler=0)]
    j["materials"] = [dict(name="frame", pbrMetallicRoughness=dict(baseColorTexture=dict(index=0), metallicFactor=0.0))]
    n = len(mesh.pos)
    attrs = dict(POSITION=glb.accessor(mesh.pos, "VEC3", "f", 5126, minmax=True),
                 NORMAL=glb.accessor(mesh.nrm, "VEC3", "f", 5126),
                 TEXCOORD_0=glb.accessor(mesh.uv, "VEC2", "f", 5126),
                 JOINTS_0=glb.accessor([[1, 0, 0, 0]] * n, "VEC4", "B", 5121),
                 WEIGHTS_0=glb.accessor([[1.0, 0.0, 0.0, 0.0]] * n, "VEC4", "f", 5126))
    j["meshes"] = [dict(name=name, primitives=[dict(attributes=attrs, material=0, mode=4,
                                                    indices=glb.accessor(mesh.idx, "SCALAR", "I", 5125))])]
    identity = [1.0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1.0]
    j["skins"] = [dict(joints=[0, 1], inverseBindMatrices=glb.accessor([identity, identity], "MAT4", "f", 5126), skeleton=0)]
    j["nodes"].append(dict(name=name + "_mesh", mesh=0, skin=0))
    j["scenes"] = [dict(nodes=[0, 2])]
    j["scene"] = 0
    j["animations"] = []
    for letter, (seconds, f) in motions(height).items():
        frames = int(round(seconds * FPS)) + 1
        times = [r6(k / FPS) for k in range(frames)]
        tr, ro, sc, last = [], [], [], None
        for k in range(frames):
            t, r, s = f(k / FPS)
            q = list(br.euler_quat(r))
            if last and sum(a * b for a, b in zip(q, last)) < 0:
                q = [-v for v in q]
            last = q
            tr.append([r6(v) for v in t])
            ro.append([r6(v) for v in q])
            sc.append([r6(v) for v in s])
        t_acc = glb.accessor(times, "SCALAR", "f", 5126, minmax=True)
        samplers, channels = [], []
        for path_, values, kind in (("translation", tr, "VEC3"), ("rotation", ro, "VEC4"), ("scale", sc, "VEC3")):
            samplers.append(dict(input=t_acc, output=glb.accessor(values, kind, "f", 5126), interpolation="LINEAR"))
            channels.append(dict(sampler=len(samplers) - 1, target=dict(node=1, path=path_)))
        j["animations"].append(dict(name=f"{name}_{letter}", channels=channels, samplers=samplers))
    out = out_dir / f"{name}.glb"
    glb.write(out)
    return out, atlas


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("art")
    p.add_argument("out")
    p.add_argument("--name", default="frame")
    a = p.parse_args()
    print(*make(a.art, a.out, a.name))


if __name__ == "__main__":
    main()
