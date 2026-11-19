#!/usr/bin/env python3
"""Card sleeves for the DLC's SLEEVE option (dlc/sleeves.c, MOD OPTIONS): the card back.

A face-down card shows the brown swirl: card kind 7 of the card faces (data_0028ff30), the right
half of TEXSYS.MRG texture 2 (512 x 384, 8 bits, one palette shared with the pink trap frame on
its left) for the large face, and a 120 x 180 copy on texture 4 for the small one (sleeves.c
points the small one at the large swirl too, so there is one picture to change). A sleeve is a
picture in mods/<mod>/sleeves/ (added with the launcher), cropped to the card's shape (256 x 368).
It gets the palette entries only the swirl uses (and the unused ones), so the trap frame keeps its
colours: about 78 colours for the sleeve.

The result is C: the byte ranges of the texture file that hold the palette and the right half's
pixel blocks, and for the game's swirl (ORIGINAL, from the clean disc) and each sleeve the bytes of
those ranges, which the DLC copies into the loaded texture. Names are the file names, upper case.

  python3 tools/dlc_sleeves.py PREVIEW.png    the sleeves as the texture will hold them
"""

import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import iso_files  # noqa: E402
import model_texture as mt  # noqa: E402
import project  # noqa: E402

TEXTURE = 2                      # the third RH2 texture of TEXSYS.MRG
BACK = (256, 0, 512, 368)        # the swirl: data_0028ff30[1][7]
MAX_SLEEVES = 15
NAME_CHARS = re.compile(r"[^A-Z0-9 .<>-]")


def texture_file(iso=None):
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup("/DATA/TEXSYS.MRG")
        start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
        data = disc.read(start, (size + 2047) // 2048)[:size]
    finally:
        disc.close()
    at = -1
    for _ in range(TEXTURE + 1):
        at = data.find(b"RH2\0", at + 1)
    t = mt.info(data, at)
    if (t["width"], t["height"]) != (512, 384):
        raise ValueError("TEXSYS.MRG: the card back texture is not where it was expected")
    return data[at:at + t["size"]]


def layout(rh2):
    """{(x, y): byte offset}, the image blocks as (offset, length, set of (x, y)), the palette offset."""
    t = mt.info(rh2, 0)
    table, size = mt._positions(t["width"], t["height"])
    where, blocks = {}, []
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        cells = set()
        for i, xy in enumerate(row):
            if xy:
                where[xy] = base + i
                cells.add(xy)
        blocks.append((base, size, cells))
    return where, blocks, t["palette"]


def spans(rh2):
    """(offset, length) of the palette and of every pixel block that holds part of the back."""
    _, blocks, palette = layout(rh2)
    x0, y0, x1, y1 = BACK
    out = [(palette, 512)]
    for base, size, cells in blocks:
        if any(x0 <= x < x1 and y0 <= y < y1 for x, y in cells):
            out.append((base, size))
    return out


def sleeve_name(path):
    name = NAME_CHARS.sub("", path.stem.upper().replace("_", " "))[:16].strip()
    return name or "SLEEVE"


def sleeves(mod):
    folder = Path(mod) / "sleeves"
    files = sorted(folder.glob("*.png")) if folder.is_dir() else []
    if len(files) > MAX_SLEEVES:
        raise ValueError(f"at most {MAX_SLEEVES} sleeves (mods/.../sleeves has {len(files)})")
    return files


def card_shaped(path, width, height):
    """The picture cropped to the card's shape and scaled to it (rows of (r, g, b))."""
    w, h, rows = mt.read_png(path)
    if w * height > h * width:
        keep = h * width // height
        rows = [r[(w - keep) // 2:(w - keep) // 2 + keep] for r in rows]
        w = keep
    else:
        keep = w * height // width
        rows = rows[(h - keep) // 2:(h - keep) // 2 + keep]
        h = keep
    return mt.resize(w, h, [[p[:3] for p in r] for r in rows], width, height)


def with_sleeve(rh2, path):
    """The texture file with the sleeve in place of the swirl, in the swirl's own palette entries."""
    data = bytearray(rh2)
    where, _, palette = layout(rh2)
    x0, y0, x1, y1 = BACK
    index = {xy: mt._place(rh2[at]) for xy, at in where.items()}
    inside = {i for (x, y), i in index.items() if x0 <= x < x1 and y0 <= y < y1}
    outside = {i for (x, y), i in index.items() if not (x0 <= x < x1 and y0 <= y < y1)}
    free = sorted((inside - outside) | (set(range(256)) - inside - outside))
    pic = card_shaped(path, x1 - x0, y1 - y0)
    colours, indexed = mt.quantize(pic, len(free))
    # free holds palette positions (index above: a pixel byte b shows the colour at position
    # _place(b)), so colour k goes to position free[k] and its pixels get the byte _place(free[k]).
    for k, (r, g, b) in enumerate(colours[:len(free)]):
        struct.pack_into("<H", data, palette + 2 * free[k], 0x8000 | r >> 3 | (g >> 3) << 5 | (b >> 3) << 10)
    for y in range(y0, y1):
        for x in range(x0, x1):
            data[where[(x, y)]] = mt._place(free[indexed[y - y0][x - x0]])
    return bytes(data)


def textures(mod, iso=None):
    """[(name, texture file bytes)]: the game's swirl, then one per sleeve."""
    original = texture_file(iso)
    return [("ORIGINAL", original)] + [(sleeve_name(p), with_sleeve(original, p)) for p in sleeves(mod)]


def generate(mod, out_path, iso=None):
    made = textures(mod, iso)
    parts = spans(made[0][1])
    lines = ["/* Written by tools/dlc_sleeves.py: the card sleeves (dlc/sleeves.c). */",
             f"const unsigned int dlc_sleeve_texture_size = {struct.unpack_from('<I', made[0][1], 8)[0]:#x};",
             f"const int dlc_sleeve_count = {len(made)};",
             f"const int dlc_sleeve_span_count = {len(parts)};",
             "const unsigned int dlc_sleeve_spans[][2] = {" + ", ".join(f"{{{a:#x}, {n:#x}}}" for a, n in parts) + "};",
             "const char *const dlc_sleeve_names[] = {" + ", ".join(f'"{n}"' for n, _ in made) + "};"]
    for k, (_, data) in enumerate(made):
        body = b"".join(data[a:a + n] for a, n in parts)
        lines.append(f"static const unsigned char sleeve_{k}[] = {{" + ",".join(str(b) for b in body) + "};")
    lines.append("const unsigned char *const dlc_sleeve_bytes[] = {" + ", ".join(f"sleeve_{k}" for k in range(len(made))) + "};")
    Path(out_path).write_text("\n".join(lines) + "\n", newline="\n")
    return [n for n, _ in made]


def preview(mod, out_png, iso=None):
    x0, y0, x1, y1 = BACK
    tiles = []
    for _, data in textures(mod, iso):
        w, h, px = mt.decode(data, 0)
        tiles.append([row[x0:x1] for row in px[y0:y1]])
    rows = [sum((t[y] + [(255, 0, 255)] * 6 for t in tiles), []) for y in range(y1 - y0)]
    mt.write_png(out_png, len(rows[0]), len(rows), rows)


if __name__ == "__main__":
    root = Path(__file__).resolve().parent.parent
    preview(root / "mods" / "widescreen", sys.argv[1])
