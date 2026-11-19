#!/usr/bin/env python3
"""The title screen's EXPANDED logo (mods/<mod>/title/expanded.png) painted into the title picture.

The title is one 640 x 448 picture, the first record of DATA/TEXETC.MRG (lSzGame_DrawTitle_Pipe
draws it across the screen), with the game's logo and its trademark sign (at about 592, 297) in it.
The logo is laid over that sign, turned ANGLE degrees (rising to the right), WIDTH units across
before turning, centred on CENTRE. The picture keeps its own 256 colours: each pixel the logo
covers becomes the palette colour nearest to the logo blended over what was there, so the rest of
the picture is exactly as it was.

The logo file's white background (the white reached from its border) is taken out; the white
inside the letters stays.

  python tools/dlc_title.py PREVIEW.png      the title picture with the logo, to look at
"""

import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_texture as mt  # noqa: E402

CENTRE = (538, 327)     # on the 30-degree line through the trademark sign (592, 297)
WIDTH = 205
ANGLE = 30
BACKGROUND_TOLERANCE = 60


def logo(path):
    """The logo, its outer background clear, scaled to WIDTH and turned by ANGLE: (width, height,
    rows of (r, g, b, a)). Plain Python (the build runs without Pillow)."""
    import math
    from collections import deque
    w, h, rows = mt.read_png(path)
    src = [[tuple(p[:3]) for p in r] for r in rows]
    # The white reached from the border is background.
    near_white = lambda p: min(p) >= 255 - BACKGROUND_TOLERANCE
    clear = [[False] * w for _ in range(h)]
    todo = deque((x, y) for x in range(w) for y in (0, h - 1)) + deque((x, y) for y in range(h) for x in (0, w - 1))
    while todo:
        x, y = todo.popleft()
        if 0 <= x < w and 0 <= y < h and not clear[y][x] and near_white(src[y][x]):
            clear[y][x] = True
            todo.extend(((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)))

    def sample(fx, fy):
        """Bilinear (r, g, b, a) at source position (fx, fy), colours weighted by their cover."""
        x0, y0 = math.floor(fx), math.floor(fy)
        acc, alpha = [0.0, 0.0, 0.0], 0.0
        for dx, dy in ((0, 0), (1, 0), (0, 1), (1, 1)):
            x, y = x0 + dx, y0 + dy
            wgt = (1 - abs(fx - x)) * (1 - abs(fy - y))
            if wgt <= 0 or not (0 <= x < w and 0 <= y < h) or clear[y][x]:
                continue
            alpha += wgt
            for c in range(3):
                acc[c] += src[y][x][c] * wgt
        if alpha <= 0:
            return (0, 0, 0, 0)
        return tuple(round(v / alpha) for v in acc) + (round(255 * min(alpha, 1.0)),)

    scale = WIDTH / w                       # output units per source pixel
    sw, sh = w * scale, h * scale
    a = math.radians(ANGLE)
    ca, sa = math.cos(a), math.sin(a)
    ow = int(math.ceil(abs(sw * ca) + abs(sh * sa))) + 2
    oh = int(math.ceil(abs(sw * sa) + abs(sh * ca))) + 2
    out = []
    for oy in range(oh):
        line = []
        for ox in range(ow):
            # Turned counter-clockwise on screen (y down): undo it about the centres.
            dx, dy = ox - ow / 2, oy - oh / 2
            ux = dx * ca - dy * sa
            uy = dx * sa + dy * ca
            line.append(sample((ux + sw / 2) / scale - 0.5, (uy + sh / 2) / scale - 0.5))
        out.append(line)
    return ow, oh, out


def compose(texetc, logo_path):
    """TEXETC.MRG with the logo painted into its first record (the title picture)."""
    data = bytearray(texetc)
    t = mt.info(data, 0)
    slots = []
    for i in range(256):
        p, = struct.unpack_from("<H", data, t["palette"] + 2 * i)
        slots.append(((p & 31) * 255 // 31, (p >> 5 & 31) * 255 // 31, (p >> 10 & 31) * 255 // 31))
    colour = [slots[mt._place(v) % 256] for v in range(256)]   # the colour a pixel byte v shows
    where = {}
    table, size = mt._positions(t["width"], t["height"])
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        for i, xy in enumerate(row):
            if xy:
                where[tuple(xy)] = base + i
    lw, lh, px = logo(logo_path)
    x0, y0 = CENTRE[0] - lw // 2, CENTRE[1] - lh // 2
    nearest = {}
    for y in range(lh):
        for x in range(lw):
            r, g, b, a = px[y][x]
            at = where.get((x0 + x, y0 + y))
            if a == 0 or at is None:
                continue
            under = colour[data[at]]
            f = a / 255
            want = tuple(round(c * f + u * (1 - f)) for c, u in zip((r, g, b), under))
            key = tuple(c >> 3 for c in want)
            if key not in nearest:
                nearest[key] = min(range(256), key=lambda v: sum((p - q) ** 2 for p, q in zip(colour[v], want)))
            data[at] = nearest[key]
    return bytes(data)


def preview(out, logo_path, iso=None):
    import iso_files
    import project
    from PIL import Image
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup("/DATA/TEXETC.MRG")
        start = struct.unpack_from("<I", entry, 2)[0]
        data = disc.read(start, 0x50000 // 2048)
    finally:
        disc.close()
    t = mt.info(data, 0)
    w, h, rows = mt.decode(compose(data[:t["size"]], logo_path), 0)
    im = Image.new("RGB", (w, h))
    im.putdata([p for r in rows for p in r])
    im.save(out)


if __name__ == "__main__":
    from runtime import ROOT
    preview(sys.argv[1], ROOT / "mods" / "widescreen" / "title" / "expanded.png")
