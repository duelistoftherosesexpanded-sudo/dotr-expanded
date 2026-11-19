#!/usr/bin/env python3
"""Monster card frames in more colours (dlc/frames.c).

A monster's card face is drawn from TEXSYS.MRG texture 0 (large: the normal frame on its right half,
the effect frame on its left) or texture 4 (small: the normal frame at 0,0-120,180, the effect frame
at 120,0-240,180). A new colour keeps the frame's pixels and changes its palette only: each colour
here is a copy of the texture file cut down to its palette (its first transfer packet, the palette
data after it), with the frame's palette entries recoloured. frames.c uploads that palette before a
card of that colour is drawn and draws the card with a copy of the texture's header that names it.

Recolouring keeps each entry's shading: the hue becomes the colour's, the saturation and brightness
follow its rule (white keeps little colour and is lightened; black darkens the frame and keeps the
pale boxes light).

A card's frame (cards.json "frame", the Card Maker): automatic (the game's choice), normal, effect,
or one of COLOURS.

  frames_gen.c     the palettes (not in the live region; frames.c binds them when first used)
  framemap_gen.c   dlc_card_frame[1000]: 0 automatic, 1 normal, 2 effect, 3 + k colour k (live)

  python3 tools/dlc_frames.py PREVIEW.png    the large and small frames in every colour
"""

import colorsys
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_texture as mt  # noqa: E402

# name: (hue in degrees or None to keep, saturation factor, brightness rule[, brightness factor])
COLOURS = {
    "red": (356, 1.0, "keep"),
    # Ritual monsters: the game's own blue, the ritual card face (TEXSYS texture 1's right half):
    # hue 210, measured at 0.69 of the monster frame's saturation and 0.77 of its brightness.
    "ritual": (210, 0.69, "keep", 0.77),
    # Link monsters: a deeper royal blue (the link frame the user gave).
    "link": (218, 1.4, "keep", 0.8),
    "white": (None, 0.10, "light"),
    "purple": (282, 0.85, "keep"),
    "black": (None, 0.20, "dark"),
}
FRAMES = ["automatic", "normal", "effect"] + list(COLOURS)
# Older names still read from cards.json.
FRAME_ALIASES = {"blue": "link"}
LARGE, SMALL = 0, 4                     # TEXSYS.MRG textures
SMALL_REGIONS = [(0, 0, 120, 180), (120, 0, 240, 180)]   # data_0028ff30[0][0] and [0][6]
CUT = 0x770                             # header, transfer packets, palette (image data from 0x770)
CARD_LIMIT = 1532          # dlc_cards.h DLC_CARD_LIMIT


def textures(iso=None):
    """{texture number: bytes of the texture file} for the large and small card faces."""
    import iso_files
    import project
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup("/DATA/TEXSYS.MRG")
        start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
        data = disc.read(start, (size + 2047) // 2048)[:size]
    finally:
        disc.close()
    found, at = [], -1
    while True:
        at = data.find(b"RH2\0", at + 1)
        if at < 0:
            break
        found.append(at)
    result = {}
    for k in (LARGE, SMALL):
        t = mt.info(data, found[k])
        if (t["width"], t["height"]) != (512, 384) or t["palette"] - found[k] != 0x570 or t["image"] - found[k] != CUT:
            raise ValueError(f"TEXSYS.MRG texture {k} is not laid out as expected")
        result[k] = data[found[k]:found[k] + t["size"]]
    return result


def frame_entries(rh2, regions):
    """The palette positions the regions' pixels use, and those used elsewhere."""
    t = mt.info(rh2, 0)
    table, size = mt._positions(t["width"], t["height"])
    inside, outside = set(), set()
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        for i, xy in enumerate(row):
            if not xy:
                continue
            pos = mt._place(rh2[base + i])
            if regions is None or any(x0 <= xy[0] < x1 and y0 <= xy[1] < y1 for x0, y0, x1, y1 in regions):
                inside.add(pos)
            else:
                outside.add(pos)
    return inside, outside


def recolour(rgb, rule):
    hue, sat_factor, light = rule[:3]
    v_factor = rule[3] if len(rule) > 3 else 1.0
    h, s, v = colorsys.rgb_to_hsv(*(c / 255 for c in rgb))
    if hue is not None:
        h = hue / 360
    pale = s < 0.25
    s = min(1.0, s * sat_factor)
    v = min(1.0, v * v_factor)
    if light == "light":
        v = 0.55 + 0.45 * v
    elif light == "dark":
        v = v * (0.85 if pale else 0.32)
    return tuple(round(c * 255) for c in colorsys.hsv_to_rgb(h, s, v))


def palette_file(rh2, name, regions):
    """The texture cut to its palette (one transfer packet), the frame's entries recoloured."""
    data = bytearray(rh2[:CUT])
    struct.pack_into("<I", data, 0x14, 0x30)   # transfer bytes: the first (palette) packet only
    struct.pack_into("<I", data, 0x58, 1)      # one section
    # Every entry the frame uses: this palette is only ever used to draw these frames, so entries
    # other faces on the texture share with them do not matter.
    inside, _ = frame_entries(rh2, regions)
    rule = COLOURS[name]
    for pos in inside:
        v, = struct.unpack_from("<H", data, 0x570 + 2 * pos)
        rgb = ((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31)
        r, g, b = recolour(rgb, rule)
        struct.pack_into("<H", data, 0x570 + 2 * pos, (v & 0x8000) | r >> 3 | (g >> 3) << 5 | (b >> 3) << 10)
    return bytes(data)


def card_frames(mod):
    """{card number: frame index} from cards.json's "frame" fields."""
    path = Path(mod) / "cards.json"
    result = {}
    if not path.is_file():
        return result
    import dlc_cards
    for card in dlc_cards.read_entries(path):
        if "number" in card and card.get("frame"):
            name = str(card["frame"]).strip().lower()
            name = FRAME_ALIASES.get(name, name)
            if name not in FRAMES:
                raise ValueError(f"card {card['number']}: frame {card['frame']!r} is not one of {', '.join(FRAMES)}")
            result[int(card["number"])] = FRAMES.index(name)
    return result


def c_bytes(name, data):
    return (f"static const unsigned char {name}[{len(data)}] __attribute__((aligned(64))) = {{"
            + ",".join(str(b) for b in data) + "};")


def generate(mod, out_palettes, out_map, iso=None):
    tex = textures(iso)
    lines = ["/* Written by tools/dlc_frames.py: card frame palettes (dlc/frames.c). */",
             f"extern const int dlc_frame_colour_count;",
             f"extern const unsigned char *const dlc_frame_palettes[][2];"]
    names = []
    for k, name in enumerate(COLOURS):
        lines.append(c_bytes(f"large_{k}", palette_file(tex[LARGE], name, None)))
        lines.append(c_bytes(f"small_{k}", palette_file(tex[SMALL], name, SMALL_REGIONS)))
        names.append(f"{{large_{k}, small_{k}}}")
    lines.append(f"const int dlc_frame_colour_count = {len(COLOURS)};")
    lines.append("const unsigned char *const dlc_frame_palettes[][2] = {" + ", ".join(names) + "};")
    Path(out_palettes).write_text("\n".join(lines) + "\n", newline="\n")
    frames = card_frames(mod)
    table = [frames.get(n, 0) for n in range(CARD_LIMIT)]
    rows = [", ".join(map(str, table[a:a + 50])) for a in range(0, CARD_LIMIT, 50)]
    Path(out_map).write_text(
        "/* Written by tools/dlc_frames.py from cards.json: each card's frame (dlc/frames.c). */\n"
        f"extern const unsigned char dlc_card_frame[{CARD_LIMIT}];\n"
        f"const unsigned char dlc_card_frame[{CARD_LIMIT}] = {{\n    " + ",\n    ".join(rows) + "\n};\n", newline="\n")


def preview(path, iso=None):
    """Large and small monster frames (normal, effect) in the game's colours and every new one."""
    from PIL import Image
    tex = textures(iso)
    rows = []
    for name in [None] + list(COLOURS):
        images = []
        for k, regions, crops in ((LARGE, None, [(256, 0, 512, 368), (0, 0, 256, 368)]),
                                  (SMALL, SMALL_REGIONS, SMALL_REGIONS)):
            data = bytearray(tex[k])
            if name:
                data[:CUT] = palette_file(tex[k], name, regions)
                struct.pack_into("<I", data, 0x14, 0x4b0)
                struct.pack_into("<I", data, 0x58, 25)
            w, h, px = mt.decode(bytes(data), 0)
            im = Image.new("RGB", (w, h))
            im.putdata([p for r in px for p in r])
            for box in crops:
                c = im.crop(box)
                images.append(c.resize((128, 184)) if c.width > 128 else c.resize((122, 184)))
        rows.append(images)
    out = Image.new("RGB", (sum(i.width for i in rows[0]) + 30, 190 * len(rows)), (40, 40, 40))
    for y, images in enumerate(rows):
        x = 0
        for im in images:
            out.paste(im, (x, y * 190))
            x += im.width + 10
    out.save(path)


if __name__ == "__main__":
    preview(sys.argv[1])
