#!/usr/bin/env python3
"""Put a picture in place of one of the game's system textures through PCSX2's texture replacement
(the launcher's PCSX2 loads launcher/textures/<serial>/replacements).

PCSX2 knows a texture by a name made from hashes of its data (and palette), which it shows only
by saving the textures the game uses ("Save textures" in the launcher writes them to
launcher/textures/<serial>/dumps while the game runs). So:

  python tools/texture_replace.py find INDEX           the saved textures that are TEXSYS texture
                                                        INDEX (compared by their pixels)
  python tools/texture_replace.py install INDEX PNG    copy PNG (any size, the same shape) into
                                                        replacements under each of those names

A texture drawn with several palettes or regions is saved under several names; install covers
them all (a region's name gets the matching part of the picture).
"""

import argparse
import re
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_texture as mt  # noqa: E402
import upscale_texture  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SERIAL = "SLUS-20515"
TEXTURES = ROOT / "launcher" / "textures" / SERIAL
# PCSX2's names: <TEX0 hash>[-<CLUT hash>][-r<w>x<h>]-<bits>.png (GSTextureReplacements.cpp)
NAME = re.compile(r"^([0-9a-f]{16})(?:-([0-9a-f]{16}))?(?:-r(\d+)x(\d+))?-([0-9a-f]+)\.png$", re.I)


def read_rgba(path):
    from PIL import Image
    im = Image.open(path).convert("RGBA")
    return im.width, im.height, im


def matches(index, tolerance=12):
    """[(dump path, (width, height) of the part it covers)] of texture INDEX among the dumps."""
    w, h, rows = upscale_texture.texture(index)
    found = []
    for path in sorted((TEXTURES / "dumps").glob("*.png")):
        m = NAME.match(path.name)
        if not m:
            continue
        dw, dh, im = read_rgba(path)
        if dw > w or dh > h:
            continue
        px = im.load()
        # Compare a grid of opaque pixels (PCSX2 widens the 5-bit colours its own way).
        same = total = 0
        for y in range(0, dh, 3):
            for x in range(0, dw, 3):
                r, g, b, a = rows[y][x]
                if a == 0:
                    continue
                total += 1
                pr, pg, pb, _ = px[x, y]
                if abs(pr - r) <= tolerance and abs(pg - g) <= tolerance and abs(pb - b) <= tolerance:
                    same += 1
        if total > 50 and same > 0.97 * total:
            found.append((path, (dw, dh)))
    return found


FONT_GLYPHS = 0x290b70   # data_00290b70: 37 glyphs per font (texture byte, u0, v0, u1, v1)
FONTS = 3


def font_glyphs():
    """The game's letter rectangles [(texture, u0, v0, u1, v1)] of its three window fonts."""
    from dotr.elf import file_offset, inventory
    import struct
    exe = (ROOT / "out/ntsc-u/SLUS_205.15").read_bytes()
    meta = inventory(exe)
    out = []
    for k in range(FONTS * 37):
        at = FONT_GLYPHS + 10 * k
        t = exe[file_offset(meta, at, 1)]
        out.append((t,) + struct.unpack_from("<4H", exe, file_offset(meta, at + 2, 8)))
    return out


def fix_glyph_edges(picture, texture, native=256):
    """SzDuel_DrawWindow_Font_Pipe draws a letter with texels u0 + 1/2 to u1 + 1/2 (and v the
    same): the right and bottom half texel come from the next letter or row of the font sheet,
    which shows at a high internal resolution (and PCSX2's sprite rounding can also reach half a
    texel to the left). In an upscaled picture the half texels on each side of a border between
    letters are made transparent, so whichever letter reaches into them draws nothing there."""
    from PIL import Image
    im = picture.copy()
    s = im.width // native
    half = s // 2
    src = picture.load()
    px = im.load()
    # Past the texture's right or bottom edge the read wraps round to the left or top (where, too,
    # nothing reads the first half texel).
    for t, u0, v0, u1, v1 in font_glyphs():
        if t != texture or u1 <= u0 or v1 <= v0:
            continue
        x0, y0, x1, y1 = u0 * s, v0 * s, u1 * s, v1 * s
        clear = (0, 0, 0, 0)
        for y in range(y0, y1 + half):
            for x in list(range(x0, x0 + half)) + list(range(x1, x1 + half)):
                px[x % im.width, y % im.height] = clear
        for x in range(x0, x1 + half):
            for y in list(range(y0, y0 + half)) + list(range(y1, y1 + half)):
                px[x % im.width, y % im.height] = clear
    return im


def install(index, picture):
    from PIL import Image
    w, h, _ = upscale_texture.texture(index)
    big = Image.open(picture).convert("RGBA")
    scale = big.width / w
    if abs(big.height / h - scale) > 1e-6:
        raise ValueError(f"{picture} is not the shape of the texture ({w} x {h})")
    out = TEXTURES / "replacements"
    out.mkdir(parents=True, exist_ok=True)
    found = matches(index)
    for path, (dw, dh) in found:
        part = big if (dw, dh) == (w, h) else big.crop((0, 0, round(dw * scale), round(dh * scale)))
        part.save(out / path.name)
        print(f"installed {out / path.name}")
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="command", required=True)
    f = sub.add_parser("find")
    f.add_argument("index", type=int)
    i = sub.add_parser("install")
    i.add_argument("index", type=int)
    i.add_argument("picture")
    args = parser.parse_args()
    if args.command == "find":
        found = matches(args.index)
        for path, size in found:
            print(path.name, size)
        if not found:
            print("none: open a screen that shows the texture with 'Save textures' on, then try again")
    else:
        if not install(args.index, args.picture):
            print("none installed: no saved texture matches yet")
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
