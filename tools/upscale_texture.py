#!/usr/bin/env python3
"""Upscale one of the game's system textures (DATA/TEXSYS.MRG, read from the clean disc) for
looking at, or for PCSX2's texture replacement.

  python tools/upscale_texture.py INDEX OUT_DIR [--scale 4]

INDEX is the texture's place in TEXSYS.MRG (0-7; 6 and 7 are the window textures). Writes to
OUT_DIR: texN_original.png (as it is), texN_nearest_Sx.png (plain enlargement) and
texN_scale4x.png (Scale2x applied twice: the pixel-art upscaler that rounds stair-stepped
diagonals but keeps hard edges and the original colours). Transparent pixels stay transparent.
Your own texture: keep the results out of the repository (launcher/ is ignored by git).
"""

import argparse
import struct
import sys
import zlib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import iso_files  # noqa: E402
import model_texture as mt  # noqa: E402
import project  # noqa: E402


def texture(index, iso=None):
    """(width, height, rows of (r, g, b, a)) of texture INDEX of TEXSYS.MRG."""
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup("/DATA/TEXSYS.MRG")
        start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
        data = disc.read(start, (size + 2047) // 2048)[:size]
    finally:
        disc.close()
    at = -1
    for _ in range(index + 1):
        at = data.find(b"RH2\0", at + 1)
    t = mt.info(data, at)
    w, h, rgb = mt.decode(data, at)
    # Alpha from the palette's top bit, as the game's colours have it.
    table, size = mt._positions(w, h)
    alpha = [[255] * w for _ in range(h)]
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        for i, where in enumerate(row):
            if where:
                p, = struct.unpack_from("<H", data, t["palette"] + 2 * mt._place(data[base + i]))
                if not p & 0x8000 and p & 0x7fff == 0:
                    alpha[where[1]][where[0]] = 0
    return w, h, [[rgb[y][x] + (alpha[y][x],) for x in range(w)] for y in range(h)]


def scale2x(w, h, rows):
    out = [[None] * (2 * w) for _ in range(2 * h)]
    for y in range(h):
        for x in range(w):
            p = rows[y][x]
            a = rows[y - 1][x] if y > 0 else p
            b = rows[y][x + 1] if x < w - 1 else p
            c = rows[y][x - 1] if x > 0 else p
            d = rows[y + 1][x] if y < h - 1 else p
            out[2 * y][2 * x] = a if c == a and c != d and a != b else p
            out[2 * y][2 * x + 1] = b if a == b and a != c and b != d else p
            out[2 * y + 1][2 * x] = c if d == c and d != b and c != a else p
            out[2 * y + 1][2 * x + 1] = d if b == d and b != a and d != c else p
    return 2 * w, 2 * h, out


def nearest(w, h, rows, s):
    return w * s, h * s, [[rows[y // s][x // s] for x in range(w * s)] for y in range(h * s)]


def write_rgba(path, w, h, rows):
    raw = b"".join(b"\0" + bytes(c for px in row for c in px) for row in rows)

    def chunk(kind, body):
        return struct.pack(">I", len(body)) + kind + body + struct.pack(">I", zlib.crc32(kind + body))
    Path(path).write_bytes(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
                          + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("index", type=int)
    parser.add_argument("out")
    parser.add_argument("--scale", type=int, default=4)
    args = parser.parse_args()
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    w, h, rows = texture(args.index)
    write_rgba(out / f"tex{args.index}_original.png", w, h, rows)
    write_rgba(out / f"tex{args.index}_nearest_{args.scale}x.png", *nearest(w, h, rows, args.scale))
    big = (w, h, rows)
    for _ in range({2: 1, 4: 2, 8: 3}[args.scale]):
        big = scale2x(*big)
    write_rgba(out / f"tex{args.index}_scale{args.scale}x.png", *big)
    print(out)


if __name__ == "__main__":
    main()
