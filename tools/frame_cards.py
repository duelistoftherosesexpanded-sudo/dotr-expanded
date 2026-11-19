#!/usr/bin/env python3
"""Gives every new monster card without a 3D model of its own a picture-frame model made from its
card art (tools/frame_model.py): picture frames/<number> <name>/frame.glb and frame_atlas.png,
and the card's cards.json fields (model, model_texture, model_animated, model_frame). A card
whose art is still the placeholder is left as it is. Run again after a card's art changes (the
frames it made before are made again).

The game shrinks a model's texture to its donor's texture size, and a frame card shows none of its
donor's model, so a frame card's donor is chosen for its texture: one of the game's monsters
without an effect with the largest texture (512 x 256 for most), of the card's type where one has
it, then the nearest level. Before a donor changes, what the card took from the old one (type,
attribute, level, attack, defense, deck cost, and its effect if it had one) is written onto the
card, so the card plays the same. model_frame keeps the texture size the frame was made for (the
player's setup tool makes the frame again at that size, tools/setup_core.py).

  frame_cards.py [--dry-run] [--keep-donors]
"""
import argparse
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
MOD = ROOT / "mods" / "widescreen"
CARDS = MOD / "cards.json"
FRAMES = ROOT / "picture frames"


def slug(name):
    return re.sub(r"\s+", " ", re.sub(r'[<>:"/\\|?*]', "", name)).strip().lower()


class Textures:
    """Texture size of the game's model of a card (its slot in DATA/MONSTER.MRG, clean disc)."""

    def __init__(self):
        import iso_files
        self.iso = iso_files.Image(next((ROOT / "baserom").glob("*.iso")), read_only=True)
        _, _, entry = self.iso.lookup("/DATA/MONSTER.MRG")
        self.base = struct.unpack_from("<I", entry, 2)[0]
        self.cache = {}

    def size(self, card):
        if card not in self.cache:
            import model_research as mr
            import model_texture as mt
            try:
                data = bytearray(self.iso.read(self.base + card * 512, 512).ljust(0x200000, b"\0"))
                at, = struct.unpack_from("<I", data, mr.segments(data)[5][0] + 16)
                info = mt.info(data, at)
                self.cache[card] = (info["width"], info["height"])
            except Exception:
                self.cache[card] = None
        return self.cache[card]


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--keep-donors", action="store_true", help="make the frames for the donors the cards have")
    a = p.parse_args()
    import dlc_cards
    import frame_model
    exe = dlc_cards.game_exe()
    names = dlc_cards.card_names(dlc_cards.Text(exe))
    textures = Textures()
    cards = json.loads(CARDS.read_text(encoding="utf-8"))
    pool = []
    for k in range(683):
        rec = dlc_cards.record(exe, k)
        size = textures.size(k) if rec["effect"] == 0xFFFF and k != 671 else None
        if size:
            pool.append((k, rec, size))
    used = Counter(c.get("donor") for c in cards if "donor" in c)
    made = []
    for c in cards:
        if "number" not in c or c["number"] < dlc_cards.FIRST:
            continue
        if c.get("model") and not c.get("model_frame"):
            continue
        donor = c.get("donor")
        donor = donor if isinstance(donor, int) else names.get(str(donor).lower())
        if donor is None or not dlc_cards.record(exe, donor)["monster"]:
            continue                      # spells and traps have no model
        art = c.get("art")
        if not art or art == "placeholder.png" or not (MOD / art).is_file():
            continue
        old = dlc_cards.record(exe, donor)
        mtype = c.get("monster_type")
        mtype = dlc_cards.MONSTER_TYPES.index(mtype) if isinstance(mtype, str) and mtype in dlc_cards.MONSTER_TYPES else old["monster_type"]
        level = int(c.get("level", old["level"]))
        if not a.keep_donors:
            best = max(pool, key=lambda kr: (kr[2][0] * kr[2][1], kr[1]["monster_type"] == mtype,
                                             -abs(kr[1]["level"] - level), -used[kr[0]], -kr[0]))
            if best[0] != donor:
                # What the card had from its old donor stays the card's own.
                c.setdefault("monster_type", dlc_cards.MONSTER_TYPES[old["monster_type"]] if old["monster_type"] < 21 else old["monster_type"])
                c.setdefault("attribute", dlc_cards.ATTRIBUTES[old["attribute"]] if old["attribute"] < 6 else old["attribute"])
                c.setdefault("level", old["level"])
                c.setdefault("attack", old["attack"])
                c.setdefault("defense", old["defense"])
                c.setdefault("deck_cost", old["deck_cost"])
                if old["effect"] != 0xFFFF:
                    c.setdefault("effect_from", donor)
                used[donor] -= 1
                used[best[0]] += 1
                c["donor"] = best[0]
                donor = best[0]
        size = textures.size(donor) or (256, 128)
        folder = FRAMES / f"{c['number']} {slug(c.get('name') or str(c['number']))}"
        made.append((c["number"], c.get("name"), f"donor {donor}", f"{size[0]}x{size[1]}"))
        if a.dry_run:
            continue
        glb, atlas = frame_model.make(MOD / art, folder, size=size)
        c["model"] = f"../../picture frames/{folder.name}/{glb.name}"
        c["model_texture"] = f"../../picture frames/{folder.name}/{atlas.name}"
        c["model_animated"] = True
        c["model_frame"] = list(size)     # a stand-in made here, for a texture of this size
        c.setdefault("model_size", 1.0)
    for m in made:
        print(*m)
    print(f"{len(made)} picture frames; textures {dict(Counter(m[3] for m in made))}")
    if not a.dry_run and made:
        CARDS.write_text(json.dumps(cards, indent=2, ensure_ascii=False) + "\n", encoding="utf-8", newline="\n")


if __name__ == "__main__":
    main()
