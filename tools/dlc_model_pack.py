#!/usr/bin/env python3
"""Adds a folder of model folders (dotr-dlc-models/) to a mod's cards.json as new cards, one per
folder, numbered from the first free placeholder: the card is named after its folder, takes its 3D
model (the FBX or OBJ in the folder; of an "_01J" and "_01A" pair the 01J) and the texture next to
it ("..._def.png", else the only PNG), and gets card art drawn from the model (tools/model_preview.py)
into the mod's art/ folder.

The folders have no animations of their own (the FBX animation stacks of these models are empty),
so each card moves with a donor's animations: CARDS below names, for each folder, a donor with a
similar body and the card's own type, attribute, level, attack and defense. A folder not listed gets
a donor by guess ("Mystical Elf") and the placeholder's stats; edit cards.json (or the Card Maker)
afterwards. A folder already in cards.json (the same model path) is left alone, so the tool can be
run again after adding folders.

  dlc_model_pack.py [--models dotr-dlc-models] [--cards mods/widescreen/cards.json] [--art-only]
"""

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_preview  # noqa: E402
import model_texture as mt  # noqa: E402
from runtime import ROOT  # noqa: E402

# folder: (card name or None for the folder's, donor, type, attribute, level, attack, defense, arms down)
CARDS = {
    "black luster": ("Black Luster", "Black Luster Soldier", "Warrior", "Light", 8, 3000, 2500, 65),
    "black rose dragon": ("Black Rose Dragon", "Meteor Dragon", "Dragon", "Fire", 7, 2400, 1800, 0),
    "dark magician of chaos": ("Dark Magician of Chaos", "Flame Manipulator", "Spellcaster", "Dark", 8,
                               2800, 2600, 65),
    "gagaga clerk": ("Gagaga Clerk", "Dark Elf", "Spellcaster", "Earth", 2, 400, 800, 65),
    "gagaga girl": ("Gagaga Girl", "Dark Elf", "Spellcaster", "Dark", 3, 1000, 800, 65),
    "gagaga magician": ("Gagaga Magician", "Rogue Doll", "Spellcaster", "Dark", 4, 1500, 1000, 65),
    "junk destroyer": ("Junk Destroyer", "Skull Guardian", "Warrior", "Earth", 8, 2600, 2500, 65),
    "junk synchron": ("Junk Synchron", "Armed Ninja", "Warrior", "Dark", 3, 1300, 500, 65),
    "junk warrior": ("Junk Warrior", "Flame Swordsman", "Warrior", "Dark", 5, 2300, 1300, 65),
    "mokey mokey": ("Mokey Mokey", "Petit Angel", "Fairy", "Light", 1, 300, 100, 0),
    "odd eyes": ("Odd Eyes", "Tri-Horned Dragon", "Dragon", "Dark", 7, 2500, 2000, 0),
    "ojama black": ("Ojama Black", "Kuriboh", "Beast", "Light", 2, 0, 1000,65),
    "ojama green": ("Ojama Green", "Kuriboh", "Beast", "Light", 2, 0, 1000,65),
    "ojama yellow": ("Ojama Yellow", "Kuriboh", "Beast", "Light", 2, 0, 1000,65),
    "red dragon archfiend": ("Red Dragon Archfiend", "Lesser Dragon", "Dragon", "Dark", 8, 3000, 2000,65),
    "rocket warrior": ("Rocket Warrior", "Celtic Guardian", "Warrior", "Light", 4, 1500, 1300, 65),
    "rose fairy": ("Rose Fairy", "Petit Angel", "Plant", "Light", 3, 600, 1200, 0),
    "stardust dragon": ("Stardust Dragon", 15, "Dragon", "Wind", 8, 2500, 2000,65),
    "v-tiger jet": ("V-Tiger Jet", "Space Megatron", "Machine", "Light", 4, 1600, 1800, 0),
    "vw-tiger catapult": ("VW-Tiger Catapult", "Pendulum Machine", "Machine", "Light", 6, 2000, 2100, 0),
    "vwxyz": ("VWXYZ", "Lava Battleguard", "Machine", "Light", 8, 3000, 2800, 0),
    "w-wing catapult": ("W-Wing Catapult", "Ground Attacker Bugroth", "Machine", "Light", 4, 1300, 1500, 0),
    "winged kuriboh": ("Winged Kuriboh", "Kuriboh", "Fairy", "Light", 1, 300, 200, 0),
    "x head cannon": ("X Head Cannon", "Cannon Soldier", "Machine", "Light", 4, 1800, 1500, 0),
    "xyz dragon cannon": ("XYZ Dragon Cannon", "Gear Golem the Moving Fortress", "Machine", "Light", 8, 2800, 2600, 0),
    "y dragon head": ("Y Dragon Head", "Metal Dragon", "Machine", "Light", 4, 1500, 1600, 0),
    "z metal tank": ("Z Metal Tank", "Labyrinth Tank", "Machine", "Light", 4, 1500, 1300, 0),
    # Folders of animations/ whose model has no card yet (animated, see attach_animations).
    "blade skater": ("Blade Skater", "Dark Elf", "Warrior", "Earth", 4, 1400, 1500, 0),
    "burstinatrix": ("Burstinatrix", "Dark Elf", "Warrior", "Fire", 3, 1200, 800, 0),
    "cyber blader": ("Cyber Blader", "Dark Elf", "Warrior", "Earth", 7, 2100, 800, 0),
    "cyber prima": ("Cyber Prima", "Dark Elf", "Warrior", "Light", 6, 2300, 1600, 0),
    "cyber tutu": ("Cyber Tutu", "Dark Elf", "Warrior", "Earth", 3, 1000, 800, 0),
    "edge imp sabres": ("Edge Imp Sabres", "Kuriboh", "Fiend", "Dark", 4, 1200, 800, 0),
    "etoile cyber": ("Etoile Cyber", "Dark Elf", "Warrior", "Earth", 4, 1200, 1600, 0),
    "performapal hip hippo": ("Performapal Hip Hippo", "Kuriboh", "Beast", "Earth", 3, 800, 800, 0),
    "traptrix atrax": ("Traptrix Atrax", "Dark Elf", "Insect", "Earth", 4, 1800, 1000, 0),
    "traptrix nepenthes": ("Traptrix Nepenthes", "Dark Elf", "Plant", "Earth", 4, 1700, 1200, 0),
    "trickster holly angel": ("Trickster Holly Angel", "Dark Elf", "Fairy", "Light", 4, 2000, 0, 0),
    # Animated models from other games (their own clips, mapped to motion slots in EXTRA).
    "jinzo": ("Jinzo", "Celtic Guardian", "Machine", "Dark", 6, 2400, 1500, 0),
    "toon dark magician girl": ("Toon Dark Magician Girl", "Dark Elf", "Spellcaster", "Dark", 6, 2000, 1700, 0),
}

# Jinzo's FBX has one 12.4 s summon clip: it lands crouched (0-2.5 s), stands (4.5-8 s) and
# spreads its arms (9-12.4 s); the other slots take pieces of it.
JINZO_CLIP = "A_Mon047A_Summon00"


# Further cards.json fields, by folder.
EXTRA = {
    # The mesh holds the calm (white) and the angry (pink) Mokey Mokey; the pink half of the texture
    # and its block at the top right belong to the angry one.
    "mokey mokey": {"model_hide_uv": [[0.0, 0.52, 1.0, 1.0], [0.59, 0.0, 1.0, 0.17]]},
    "jinzo": {"model_animated": True, "model_clips": {
        "0": [JINZO_CLIP, 6.0, 8.0], "1": [JINZO_CLIP, 0.0, 12.37], "2": [JINZO_CLIP, 4.5, 6.5],
        "3": [JINZO_CLIP, 8.5, 12.37], "4": [JINZO_CLIP, 2.0, 3.0], "5": [JINZO_CLIP, 2.0, 3.0],
        "6": [JINZO_CLIP, 2.0, 3.0]}},
    # Motions (Collada files): A flies in (summon), B idle, C a second idle, D the attack; no hit
    # reaction, so the hit slots keep idling.
    "toon dark magician girl": {"model_animated": True, "model_clips": {
        "0": "b", "1": "a", "2": "c", "3": "d", "4": "b", "5": "b", "6": "c"}},
}


def load_animated(path):
    if str(path).lower().endswith(".rig.json"):
        import rig_transfer
        return rig_transfer.load(path)
    if str(path).lower().endswith(".dae"):
        import dae_anim
        return dae_anim.load(path)
    if str(path).lower().endswith(".dat"):
        import hsd_anim
        return hsd_anim.load(path)
    if str(path).lower().endswith(".glb"):
        import glb_anim
        return glb_anim.load(path)
    import fbx_anim
    return fbx_anim.load(path)


def animated_art(model_path, texture, background, clips=None):
    """Card art of an animated model in the first frame of its idle (motion slot 0)."""
    import fbx_anim
    import model_skinned
    model = load_animated(model_path)
    want = (clips or {}).get("0", model_skinned.CLIP_SLOTS[0])
    start = 0.0
    if isinstance(want, list):
        want, start = want[0], float(want[1])
    clip = next((c for c in model["clips"] if c["name"] == want or c["name"][-1].lower() == str(want).lower()),
                model["clips"][0] if model["clips"] else None)
    positions, coords, triangles = fbx_anim.posed_mesh(model, clip, start)
    tex = mt.read_png(str(texture)) if texture else None
    return model_preview.draw(positions, coords, triangles, tex, 256, background=background)


def pick_model(folder):
    models = sorted(list(folder.glob("*.fbx")) + list(folder.glob("*.obj")))
    if not models:
        # A skinned Collada model (its motions are "<stem>_Motion*.dae"); its images packed into
        # one texture next to the mod's art.
        daes = sorted(p for p in folder.glob("*.dae") if "_Motion" not in p.stem)
        if daes:
            import dae_anim
            if len(dae_anim.images(daes[0])) > 1:
                out = ROOT / "mods" / "widescreen" / "textures" / f"{slug(folder.name)}_atlas.png"
                out.parent.mkdir(exist_ok=True)
                return daes[0], dae_anim.atlas(daes[0], out)
            found = dae_anim.images(daes[0])
            return daes[0], (found[0] if found else None)
        return None, None
    model = next((m for m in models if m.stem.replace(" (merge)", "").endswith("_01J")), models[0])
    pngs = sorted(folder.glob("*.png"))
    stem = model.stem.replace(" (merge)", "")
    texture = next((p for p in pngs if p.name == f"tex_{stem}_1_def.png"), None)
    texture = texture or next((p for p in pngs if p.name.endswith("_def.png")), None)
    texture = texture or (pngs[0] if len(pngs) == 1 else next((p for p in pngs if "msk" not in p.name
                                                               and not p.name.startswith("EFF_")), None))
    return model, texture


def attach_animations(cards, base, folder):
    """Cards whose model is a Duel Links model (a file named after its five-digit ID) take the
    animated FBX of the same model from `folder` ("<ID>_01J (merge).fbx", the same variant as the
    card's model when there is one) and its texture, with model_animated set: the card then moves
    with the model's own animations (tools/model_skinned.py)."""
    import os
    found = {}
    for path in folder.glob("*/*(merge).fbx"):
        found.setdefault(path.name[:5], []).append(path)
    for card in cards:
        model = card.get("model")
        if not model or card.get("model_animated"):
            continue
        stem = Path(model).stem.replace(" (merge)", "")
        match = re.match(r"(\d{5})_(\w+)", stem)
        if not match or match.group(1) not in found:
            continue
        options = found[match.group(1)]
        anim = next((p for p in options if p.name.startswith(stem)), None) or \
            next((p for p in options if "_01J" in p.name), options[0])
        variant = re.match(r"(\d{5}_\w+?) \(merge\)", anim.name).group(1)
        texture = anim.parent / f"tex_{variant}_1_def.png"
        card["model"] = Path(os.path.relpath(anim, base)).as_posix()
        if texture.is_file():
            card["model_texture"] = Path(os.path.relpath(texture, base)).as_posix()
        card["model_animated"] = True
        for key in ("model_arms_down", "model_hide_uv"):
            card.pop(key, None)
        print(f"card {card['number']} {card.get('name')}: animated from {anim.parent.name}/{anim.name}")


def slug(name):
    return re.sub(r"[^a-z0-9]+", "_", name.lower()).strip("_")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--models", default=str(ROOT / "dotr-dlc-models"))
    parser.add_argument("--cards", default=str(ROOT / "mods" / "widescreen" / "cards.json"))
    parser.add_argument("--art-only", action="store_true", help="only (re)draw the art of the listed folders")
    parser.add_argument("--only", nargs="*", help="folder names to handle (default: all)")
    parser.add_argument("--update", action="store_true",
                        help="also give cards already added the donor and stats listed in CARDS")
    parser.add_argument("--animations", nargs="?", const=str(ROOT / "animations"),
                        help="give cards the animated FBX ('ID_01J (merge).fbx') of the same Duel Links model "
                             "found in this folder (default: animations/)")
    args = parser.parse_args(argv)
    cards_path = Path(args.cards)
    base = cards_path.parent
    cards = json.loads(cards_path.read_text())
    if args.animations:
        attach_animations(cards, base, Path(args.animations))
        cards_path.write_text(json.dumps(cards, indent=2) + "\n")
        return 0
    used = {c["number"] for c in cards if "number" in c}
    have_models = {str((base / c["model"]).resolve()) for c in cards if c.get("model")}
    placeholders = next((c for c in cards if "placeholders" in c), None)
    first, last = placeholders["placeholders"] if placeholders else (871, 998)
    free = [n for n in range(first, last + 1) if n not in used and n != 999]
    (base / "art").mkdir(exist_ok=True)
    added = []
    for folder in sorted(p for p in Path(args.models).iterdir() if p.is_dir()):
        if args.only and folder.name not in args.only:
            continue
        model, texture = pick_model(folder)
        if model is None:
            print(f"{folder.name}: no FBX or OBJ, skipped")
            continue
        entry = CARDS.get(folder.name.lower())
        name, donor, kind, attribute, level, attack, defense, arms = entry or (
            None, "Mystical Elf", None, None, None, None, None, 0)
        name = name or folder.name.title()
        art = base / "art" / f"{slug(name)}.png"
        extra = EXTRA.get(folder.name.lower(), {})
        hide = extra.get("model_hide_uv")
        if extra.get("model_animated"):    # drawn in its idle pose
            pixels = animated_art(model, texture, (attribute or "grey").lower(), extra.get("model_clips"))
            arms = 0
        else:
            try:
                pixels = model_preview.render(model, texture, 256, arms_down=arms if model.suffix == ".fbx" else 0,
                                              background=(attribute or "grey").lower(), hide_uv=hide)
            except ValueError as error:   # no arm bones: draw it as it is
                print(f"{folder.name}: {error}; drawn without lowering the arms")
                arms = 0
                pixels = model_preview.render(model, texture, 256, background=(attribute or "grey").lower(),
                                              hide_uv=hide)
        mt.write_png(art, 256, 256, pixels)
        if args.art_only or str(model.resolve()) in have_models:
            if args.update and entry:
                for card in cards:
                    if card.get("model") and str((base / card["model"]).resolve()) == str(model.resolve()):
                        card.update({"donor": donor, "monster_type": kind, "attribute": attribute, "level": level,
                                     "attack": attack, "defense": defense})
                        if arms:
                            card["model_arms_down"] = arms
                        card.update(extra)
                        added.append(None)
                        print(f"{folder.name}: card {card['number']} now uses donor {donor}")
            print(f"{folder.name}: art drawn")
            continue
        # A card already named like the folder (Slifer: "Slifer the Sky Dragon") that has no model of
        # its own gets this one, instead of a second card.
        existing = next((c for c in cards if c.get("name", "").lower().startswith(folder.name.lower())
                         and not c.get("model")), None)
        if existing:
            existing["model"] = Path(__import__("os").path.relpath(model, base)).as_posix()
            if texture:
                existing["model_texture"] = Path(__import__("os").path.relpath(texture, base)).as_posix()
            existing["art"] = art.relative_to(base).as_posix()
            added.append(None)
            print(f"{folder.name}: model given to card {existing['number']} {existing['name']}")
            continue
        if not free:
            print(f"{folder.name}: no free card number left, skipped")
            continue
        number = free.pop(0)
        card = {"number": number, "donor": donor, "name": name,
                "description": f"{name}.\nA new card from the DLC.", "password": f"{number:08d}"}
        for key, value in (("monster_type", kind), ("attribute", attribute), ("level", level),
                           ("attack", attack), ("defense", defense)):
            if value is not None:
                card[key] = value
        card["art"] = art.relative_to(base).as_posix()
        card["model"] = Path(__import__("os").path.relpath(model, base)).as_posix()
        if texture:
            card["model_texture"] = Path(__import__("os").path.relpath(texture, base)).as_posix()
        if "(merge)" in model.name:      # an animated Duel Links FBX: its own skeleton and clips
            card["model_animated"] = True
        elif arms:
            card["model_arms_down"] = arms
        card.update(extra)
        added.append(card)
        print(f"{folder.name}: card {number} {name} (donor {donor})")
    if added:
        added = [c for c in added if c is not None]
        at = cards.index(placeholders) if placeholders else len(cards)
        cards[at:at] = added
        cards_path.write_text(json.dumps(cards, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
