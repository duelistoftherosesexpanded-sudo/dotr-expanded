"""Card effects and leader abilities of the game, read from the executable: a catalogue.

    python3 tools/card_effects.py [--md docs/topics/card-effects.md] [--json <catalogue.json>]

Where the game keeps them (docs/topics/card-effects.md says more):

  card record bytes 4-5  the card's effect row, 0xffff for none
  EffIdx_M  0x2c9df0     monsters: 256 rows of five slots, each two words (8 bytes)
  EffIdx_S  0x2cc5f0     every other card: rows of one slot
  EffIdx_L  0x2c3338     leader abilities: one row of five slots per monster card (0-682), read
                         as nine words by SzDuel_GetUnitLeaderAbility3

A slot's first word holds the effect type above its low byte (word0 >> 8); the low byte and the
second word are the type's parameters. SzDuel_InitCardInfo copies a card's row into its unit when
it is put on the field; the leader unit gets the card's EffIdx_L row instead (SzDuel_AddUnit2).

The JSON catalogue lists, per monster slot and per card kind, every effect type with the cards
that use it, their words and their descriptions: what the card maker offers as effect templates."""

import argparse
import collections
import json
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dotr.elf import file_offset, inventory  # noqa: E402
from dlc_cards import Text, MONSTER_TYPES, CARD_TYPES  # noqa: E402

RECORDS = 0x2bf080
EFF_L, EFF_M, EFF_S = 0x2c3338, 0x2c9df0, 0x2cc5f0
NAMES, DESCRIPTIONS = 320, 1174

SLOTS = {
    0: "slot 0: ATTACK (in battle)",
    1: "slot 1: MOVEMENT",
    2: "slot 2: NATURE EFFECT (while face up in defence)",
    3: "slot 3: FLIP (when flipped face up)",
    4: "slot 4: DESTRUCTION (when destroyed)",
}

# Effect types by what they do (names from the cases of the three routines below; the passive
# ones from the cards that use them).
TYPE_NAMES = {
    1: "Cancel enemy power-ups in battle", 2: "Terrain change", 4: "Destroy", 5: "No LP damage in battle",
    6: "Extra movement", 7: "Ignore limited traps", 8: "Enter labyrinth", 9: "Enter labyrinth and transform",
    10: "Enter labyrinth, make it normal", 11: "Immune to spells (type)", 12: "Transform", 13: "Change LP",
    14: "Spells and rituals ineffective", 15: "Traps cannot trigger", 17: "Trap", 18: "ATK/DEF boost",
    19: "ATK/DEF reduction", 20: "Revive", 22: "Grow to a value", 23: "Spellbind", 24: "Flip cards face up",
    26: "Set summoning power", 27: "Battle bonus against a type", 29: "Reveal hand", 30: "Reveal types",
    31: "Take control", 32: "Magic-neutralising field", 33: "Swap ATK and DEF", 34: "DEF boost",
    35: "Shift to attack position", 36: "Return to deck", 37: "Teleport", 38: "Summon again",
    39: "Steal summoning power", 40: "Earthshaker", 50: "Ritual", 51: "Time Wizard",
    52: "Power-up cards stronger", 53: "ATK boost from graveyard", 54: "Change monster type",
    55: "Change attribute", 56: "Summon a face-up copy", 57: "Control can't be taken",
    58: "Destroyed foes can't be revived", 59: "Change LP damage/recovery", 60: "Teleport (Jigen Bakudan)",
    61: "Classes visible face down", 62: "Revive strongest for 1000 LP", 63: "Random ATK/DEF in battle",
    65: "Boost monsters 1000 for 1000 LP", 66: "LP cost per move", 67: "Cancel extra movement",
    70: "Blast after turns", 71: "Spellbind count paused", 72: "Magical Labyrinth", 73: "Flip and spellbind",
    77: "Weaken enemies each turn", 78: "Boost a type each turn", 79: "Cannot move", 81: "Jowls of Dark Demise",
    82: "Copy leaders' stats", 83: "Eat graveyards", 84: "Pull cards closer", 85: "Spellbind count doubled",
    86: "Summoning power per turn", 87: "Discard hands", 88: "Leader abilities ineffective",
}
for _t in (41, 42, 43, 44, 45, 46, 48, 49, 74, 75, 76, 80):
    TYPE_NAMES.setdefault(_t, "Trap (waits for an event)")

# Which types each trigger can run: the cases of the routine that runs it. A spell's effect type
# that the FLIP or DESTRUCTION routine also has can be given to a monster (and a monster's to a
# new spell). Full-range traps wait for events (moves, spells) checked elsewhere; their types
# have no case in these routines.
RUNS = {
    "spell": {2, 4, 13, 18, 20, 23, 24, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 50, 54, 72, 73},  # Ta_Do_Magic
    "flip": {2, 4, 12, 13, 18, 19, 20, 22, 23, 24, 26, 30, 31, 35, 36, 37, 38, 51, 53, 54, 55, 56,
             60, 70, 81, 87},                                                             # Ta_Manage_ActEfc
    "destruction": {2, 4, 13, 19, 20, 23, 26, 36, 38, 56, 62, 65, 81},                    # Ta_Manage_DestEfc
}


def type_name(t):
    return TYPE_NAMES.get(int(t), f"type {t}")


# The low byte of word 0: bits 6-7 whose cards (Ta_SideMatch: 1 own, 2 the opponent's, 3 both;
# 0 the card itself or no target), bits 0-5 which cards (the game's search kinds).
SCOPES = {0: "self / none", 1: "own", 2: "enemy", 3: "both"}
TARGETS = {i: n for i, n in enumerate(
    ["Dragon", "Spellcaster", "Zombie", "Warrior", "Beast-Warrior", "Beast", "Winged Beast", "Fiend",
     "Fairy", "Insect", "Dinosaur", "Reptile", "Fish", "Sea Serpent", "Machine", "Thunder", "Aqua",
     "Pyro", "Rock", "Plant", "Light", "Dark", "Fire", "Earth", "Water", "Wind"])}
TARGETS.update({27: "this card", 28: "card number", 29: "monsters", 30: "non-monsters", 31: "all cards", 49: "female",
                52: "non-leaders", 59: "Immortal", 61: "all cards (2)"})


def target(word0):
    low = word0 & 0xff
    return dict(scope=SCOPES[low >> 6], kind=TARGETS.get(low & 63, f"kind {low & 63}"))

ABILITIES = [
    # (number, name, word, half, rank in high byte (True) or low byte (False), has a value)
    (0, "Extra slots", 0, 1, None, False),
    (1, "Hidden card", 0, 0, None, False),
    (2, "Summoning power bonus", 1, 1, True, True),   # "EXODIA?" in modders' notes; value 1-3
    (3, "Destiny draw", 1, 0, True, False),
    (4, "Increased movement", 2, 1, True, False),
    (5, "LP recovery", 2, 0, True, False),
    (6, "Direct damage halved", 3, 1, True, False),
    (7, "Terrain change", 3, 0, False, True),
    (8, "Level cost reduction (same type)", 4, 1, False, True),
    (9, "Extended support range", 4, 0, True, False),
    (10, "Increased strength (same type)", 5, 1, False, True),
    (11, "Improved resistance (same type)", 5, 0, True, False),
    (12, "Movement boost (same type)", 6, 1, True, False),
    (13, "Open opponent's cards", 6, 0, True, False),
    (14, "Weaken specific enemy type", 7, 1, False, True),
    (15, "Spellbind specific enemy type", 7, 0, True, False),
    (16, "Destroy specific enemy type", 8, 1, True, False),
]


def clean(s):
    return " ".join(s.replace("?", " ").split())


def leader_abilities(words):
    """[(ability, name, rank needed, value)] of a nine-word EffIdx_L row."""
    out = []
    for number, name, w, high, rank_high, has_value in ABILITIES:
        v = words[w] >> 16 if high else words[w] & 0xffff
        if v == 0xffff:
            continue
        if rank_high is None:
            if v:
                out.append((number, name, 0, None))
            continue
        rank = v >> 8 if rank_high else v & 0xff
        value = (v & 0xff if rank_high else v >> 8) if has_value else None
        out.append((number, name, rank, value))
    return out


def read(exe):
    meta = inventory(exe)
    text = Text(exe)

    def rd(addr, n):
        return exe[file_offset(meta, addr, n):][:n]

    cards = []
    for c in range(854):
        r = rd(RECORDS + 20 * c, 20)
        cards.append(dict(id=c, name=text.decode(NAMES + c), kind=r[0] >> 5, mtype=r[0] & 31,
                          effect_row=struct.unpack_from("<H", r, 4)[0],
                          description=clean(text.decode(DESCRIPTIONS + c))))
    monsters = collections.defaultdict(lambda: collections.defaultdict(list))
    others = collections.defaultdict(lambda: collections.defaultdict(list))
    leaders = {}
    for c in cards:
        if c["id"] < 683 and c["id"] != 671:
            words = struct.unpack("<10I", rd(EFF_L + 40 * c["id"], 40))
            abilities = leader_abilities(words)
            if abilities:
                leaders[c["id"]] = dict(name=c["name"], type=MONSTER_TYPES[c["mtype"]] if c["mtype"] < 21 else c["mtype"],
                                        words=[f"{w:#010x}" for w in words[:9]], abilities=abilities)
        if c["effect_row"] == 0xffff:
            continue
        if c["id"] < 683:
            row = struct.unpack("<10I", rd(EFF_M + 40 * c["effect_row"], 40))
            for s in range(5):
                w0, w1 = row[2 * s], row[2 * s + 1]
                if w0 == 0xffffffff and w1 == 0xffffffff:
                    continue
                monsters[s][w0 >> 8].append(dict(card=c["id"], name=c["name"], row=c["effect_row"],
                                                 word0=f"{w0:#010x}", word1=f"{w1:#010x}",
                                                 target=target(w0), value=w1 & 0xffff, extra=w1 >> 16,
                                                 description=c["description"]))
        else:
            w0, w1 = struct.unpack("<II", rd(EFF_S + 8 * c["effect_row"], 8))
            others[c["kind"]][w0 >> 8].append(dict(card=c["id"], name=c["name"], row=c["effect_row"],
                                                  word0=f"{w0:#010x}", word1=f"{w1:#010x}",
                                                  target=target(w0), value=w1 & 0xffff, extra=w1 >> 16,
                                                  description=c["description"]))
    return cards, monsters, others, leaders


def catalogue(exe):
    cards, monsters, others, leaders = read(exe)
    return dict(
        monster_slots={str(s): {str(t): v for t, v in sorted(types.items())} for s, types in sorted(monsters.items())},
        card_kinds={CARD_TYPES[k] if k < len(CARD_TYPES) else str(k): {str(t): v for t, v in sorted(types.items())}
                    for k, types in sorted(others.items())},
        leaders={str(k): v for k, v in leaders.items()},
        abilities=[dict(number=n, name=name, has_value=hv) for n, name, _, _, _, hv in ABILITIES],
        monster_bundles={str(s): bundle(types, s) for s, types in sorted(monsters.items())},
        card_bundles={CARD_TYPES[k] if k < len(CARD_TYPES) else str(k): bundle(types, None)
                      for k, types in sorted(others.items())},
    )


# The headings of the game's descriptions; one starts the description or follows a sentence's end.
HEADINGS = ["ATTACK", "MOVEMENT", "NATURE EFFECT", "FLIP", "DESTRUCTION", "SPECIAL POWER UP"]
SLOT_HEADING = {0: "ATTACK", 1: "MOVEMENT", 2: "NATURE EFFECT", 3: "FLIP", 4: "DESTRUCTION"}
_HEADING = re.compile(r"(?:^|(?<=[!.)]) )(" + "|".join(HEADINGS) + r")\b ?")


def section(description, slot, name):
    """The part of a description under the slot's heading (all of it for other cards), with the
    card's own name as "this card"."""
    text = description
    if slot is not None:
        parts = _HEADING.split(description)
        found = {parts[i]: parts[i + 1].strip() for i in range(1, len(parts) - 1, 2)}
        text = found.get(SLOT_HEADING[slot], description)
    else:
        text = re.sub(r"^(MAGIC|RITUAL|POWER UP|TRAP( ?FULL RANGE| ?LIMITED RANGE)?|EQUIP) *", "", text)
    if name:
        text = text.replace(name, "this card")
    return text.strip() or description


def bundle(types, slot):
    """Like effects together: the cards whose slot holds the same two words do the same thing.
    One entry per pair of words, sorted by type and amount, with the description they share."""
    groups = {}
    for t, users in types.items():
        for u in users:
            # Type 0 of power-ups and limited traps says nothing (other tables decide): one each.
            key = (u["word0"], u["word1"]) if t != 0 else u["card"]
            g = groups.setdefault(key, dict(type=t, word0=u["word0"], word1=u["word1"], target=u["target"],
                                            value=u["value"], extra=u["extra"], texts={}, cards=[]))
            g["cards"].append(dict(card=u["card"], name=u["name"]))
            text = section(u["description"], slot, u["name"])
            g["texts"][text] = g["texts"].get(text, 0) + 1
    out = []
    for g in groups.values():
        g["text"] = max(g.pop("texts").items(), key=lambda kv: (kv[1], -len(kv[0])))[0]
        g["type_name"] = type_name(g["type"])
        out.append(g)
    return sorted(out, key=lambda g: (g["type"], g["extra"], g["value"], g["word0"]))


def markdown(cat):
    out = ["# Card effects and leader abilities", "",
           "Generated by `tools/card_effects.py` from the executable; the layout is described there.",
           "Each effect is a slot of two words:", "",
           "- **type** = word0 >> 8: what the effect does (the lists below, with every card that uses it);",
           "- **target** = the low byte of word0: bits 6-7 whose cards (0 the card itself or none, 1 own,",
           "  2 the opponent's, 3 both), bits 0-5 which cards (0-19 a monster type, 20-25 an attribute,",
           "  29 all monsters, 49 female ...; the game's search kinds);",
           "- **word1**: the amounts, usually a value in the low half (points, turns, a card number, a",
           "  terrain) and a second one in the high half (a monster type, a range, a count).", "",
           "Monsters have five slots, one per kind of trigger (the headings in the game's descriptions:",
           "ATTACK, MOVEMENT, NATURE EFFECT, FLIP, DESTRUCTION); Ta_Open runs slot 3 when a card is",
           "flipped and Ta_Manage_DestEfc slot 4 when one is destroyed. Other cards have one slot.", ""]
    out += ["## Monster effects (EffIdx_M)", ""]
    def bundles(groups):
        lines, last = [], None
        for g in groups:
            if g["type"] != last:
                lines.append(f"- **type {g['type']}: {g['type_name']}**")
                last = g["type"]
            cards = ", ".join(c["name"] for c in g["cards"][:8]) + (f" and {len(g['cards']) - 8} more"
                                                                     if len(g["cards"]) > 8 else "")
            lines.append(f"  - {g['text']} `{g['word0']} {g['word1']}` [{g['target']['scope']}, "
                         f"{g['target']['kind']}; {g['value']}, {g['extra']}] ({len(g['cards'])}: {cards})")
        return lines

    out.append("Like effects are bundled: the cards listed after one do exactly the same (the same two words).")
    out.append("")
    for s, groups in cat["monster_bundles"].items():
        out += [f"### {SLOTS[int(s)]}", ""]
        out += bundles(groups)
        out.append("")
    out += ["## Spell, trap, ritual and power-up effects (EffIdx_S)", "",
            "Power-ups (equips) and limited-range traps keep type 0 here: their behaviour comes from other",
            "tables (the equip match table at 0x29d580 with kinds at 0x29d1f0, and the trap kind the unit",
            "carries, `SzDuel_GetUnitKindOfs`).", ""]
    for kind, groups in cat["card_bundles"].items():
        out += [f"### {kind}", ""]
        out += bundles(groups)
        out.append("")
    out += ["## Which effects can go where", "",
            "Spells, FLIP and DESTRUCTION each run through a routine with one case per effect type",
            "(Ta_Do_Magic, Ta_Manage_ActEfc, Ta_Manage_DestEfc); the numbering is shared, so a spell's effect",
            "whose type the FLIP routine also has can be a monster's FLIP effect (and the other way round, for",
            "new spells). Whether the parameters mean exactly the same in both routines is checked case by",
            "case only in play. Full-range traps wait for events (moves, spells) checked elsewhere: their",
            "types have no case in these routines. ATTACK, MOVEMENT and NATURE EFFECT are read where they",
            "apply (battle, movement, each turn), not through one routine.", "",
            "| type | effect | spell | FLIP | DESTRUCTION |", "|---|---|---|---|---|"]
    for t in sorted(set().union(*RUNS.values())):
        out.append(f"| {t} | {type_name(t)} | " + " | ".join("yes" if t in RUNS[k] else ""
                                                           for k in ("spell", "flip", "destruction")) + " |")
    out.append("")
    out += ["## Leader abilities (EffIdx_L)", "",
            "One row per monster card. Ability (rank needed, value where the ability has one):", ""]
    for cid, l in cat["leaders"].items():
        abil = "; ".join(f"{name} (rank {rank}{'' if value is None else f', value {value}'})"
                         for _, name, rank, value in l["abilities"])
        out.append(f"- {l['name']} (#{cid}, {l['type']}): {abil}")
    return "\n".join(out) + "\n"


def main():
    import mod as M
    parser = argparse.ArgumentParser()
    parser.add_argument("--md")
    parser.add_argument("--json")
    args = parser.parse_args()
    exe, _ = M.verified_elf()
    cat = catalogue(exe)
    if args.json:
        Path(args.json).write_text(json.dumps(cat, indent=1))
    if args.md:
        Path(args.md).write_text(markdown(cat))
    print({k: len(v) for k, v in cat.items()})


if __name__ == "__main__":
    main()
