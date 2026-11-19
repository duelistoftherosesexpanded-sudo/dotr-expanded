"""Card effects and leader abilities of one's own: mods/<mod>/effects.json -> C for the DLC
(dlc/effects.c reads it; docs/topics/card-effects.md describes the game's tables).

effects.json is a list, one entry per card (a game card 0-853 or a new card 871-998, by number or
name):

    {"card": "Bitron",
     "effects": {"from": "Skull Guardian",                      monsters: the five slots
                 "attack": {"copy": "Seiyaryu", "slot": "attack"},
                 "flip": {"type": 18, "scope": "own", "target": "Machine", "value": 300},
                 "destruction": "none"},
     "effect": {"copy": "Dark Hole"},                           other cards: the one slot
     "leader": {"from": "Dark Magician",                        leader abilities
                "abilities": {"10": {"rank": 3, "value": 5}, "13": "none"}}}

A slot is "none", {"copy": card, "slot": name} (another card's slot; the same slot by default),
{"type", "scope", "target", "value", "extra"} (type: word0 >> 8; scope: self, own, enemy or both;
target: a monster type, an attribute, a kind number ...; value and extra: the low and high
halves of word1), or {"word0", "word1"} as numbers. Slots not given keep the card's own (for a
new card: its record's, i.e. its donor's or effect_from's), or "from"'s when given.

Leader abilities (numbers as in tools/card_effects.py ABILITIES) start from the card's own row
(a new card: its donor's) or "from"'s; each given ability is "none" or {"rank", "value"} (rank
0-12 needed; value where the ability has one: a terrain, a level, points / 100, a monster type)."""

import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dotr.elf import file_offset, inventory  # noqa: E402
import dlc_cards  # noqa: E402
from card_effects import ABILITIES, EFF_L, EFF_M, EFF_S, RECORDS, SCOPES, TARGETS  # noqa: E402

SLOT_NAMES = ["attack", "movement", "nature", "flip", "destruction"]
NONE = 0xffffffff
# Effects stacked on a slot after its own (dlc/dlc_effects.h DLC_STACK_MAX); the slots that run
# stacked effects (dlc/stack.c): all five.
STACK_MAX = 4
# The table always has room for this many cards, so it keeps its size in DLC.BIN when effects.json
# changes and the launcher can write it into the running game (tools/mod.py's live region).
CARD_CAPACITY = 256
STACKABLE = ["attack", "movement", "nature", "flip", "destruction"]


class Effects:
    def __init__(self, exe, cards_path=None):
        self.exe = exe
        self.meta = inventory(exe)
        self.text = dlc_cards.Text(exe)
        self.names = dlc_cards.card_names(self.text)
        self.new = {}
        if cards_path and Path(cards_path).is_file():
            for c in dlc_cards.load_cards(cards_path, self.text):
                self.new[int(c["number"])] = c
                self.names.setdefault(c["name"].strip().lower(), int(c["number"]))

    def rd(self, addr, n):
        return self.exe[file_offset(self.meta, addr, n):][:n]

    def number(self, card):
        if isinstance(card, int) or (isinstance(card, str) and card.strip().isdigit()):
            n = int(card)
        else:
            n = self.names.get(str(card).strip().lower())
            if n is None:
                raise ValueError(f"no card named {card!r}")
        if not (0 <= n < 854 or n in self.new) or n == 671:
            raise ValueError(f"card {card!r}: not a card of the game or of cards.json")
        return n

    def game_card(self, n):
        """The game card whose record a card uses (a new card: its donor)."""
        return self.new[n]["donor"] if n in self.new else n

    def is_monster(self, n):
        return self.game_card(n) < 683

    def effect_row(self, n):
        """The card's effect words (10 for a monster, 2 otherwise), as the game has them."""
        source = n
        if n in self.new:
            source = self.new[n]["_effect_from"] if self.new[n]["_effect_from"] >= 0 else self.new[n]["donor"]
        row = struct.unpack_from("<H", self.rd(RECORDS + 20 * source, 20), 4)[0]
        if source < 683:
            return list(struct.unpack("<10I", self.rd(EFF_M + 40 * row, 40))) if row != 0xffff else [NONE] * 10
        return list(struct.unpack("<2I", self.rd(EFF_S + 8 * row, 8))) if row != 0xffff else [NONE] * 2

    def leader_row(self, n):
        g = self.game_card(n)
        if g >= 683:
            return [NONE] * 10
        return list(struct.unpack("<10I", self.rd(EFF_L + 40 * g, 40)))

    def slot(self, spec, slot_index, where):
        if spec == "none":
            return NONE, NONE
        if not isinstance(spec, dict):
            raise ValueError(f"{where}: a slot is \"none\" or an object")
        if "copy" in spec:
            source = self.number(spec["copy"])
            words = self.effect_row(source)
            s = SLOT_NAMES.index(spec.get("slot", SLOT_NAMES[slot_index])) if len(words) == 10 else 0
            return words[2 * s], words[2 * s + 1]
        if "word0" in spec:
            return int(str(spec["word0"]), 0) & NONE, int(str(spec.get("word1", 0)), 0) & NONE
        if "type" not in spec:
            raise ValueError(f"{where}: give copy, type or word0")
        kind = int(spec["type"])
        if not 0 <= kind < 0x1000000:
            raise ValueError(f"{where}: type out of range")
        scopes = {v: k for k, v in SCOPES.items()}
        scopes.update(self=0, none=0)
        scope = spec.get("scope", "self")
        scope = scopes[scope] if scope in scopes else int(scope)
        target = spec.get("target", 0)
        if isinstance(target, str) and not target.isdigit():
            lowered = {v.lower(): k for k, v in TARGETS.items()}
            if target.lower() not in lowered:
                raise ValueError(f"{where}: target {target!r} is not one of {', '.join(TARGETS.values())}")
            target = lowered[target.lower()]
        target = int(target)
        value, extra = int(spec.get("value", 0)), int(spec.get("extra", 0))
        if not (0 <= scope < 4 and 0 <= target < 64 and 0 <= value < 65536 and 0 <= extra < 65536):
            raise ValueError(f"{where}: scope 0-3, target 0-63, value and extra 0-65535")
        return (kind << 8) | (scope << 6) | target, (extra << 16) | value

    def leader(self, spec, n, where):
        words = self.leader_row(self.number(spec["from"])) if "from" in spec else self.leader_row(n)
        for key, a in spec.get("abilities", {}).items():
            number = int(key)
            entry = next((x for x in ABILITIES if x[0] == number), None)
            if entry is None:
                raise ValueError(f"{where}: no leader ability {key}")
            _, name, w, high, rank_high, has_value = entry
            if a == "none":
                half = 0xffff
            elif rank_high is None:
                half = 1
            else:
                rank, value = int(a.get("rank", 0)), int(a.get("value", 0)) if has_value else 0
                if not (0 <= rank <= 12 and 0 <= value < 256):
                    raise ValueError(f"{where}: {name}: rank 0-12, value 0-255")
                half = (rank << 8 | value) if rank_high else (value << 8 | rank)
            if high:
                words[w] = (words[w] & 0xffff) | (half << 16)
            else:
                words[w] = (words[w] & 0xffff0000) | half
        return words

    def entry(self, e):
        n = self.number(e["card"])
        where = f"effects.json card {e['card']!r}"
        effect = None
        stacked = [[] for _ in range(5)]
        if "effects" in e or "effect" in e:
            if self.is_monster(n):
                spec = e.get("effects", {})
                if "effect" in e:
                    raise ValueError(f"{where}: a monster has \"effects\" (five slots), not \"effect\"")
                words = self.effect_row(self.number(spec["from"])) if "from" in spec else self.effect_row(n)
                if len(words) != 10:
                    raise ValueError(f"{where}: \"from\" must be a monster")
                for i, name in enumerate(SLOT_NAMES):
                    if name not in spec:
                        continue
                    given = spec[name]
                    # A list stacks effects: the first is the slot's own, the rest come on top.
                    items = given if isinstance(given, list) else [given]
                    if not items:
                        raise ValueError(f"{where} {name}: an empty list")
                    if len(items) - 1 > STACK_MAX:
                        raise ValueError(f"{where} {name}: at most {STACK_MAX + 1} effects in one slot")
                    if len(items) > 1 and name not in STACKABLE:
                        raise ValueError(f"{where} {name}: stacking works for {', '.join(STACKABLE)} for now")
                    words[2 * i], words[2 * i + 1] = self.slot(items[0], i, f"{where} {name}")
                    for k, item in enumerate(items[1:]):
                        if item == "none":
                            raise ValueError(f"{where} {name}: \"none\" only as the slot's own effect")
                        stacked[i].append(self.slot(item, i, f"{where} {name} #{k + 2}"))
                unknown = set(spec) - set(SLOT_NAMES) - {"from"}
                if unknown:
                    raise ValueError(f"{where}: unknown slots {sorted(unknown)} (slots: {', '.join(SLOT_NAMES)})")
            else:
                if "effects" in e:
                    raise ValueError(f"{where}: only monsters have \"effects\"; this card has one \"effect\"")
                w0, w1 = self.slot(e["effect"], 0, where)
                words = [w0, w1] + [NONE] * 8
            effect = words
        leader = None
        if "leader" in e:
            if not self.is_monster(n):
                raise ValueError(f"{where}: only monsters lead")
            leader = self.leader(e["leader"], n, where)
        return n, effect, leader, stacked


def generate(effects_path, cards_path, out_path, exe):
    fx = Effects(exe, cards_path)
    entries = json.loads(Path(effects_path).read_text()) if effects_path and Path(effects_path).is_file() else []
    rows, seen = [], set()
    for e in entries:
        n, effect, leader, stacked = fx.entry(e)
        if n in seen:
            raise ValueError(f"effects.json: card {n} given twice")
        seen.add(n)
        rows.append((n, effect, leader, stacked))
    if len(rows) > CARD_CAPACITY:
        raise ValueError(f"effects.json: at most {CARD_CAPACITY} cards (CARD_CAPACITY in tools/dlc_effects.py)")
    lines = ["/* Written by tools/dlc_effects.py from effects.json. */", '#include "dlc_effects.h"', "",
             f"const struct DlcEffectCard dlc_effect_cards[{CARD_CAPACITY}] = {{"]
    for n, effect, leader, stacked in rows:
        ew = effect or [NONE] * 10
        lw = leader or [NONE] * 10
        lines.append(f"    {{{n}, {1 if effect else 0}, {1 if leader else 0},")
        lines.append("     {" + ", ".join(f"{w:#010x}" for w in ew) + "},")
        lines.append("     {" + ", ".join(f"{w:#010x}" for w in lw) + "},")
        lines.append("     {" + ", ".join(str(len(s)) for s in stacked) + "},")
        slots = []
        for s in stacked:
            pairs = list(s) + [(NONE, NONE)] * (STACK_MAX - len(s))
            slots.append("{" + ", ".join(f"{{{a:#010x}, {b:#010x}}}" for a, b in pairs) + "}")
        lines.append("     {" + ", ".join(slots) + "}},")
    if not rows:
        lines.append("    {0, 0, 0, {0}, {0}, {0}, {{{0}}}},")
    lines += ["};", f"const int dlc_effect_count = {len(rows)};", ""]
    Path(out_path).write_text("\n".join(lines), newline="\n")
    return rows
