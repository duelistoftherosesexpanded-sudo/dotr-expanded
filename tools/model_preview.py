#!/usr/bin/env python3
"""Draws a textured 3D model (FBX or OBJ, as tools/model_import.py reads them) into a PNG: card art
for new cards, and a quick look at a model before importing it.

  model_preview.py MODEL --texture TEX.png --out OUT.png [--size 256] [--yaw -25] [--pitch 12]
                   [--up +y] [--front +z] [--turn] [--arms-down 0] [--background light|dark|fire|...]

A software renderer in plain Python: an orthographic view turned by yaw and pitch, a depth buffer,
texture lookup per pixel and simple light from the upper left. The model fills the picture with a
small margin, over a soft gradient (an attribute's colours, or grey).
"""

import argparse
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_import as mi  # noqa: E402
import model_texture as mt  # noqa: E402

BACKGROUNDS = {   # inner and outer colours of the gradient, by attribute
    "light": ((250, 236, 170), (150, 110, 40)),
    "dark": ((120, 70, 150), (20, 10, 40)),
    "fire": ((255, 170, 90), (120, 20, 10)),
    "earth": ((200, 170, 120), (70, 50, 30)),
    "water": ((140, 200, 255), (20, 50, 120)),
    "wind": ((190, 255, 190), (30, 100, 60)),
    "grey": ((200, 200, 200), (70, 70, 70)),
}


def render(model, texture, size=256, yaw=-25.0, pitch=12.0, up="+y", front="+z", turn=False, arms_down=0.0,
           background="grey", flip_v=True, hide_uv=None):
    positions, coords, triangles = mi.load_mesh(model, arms_down, hide_uv)
    (us, ua), (fs, fa) = mi.axis(up), mi.axis(front)
    if turn:
        fs = -fs
    basis_up = [us if i == ua else 0.0 for i in range(3)]
    basis_front = [fs if i == fa else 0.0 for i in range(3)]
    basis_side = mi.cross(basis_up, basis_front)
    # Model axes: x side, y up, z towards the viewer.
    turned = [(sum(basis_side[i] * p[i] for i in range(3)), sum(basis_up[i] * p[i] for i in range(3)),
               sum(basis_front[i] * p[i] for i in range(3))) for p in positions]
    tex = mt.read_png(texture) if texture else (1, 1, [[(200, 200, 200)]])
    return draw(turned, coords, triangles, tex, size, yaw, pitch, background, flip_v)


def draw(positions, coords, triangles, texture, size=256, yaw=-25.0, pitch=12.0, background="grey", flip_v=True):
    """Draws a mesh given in model axes (x side, y up, z to the viewer) with `texture` as
    (width, height, rows); returns rows of (r, g, b)."""
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    cp, sp = math.cos(math.radians(pitch)), math.sin(math.radians(pitch))

    def view(p):
        # Turn about y (yaw), then about x (pitch).
        x, y, z = p
        x, z = cy * x + sy * z, -sy * x + cy * z
        y, z = cp * y - sp * z, sp * y + cp * z
        return x, y, z

    points = [view(p) for p in positions]
    lo = [min(p[i] for p in points) for i in range(2)]
    hi = [max(p[i] for p in points) for i in range(2)]
    scale = 0.9 * size / max(hi[0] - lo[0], hi[1] - lo[1], 1e-6)
    mx, my = (lo[0] + hi[0]) / 2, (lo[1] + hi[1]) / 2
    screen = [((p[0] - mx) * scale + size / 2, size / 2 - (p[1] - my) * scale, p[2]) for p in points]

    tw, th, tex = texture
    inner, outer = BACKGROUNDS.get(background, BACKGROUNDS["grey"])
    pixels = []
    for y in range(size):
        row = []
        for x in range(size):
            d = min(1.0, math.hypot(x - size / 2, y - size / 2) / (size * 0.7))
            row.append(tuple(int(inner[k] + (outer[k] - inner[k]) * d) for k in range(3)))
        pixels.append(row)
    depth = [[-1e30] * size for _ in range(size)]
    light = (-0.4, 0.6, 0.7)
    norm = math.sqrt(sum(v * v for v in light))
    light = tuple(v / norm for v in light)

    for a, b, c in triangles:
        p0, p1, p2 = screen[a[0]], screen[b[0]], screen[c[0]]
        area = (p1[0] - p0[0]) * (p2[1] - p0[1]) - (p1[1] - p0[1]) * (p2[0] - p0[0])
        if abs(area) < 1e-9:
            continue
        v0, v1, v2 = points[a[0]], points[b[0]], points[c[0]]
        n = mi.cross([v1[i] - v0[i] for i in range(3)], [v2[i] - v0[i] for i in range(3)])
        length = math.sqrt(sum(v * v for v in n)) or 1.0
        shade = 0.45 + 0.55 * abs(sum(n[i] * light[i] for i in range(3)) / length)
        uv = [coords[k[1]] if k[1] >= 0 else (0.0, 0.0) for k in (a, b, c)]
        x0 = max(0, int(min(p0[0], p1[0], p2[0])))
        x1 = min(size - 1, int(max(p0[0], p1[0], p2[0])) + 1)
        y0 = max(0, int(min(p0[1], p1[1], p2[1])))
        y1 = min(size - 1, int(max(p0[1], p1[1], p2[1])) + 1)
        for y in range(y0, y1 + 1):
            py = y + 0.5
            for x in range(x0, x1 + 1):
                px = x + 0.5
                w0 = ((p1[0] - px) * (p2[1] - py) - (p1[1] - py) * (p2[0] - px)) / area
                w1 = ((p2[0] - px) * (p0[1] - py) - (p2[1] - py) * (p0[0] - px)) / area
                w2 = 1.0 - w0 - w1
                if w0 < 0 or w1 < 0 or w2 < 0:
                    continue
                z = w0 * p0[2] + w1 * p1[2] + w2 * p2[2]
                if z <= depth[y][x]:
                    continue
                depth[y][x] = z
                u = w0 * uv[0][0] + w1 * uv[1][0] + w2 * uv[2][0]
                v = w0 * uv[0][1] + w1 * uv[1][1] + w2 * uv[2][1]
                tx = int((u % 1.0) * tw) % tw
                ty = int(((1.0 - v) if flip_v else v) % 1.0 * th) % th
                r, g, bl = tex[ty][tx]
                pixels[y][x] = (min(255, int(r * shade)), min(255, int(g * shade)), min(255, int(bl * shade)))
    return pixels


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("model")
    parser.add_argument("--texture")
    parser.add_argument("--out", required=True)
    parser.add_argument("--size", type=int, default=256)
    parser.add_argument("--yaw", type=float, default=-25.0)
    parser.add_argument("--pitch", type=float, default=12.0)
    parser.add_argument("--up", default="+y")
    parser.add_argument("--front", default="+z")
    parser.add_argument("--turn", action="store_true")
    parser.add_argument("--arms-down", type=float, default=0.0)
    parser.add_argument("--background", default="grey")
    args = parser.parse_args(argv)
    pixels = render(args.model, args.texture, args.size, args.yaw, args.pitch, args.up, args.front, args.turn,
                    args.arms_down, args.background)
    mt.write_png(args.out, args.size, args.size, pixels)
    return 0


if __name__ == "__main__":
    sys.exit(main())
