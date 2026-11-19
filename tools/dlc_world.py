#!/usr/bin/env python3
"""The second world's map (mods/widescreen/dlc/world.c): DATA/MODEL.MRG with recoloured copies of
the campaign map packs appended.

The campaign map is model pack type 3 of DATA/MODEL.MRG (SzModel_LoadMerge(3, n, ...): file entry
33 + n; n = 2 the map, 0 the map after the game is won, 4 the circle before a side is chosen). Each
pack holds the map's models (34 + k): six land pieces, each with its land texture, a translucent
pattern drawn over it and the cliff sides; two sea planes; then the podiums. The copies keep
everything but the colours: land and pattern green, sea blue, cliffs and podiums as they are.

They follow the original file in order (entry 33's copy, then entry 35's), so world.c finds them
from the game's own offset table: the copy of entry 33 at the file's old end, the next after it.

  dlc_world.py MODEL.MRG EXE OUT_DIR    writes the extended file and PNGs of the recoloured textures
"""

import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_research as mr  # noqa: E402
import model_texture as mt  # noqa: E402

OFFSETS = 0x292a50   # anOfs_MODEL: 82 file offsets, the last the file's size
ENTRIES = 82
MAP_ENTRIES = (33, 35)   # pack type 3, n = 0 and 2

GREEN = (0.30, 0.72, 0.24)
BLUE = (0.18, 0.42, 0.95)


def table(exe):
    from dotr.elf import file_offset, inventory
    meta = inventory(exe)
    return struct.unpack_from(f"<{ENTRIES}I", exe, file_offset(meta, OFFSETS, 4 * ENTRIES))


def _tint(colour, base, floor, gain):
    r, g, b = colour
    light = (0.30 * r + 0.59 * g + 0.11 * b) / 31.0
    k = floor + gain * light
    return tuple(max(0, min(31, int(round(31 * c * k)))) for c in base)


def recolour_palette(data, at, kind):
    """Rewrite the palette of the RH2 texture at `at`: "land" green, "sea" blue, "pattern" its
    yellows green (other colours kept)."""
    t = mt.info(data, at)
    for i in range(256):
        p, = struct.unpack_from("<H", data, t["palette"] + 2 * i)
        rgb = (p & 31, p >> 5 & 31, p >> 10 & 31)
        if kind == "land":
            new = _tint(rgb, GREEN, 0.55, 0.75)
        elif kind == "sea":
            new = _tint(rgb, BLUE, 0.50, 0.85)
        else:
            r, g, b = rgb
            if r >= 12 and g >= 12 and b < min(r, g) * 0.7:      # yellow and brown: green
                new = _tint(rgb, GREEN, 0.45, 0.70)
            else:
                continue
        struct.pack_into("<H", data, t["palette"] + 2 * i, (p & 0x8000) | new[0] | new[1] << 5 | new[2] << 10)


def recolour_pack(pack):
    """A copy of one map pack with its land, pattern and sea textures recoloured; with a list of
    (texture offset, kind) changed."""
    data = bytearray(pack)
    models = [a for a in range(0, len(data), 16) if data[a:a + 4] == b"RH0\0"] + [len(data)]
    textures = [a for a in range(0, len(data), 16) if data[a:a + 4] == b"RH2\0"]
    changed = []
    for k in range(len(models) - 1):
        a, b = models[k], models[k + 1]
        try:
            positions = mr.mesh(bytes(data[a:b]))[0]
        except Exception:
            continue
        if not positions:
            continue
        span = max(max(abs(p[0]), abs(p[2])) for p in positions)
        height = max(p[1] for p in positions) - min(p[1] for p in positions)
        own = [t for t in textures if a < t < b]
        if span < 5000 or not own:
            continue              # a podium or a building
        if height < 300:
            kinds = ["sea"] * len(own)
        else:
            kinds = ["land", "pattern"] + [None] * (len(own) - 2)   # the third: the cliffs
        for t, kind in zip(own, kinds):
            if kind and data[t + 0x50] == 0x13:
                recolour_palette(data, t, kind)
                changed.append((t, kind))
    return bytes(data), changed


def extend(model_mrg, exe):
    """DATA/MODEL.MRG with the recoloured copies of the map packs appended (in MAP_ENTRIES order)."""
    ofs = table(exe)
    if len(model_mrg) != ofs[-1]:
        raise ValueError(f"MODEL.MRG is {len(model_mrg):#x} bytes; the game's table ends at {ofs[-1]:#x}")
    out = bytearray(model_mrg)
    reports = []
    for e in MAP_ENTRIES:
        copy, changed = recolour_pack(model_mrg[ofs[e]:ofs[e + 1]])
        if len(copy) % 2048:
            raise ValueError("a map pack is not a whole number of sectors")
        reports.append(dict(entry=e, at=len(out), bytes=len(copy), textures=len(changed)))
        out += copy
    return bytes(out), reports


def main(argv=None):
    argv = argv or sys.argv[1:]
    model_mrg = Path(argv[0]).read_bytes()
    exe = Path(argv[1]).read_bytes()
    out = Path(argv[2])
    out.mkdir(parents=True, exist_ok=True)
    ofs = table(exe)
    for e in MAP_ENTRIES:
        copy, changed = recolour_pack(model_mrg[ofs[e]:ofs[e + 1]])
        print("entry", e, "recoloured", changed)
        for t, kind in changed:
            w, h, px = mt.decode(copy, t)
            mt.write_png(str(out / f"e{e}_{t:06x}_{kind}.png"), w, h, px)
    return 0


if __name__ == "__main__":
    sys.exit(main())
