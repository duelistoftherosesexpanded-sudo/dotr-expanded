#!/usr/bin/env python3
"""Xbox button pictures for the DLC's BUTTONS option (dlc/buttons.c, MOD OPTIONS).

The game draws its button pictures from window texture 7, the last texture of DATA/TEXSYS.MRG
(256 x 256, 8 bits with one palette; the icon records at 0x290800 point into it):

  22 x 22 at y 176   TRIANGLE, SQUARE, CROSS, CIRCLE (x 0, 22, 44, 66)
  22 x 11 halves     R1 / L1 (x 0, y 198 / 209), R2 / L2 (x 22)
  48 x 16            SELECT (x 32, y 224)
  16 x 16            TRIANGLE (96, 224), SQUARE (112, 224), CROSS (96, 240), CIRCLE (112, 240)

This makes an Xbox version of those pixels from the clean disc (read only) and the letter shapes of
an icon sheet (mods/<mod>/art/xbox_buttons.png, Zacksly's "Xbox Series Button Icons", CC BY):
the round buttons keep the game's grey disc and get the letter in its Xbox colour by position
(CROSS -> A green, CIRCLE -> B red, SQUARE -> X blue, TRIANGLE -> Y yellow); the shoulder plates
read LB RB LT RT and SELECT reads VIEW. Only colours already in the texture's palette are used, so
the palette stays as it is. The result is C: the changed bytes of the texture file with their
PlayStation and Xbox values, which the DLC writes into the loaded texture.

  python3 tools/dlc_buttons.py PREVIEW.png    a picture of both versions, to check by eye
"""

import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import iso_files  # noqa: E402
import model_texture as mt  # noqa: E402
import project  # noqa: E402

TEXSYS = "/DATA/TEXSYS.MRG"
TEXTURE = 7   # the eighth RH2 texture of TEXSYS.MRG
# (x, y, size, PlayStation button)
ROUND = [(0, 176, 22, "TRIANGLE"), (22, 176, 22, "SQUARE"), (44, 176, 22, "CROSS"), (66, 176, 22, "CIRCLE"),
         (96, 224, 16, "TRIANGLE"), (112, 224, 16, "SQUARE"), (96, 240, 16, "CROSS"), (112, 240, 16, "CIRCLE")]
LETTER = {"CROSS": "A", "CIRCLE": "B", "SQUARE": "X", "TRIANGLE": "Y"}
COLOUR = {"A": (90, 200, 60), "B": (230, 50, 50), "X": (60, 120, 230), "Y": (250, 210, 40)}
# Where the letters are on the icon sheet: (left, top, right, bottom) around each round button.
SHEET = {"A": (55, 280, 170, 395), "B": (220, 280, 335, 395), "X": (385, 280, 500, 395), "Y": (557, 280, 672, 395)}
# (x, y, size, picture): START (the arrow) and SELECT (the bar) as round Xbox Menu and View buttons.
SMALL_ICONS = [(80, 240, 16, "MENU"), (80, 224, 16, "VIEW")]
ICON_COLOUR = (235, 235, 235)
# 10 x 10, strokes and gaps two dots wide, so they survive the game's 3/4-size drawing.
ICONS = {
    "MENU": ["########", "########", "........", "........", "########", "########", "........", "........",
             "########", "########"],
    "VIEW": ["######...", "######...", "##..##...", "##..######", "######..##", "...##...##", "...##...##",
             "...#######", "...#######"],
}
# The small (16 x 16) buttons' letters, drawn by hand: at that size the sheet's shapes blur.
SMALL_LETTERS = {
    "A": ["..####..", ".##..##.", "##....##", "##....##", "########", "########", "##....##", "##....##", "##....##"],
    "B": ["#######.", "##....##", "##....##", "##...##.", "######..", "##...##.", "##....##", "##....##",
          "#######."],
    "X": ["##....##", "##....##", ".##..##.", "..####..", "...##...", "..####..", ".##..##.", "##....##", "##....##"],
    "Y": ["##....##", "##....##", ".##..##.", "..####..", "...##...", "...##...", "...##...", "...##...", "...##..."],
}
# (x, y, w, h, text) plates whose words are replaced.
PLATES = [(0, 198, 22, 11, "RB"), (0, 209, 22, 11, "LB"), (22, 198, 22, 11, "RT"), (22, 209, 22, 11, "LT"),
          (32, 224, 48, 16, "VIEW")]
FONT = {  # 5 x 7, bit 4 the left column
    "B": [0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e], "E": [0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f],
    "I": [0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e], "L": [0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f],
    "R": [0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11], "T": [0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04],
    "V": [0x11, 0x11, 0x11, 0x11, 0x11, 0x0a, 0x04], "W": [0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0a],
}


def texture_file(iso=None):
    """The bytes of the texture (its RH2 file) from the clean disc."""
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup(TEXSYS)
        start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
        data = disc.read(start, (size + 2047) // 2048)[:size]
    finally:
        disc.close()
    at = -1
    for _ in range(TEXTURE + 1):
        at = data.find(b"RH2\0", at + 1)
    t = mt.info(data, at)
    if (t["width"], t["height"]) != (256, 256):
        raise ValueError("TEXSYS.MRG: the window texture is not where it was expected")
    return data[at:at + t["size"]]


def layout(rh2):
    """{(x, y): byte offset in the file} and the palette as (r, g, b, opaque)."""
    t = mt.info(rh2, 0)
    palette = []
    for i in range(256):
        p, = struct.unpack_from("<H", rh2, t["palette"] + 2 * i)
        palette.append(((p & 31) * 255 // 31, (p >> 5 & 31) * 255 // 31, (p >> 10 & 31) * 255 // 31, p >> 15))
    # A pixel byte b shows palette[_place(b)] (model_texture.decode); grids here hold _place(b).
    table, size = mt._positions(t["width"], t["height"])
    where = {}
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        for i, xy in enumerate(row):
            if xy:
                where[xy] = base + i
    return where, palette


def nearest(palette, rgb, used):
    """The palette index closest to rgb among the given indices."""
    return min(used, key=lambda i: sum((palette[i][c] - rgb[c]) ** 2 for c in range(3)))


def letter_mask(sheet, letter, size):
    """size x size booleans: the letter's shape from the icon sheet (the holes in its white disc)."""
    w, h, rows = sheet
    x0, y0, x1, y1 = SHEET[letter]
    white = [(x, y) for y in range(y0, y1) for x in range(x0, x1) if min(rows[y][x][:3]) > 200]
    cx = (min(x for x, _ in white) + max(x for x, _ in white)) / 2
    cy = (min(y for _, y in white) + max(y for _, y in white)) / 2
    r = (max(x for x, _ in white) - min(x for x, _ in white)) / 2
    inside = [(x, y) for y in range(y0, y1) for x in range(x0, x1)
              if (x - cx) ** 2 + (y - cy) ** 2 < (0.8 * r) ** 2 and min(rows[y][x][:3]) <= 200]
    lx0, lx1 = min(x for x, _ in inside), max(x for x, _ in inside) + 1
    ly0, ly1 = min(y for _, y in inside), max(y for _, y in inside) + 1
    shape = set(inside)
    span = max(lx1 - lx0, ly1 - ly0)
    ox, oy = lx0 - (span - (lx1 - lx0)) / 2, ly0 - (span - (ly1 - ly0)) / 2
    mask = []
    for j in range(size):
        line = []
        for i in range(size):
            sx0, sx1 = ox + span * i / size, ox + span * (i + 1) / size
            sy0, sy1 = oy + span * j / size, oy + span * (j + 1) / size
            pts = [(x, y) for y in range(int(sy0), int(sy1) + 1) for x in range(int(sx0), int(sx1) + 1)]
            line.append(sum(p in shape for p in pts) >= 0.42 * len(pts))
        mask.append(line)
    return mask


def text_pixels(text):
    """Bold 5 x 7 text as a set of (x, y), and its width."""
    dots, x = set(), 0
    for c in text:
        for r, bits in enumerate(FONT[c]):
            for col in range(5):
                if bits & (0x10 >> col):
                    dots.add((x + col, r))
                    dots.add((x + col + 1, r))
        x += 7
    return dots, x - 1


def xbox_indices(rh2, sheet):
    """(original, changed) index grids of the texture."""
    where, palette = layout(rh2)
    grid = [[mt._place(rh2[where[(x, y)]]) for x in range(256)] for y in range(256)]
    new = [row[:] for row in grid]
    opaque = sorted({grid[y][x] for y in range(256) for x in range(256) if palette[grid[y][x]][3]})

    def inner_radius(size):
        return size / 2 - (3.5 if size > 16 else 2.5)

    # The discs' dark grey: the commonest colour inside the rings of all four buttons of a size
    # (each symbol covers only a part of its own disc).
    fills = {}
    for size in {s for _, _, s, _ in ROUND}:
        c, inner = (size - 1) / 2, inner_radius(size)
        cells = [grid[y0 + j][x0 + i] for x0, y0, s, _ in ROUND if s == size
                 for j in range(size) for i in range(size) if ((i - c) ** 2 + (j - c) ** 2) ** 0.5 < inner]
        fills[size] = max(set(cells), key=cells.count)
    def plain(k):
        r, g, b = palette[k][:3]
        return max(r, g, b) - min(r, g, b) < 45

    # A clean disc for each size: at each pixel the colour the four buttons share there (the
    # symbols lie in different places), coloured symbol pixels left out.
    blank = {}
    for size in fills:
        c, inner = (size - 1) / 2, inner_radius(size)
        tiles = [(x0, y0) for x0, y0, s, _ in ROUND if s == size]
        disc = []
        for j in range(size):
            line = []
            for i in range(size):
                if ((i - c) ** 2 + (j - c) ** 2) ** 0.5 < inner:
                    line.append(fills[size])
                    continue
                seen = [grid[y0 + j][x0 + i] for x0, y0 in tiles]
                calm = [k for k in seen if plain(k)] or seen
                line.append(max(set(calm), key=calm.count))
            disc.append(line)
        blank[size] = disc
    for x0, y0, size, button in ROUND:
        letter = LETTER[button]
        for j in range(size):
            for i in range(size):
                new[y0 + j][x0 + i] = blank[size][j][i]
        # Bold: the shape one dot smaller, grown a dot right and down, so no stroke is one dot
        # thin (the game often draws these at 3/4 size, dropping every fourth row and column).
        if size > 16:
            n = 11
            thin = letter_mask(sheet, letter, n - 1)
            mask = [[any(0 <= j - dj < n - 1 and 0 <= i - di < n - 1 and thin[j - dj][i - di]
                         for dj in (0, 1) for di in (0, 1)) for i in range(n)] for j in range(n)]
        else:
            mask = [[dot == "#" for dot in line] for line in SMALL_LETTERS[letter]]
        colour = nearest(palette, COLOUR[letter], opaque)
        h, w = len(mask), max(len(line) for line in mask)
        ox, oy = (size - w) // 2, (size - h) // 2
        for j, line in enumerate(mask):
            for i, dot in enumerate(line):
                if dot:
                    new[y0 + oy + j][x0 + ox + i] = colour
    light = nearest(palette, ICON_COLOUR, opaque)
    for x0, y0, size, name in SMALL_ICONS:
        for j in range(size):
            for i in range(size):
                new[y0 + j][x0 + i] = blank[size][j][i]
        picture = ICONS[name]
        h, w = len(picture), max(len(r) for r in picture)
        ox, oy = x0 + (size - w) // 2, y0 + (size - h) // 2
        for j, line in enumerate(picture):
            for i, dot in enumerate(line):
                if dot == "#":
                    new[oy + j][ox + i] = light

    def plate_colour(plates):
        cells = [grid[y0 + j][x0 + i] for x0, y0, w, h, _ in plates for j in range(1, h - 1) for i in range(1, w - 1)]
        dark = min(set(cells), key=lambda k: sum(palette[k][:3]))
        # The commonest light colour (the letters and their shadows are dark).
        light = [k for k in cells if sum(palette[k][:3]) > 360] or [k for k in cells if k != dark]
        return dark, max(set(light), key=light.count)

    shoulders = plate_colour([p for p in PLATES if p[3] == 11])
    for x0, y0, w, h, text in PLATES:
        dark, plate = shoulders if h == 11 else plate_colour([(x0, y0, w, h, text)])
        for j in range(1, h - 1):
            for i in range(1, w - 1):
                new[y0 + j][x0 + i] = plate
        dots, width = text_pixels(text)
        tx, ty = x0 + (w - width) // 2, y0 + (h - 7) // 2
        for i, j in dots:
            new[ty + j][tx + i] = dark
    return grid, new, where, palette


def changes(iso=None, sheet_path=None):
    """[(byte offset, PlayStation byte, Xbox byte)] of the texture file."""
    rh2 = texture_file(iso)
    grid, new, where, _ = xbox_indices(rh2, mt.read_png(sheet_path))
    out = []
    for y in range(256):
        for x in range(256):
            if grid[y][x] != new[y][x]:
                out.append((where[(x, y)], mt._place(grid[y][x]), mt._place(new[y][x])))
    return sorted(out), struct.unpack_from("<I", rh2, 8)[0]


def generate(sheet_path, out_path, iso=None):
    rows, size = changes(iso, sheet_path)
    lines = ["/* Written by tools/dlc_buttons.py: the Xbox button pictures (dlc/buttons.c). */",
             f"const unsigned int dlc_button_texture_size = {size:#x};",
             f"const int dlc_button_count = {len(rows)};",
             "const unsigned int dlc_button_at[] = {" + ", ".join(f"{a:#x}" for a, _, _ in rows) + "};",
             "const unsigned char dlc_button_ps[] = {" + ", ".join(str(p) for _, p, _ in rows) + "};",
             "const unsigned char dlc_button_xbox[] = {" + ", ".join(str(x) for _, _, x in rows) + "};", ""]
    Path(out_path).write_text("\n".join(lines), newline="\n")
    return len(rows)


def preview(sheet_path, out_png, iso=None, scale=6):
    rh2 = texture_file(iso)
    grid, new, _, palette = xbox_indices(rh2, mt.read_png(sheet_path))
    x0, y0, x1, y1 = 0, 176, 128, 256
    rows = []
    for g in (grid, new):
        for y in range((y1 - y0) * scale):
            rows.append([palette[g[y0 + y // scale][x0 + x // scale]][:3] for x in range((x1 - x0) * scale)])
        rows += [[(255, 0, 255)] * ((x1 - x0) * scale)] * 4
    mt.write_png(out_png, (x1 - x0) * scale, len(rows), rows)


if __name__ == "__main__":
    root = Path(__file__).resolve().parent.parent
    preview(root / "mods/widescreen/art/xbox_buttons.png", sys.argv[1])
