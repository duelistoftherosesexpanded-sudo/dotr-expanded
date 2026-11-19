#!/usr/bin/env python3
"""Leader platforms (not built yet; kept for the campaign project): the first version of the card sleeves, which replaced the deck leader's rose platform. dlc/sleeves.c and tools/mod.py used it as written below; to use it again, give it its own DLC file and option.

The player's own face-down cards show their side's emblem, the red or the white rose shield: the
two 128 x 128 halves of TEXSYS.MRG texture 5 (256 x 128, 8 bits with a palette of its own). A sleeve
is a picture in mods/<mod>/sleeves/ (PNG, added with the launcher); it is cropped to a square and put
in both halves, with a palette made for it, so it keeps its colours.

The result is C: the byte ranges of the texture file that hold the palette and the pixels, and
for the game's own texture (ROSE, from the clean disc) and each sleeve the bytes of those ranges,
which the DLC copies into the loaded texture. Names are the file names, upper case.

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

TEXTURE = 5   # the sixth RH2 texture of TEXSYS.MRG: the two rose shields
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
    if (t["width"], t["height"]) != (256, 128):
        raise ValueError("TEXSYS.MRG: the shield texture is not where it was expected")
    return data[at:at + t["size"]]


def ranges(rh2):
    """(offset, length) of the palette and of each block of pixels (the packets' headers, which the
    game fills in when it uploads the texture, are left out)."""
    t = mt.info(rh2, 0)
    _, size = mt._positions(t["width"], t["height"])
    blocks = (t["width"] // min(t["width"], 128)) * (t["height"] // min(t["height"], 64))
    out = [(t["palette"], 512)]
    for k in range(blocks):
        out.append((t["image"] + k * (size + 0x60) + 0x60, size))
    return out


def square(path):
    """The picture cropped to its middle square and scaled to 128 x 128 (rows of (r, g, b))."""
    w, h, rows = mt.read_png(path)
    side = min(w, h)
    x0, y0 = (w - side) // 2, (h - side) // 2
    rows = [r[x0:x0 + side] for r in rows[y0:y0 + side]]
    return mt.resize(side, side, [[p[:3] for p in r] for r in rows], 128, 128)


def sleeve_name(path):
    name = NAME_CHARS.sub("", path.stem.upper().replace("_", " "))[:16].strip()
    return name or "SLEEVE"


def sleeves(mod):
    folder = Path(mod) / "sleeves"
    files = sorted(folder.glob("*.png")) if folder.is_dir() else []
    if len(files) > MAX_SLEEVES:
        raise ValueError(f"at most {MAX_SLEEVES} sleeves (mods/.../sleeves has {len(files)})")
    return files


def textures(mod, iso=None):
    """[(name, texture file bytes)]: the game's, then one per sleeve."""
    original = texture_file(iso)
    out = [("ROSE", original)]
    for path in sleeves(mod):
        data = bytearray(original)
        pic = square(path)
        mt.encode(data, 0, [row + row for row in pic])
        out.append((sleeve_name(path), bytes(data)))
    return out


def generate(mod, out_path, iso=None):
    made = textures(mod, iso)
    spans = ranges(made[0][1])
    lines = ["/* Written by tools/dlc_sleeves.py: the card sleeves (dlc/sleeves.c). */",
             f"const unsigned int dlc_sleeve_texture_size = {struct.unpack_from('<I', made[0][1], 8)[0]:#x};",
             f"const int dlc_sleeve_count = {len(made)};",
             f"const int dlc_sleeve_span_count = {len(spans)};",
             "const unsigned int dlc_sleeve_spans[][2] = {" + ", ".join(f"{{{a:#x}, {n:#x}}}" for a, n in spans) + "};",
             "const char *const dlc_sleeve_names[] = {" + ", ".join(f'"{n}"' for n, _ in made) + "};"]
    for k, (_, data) in enumerate(made):
        body = b"".join(data[a:a + n] for a, n in spans)
        lines.append(f"static const unsigned char sleeve_{k}[] = {{" + ",".join(str(b) for b in body) + "};")
    lines.append("const unsigned char *const dlc_sleeve_bytes[] = {" + ", ".join(f"sleeve_{k}" for k in range(len(made))) + "};")
    Path(out_path).write_text("\n".join(lines) + "\n", newline="\n")
    return [n for n, _ in made]


def preview(mod, out_png, iso=None):
    made = textures(mod, iso)
    rows = []
    for _, data in made:
        w, h, px = mt.decode(data, 0)
        rows += [r + [(255, 0, 255)] * 4 for r in px] + [[(255, 0, 255)] * (w + 4)] * 4
    mt.write_png(out_png, 260, len(rows), rows)


if __name__ == "__main__":
    root = Path(__file__).resolve().parent.parent
    preview(root / "mods" / "widescreen", sys.argv[1])

