#!/usr/bin/env python3
"""The parts of DLC.BIN made from the Duelists of the Roses disc, so a release can leave them out.

Four generators of tools/mod.py read the clean disc as they write C for the DLC: the Xbox button
pictures (tools/dlc_buttons.py: the window texture's bytes), the card frame colours
(tools/dlc_frames.py: the card face textures' headers and palettes), the card sleeves
(tools/dlc_sleeves.py: the card back texture's palette and pixels) and the card shop's pictures
(tools/dlc_shop.py: the picture texture's header and palette, the trophy and "?" icons). Their
arrays here are the ones that hold such bytes; the rest of what they write is the mod's own.

  arrays(mod, iso)   {"<generator>:<array>": bytes}, as the build compiles them into DLC.BIN

tools/release.py finds each array in the build's DLC.BIN, zeroes it and records where it was
(release.json "regen"); the player's setup tool (setup_core.build_disc) makes the arrays again
from the player's own disc and writes them back, checking each against the recorded hash. The
inputs that are the mod's own (sleeves/, shop/, art/xbox_buttons.png, cards.json) ship with the
release.
"""
import re
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

# The arrays, by generator, that hold bytes of the disc (or are made from them).
WANTED = {
    "buttons": re.compile(r"dlc_button_(ps|xbox)"),
    "frames": re.compile(r"(large|small)_\d+"),
    "sleeves": re.compile(r"sleeve_\d+"),
    "shop": re.compile(r"trophy|question|tile_\d+|keeper|sleeve_\d+"),
}
ARRAY = re.compile(r"unsigned char\s+([A-Za-z_]\w*)\s*\[[^\]]*\][^=]*=\s*\{([^}]*)\}\s*;")


def parse(text, wanted):
    """{name: bytes} of the unsigned char arrays in C text whose names match."""
    out = {}
    for m in ARRAY.finditer(text):
        if wanted.fullmatch(m.group(1)):
            out[m.group(1)] = bytes(int(v, 0) for v in m.group(2).replace("\n", "").split(",") if v.strip())
    return out


def sources(mod, iso, work):
    """{generator: C text}, each generator run with this mod folder and disc."""
    import dlc_buttons
    import dlc_frames
    import dlc_shop
    import dlc_sleeves
    mod, work = Path(mod), Path(work)
    dlc_buttons.generate(mod / "art" / "xbox_buttons.png", work / "buttons_gen.c", iso)
    dlc_frames.generate(mod, work / "frames_gen.c", work / "framemap_gen.c", iso)
    dlc_sleeves.generate(mod, work / "sleeves_gen.c", iso)
    dlc_shop.generate_pictures(mod, work / "shop_gen.c", iso)
    return {g: (work / f"{g}_gen.c").read_text() for g in WANTED}


def arrays(mod, iso):
    with tempfile.TemporaryDirectory() as work:
        texts = sources(mod, str(iso), work)
    out = {}
    for g, text in texts.items():
        for name, data in parse(text, WANTED[g]).items():
            out[f"{g}:{name}"] = data
    return out


def places(code, data):
    """Every offset of data in code."""
    found, at = [], code.find(data)
    while at >= 0:
        found.append(at)
        at = code.find(data, at + 1)
    return found


if __name__ == "__main__":
    for key, data in arrays(sys.argv[1], sys.argv[2]).items():
        print(key, len(data))
