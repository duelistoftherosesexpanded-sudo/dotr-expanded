#!/usr/bin/env python3
"""The mod's own achievements (dlc/achievements.c): mods/<mod>/achievements.json into C.

  {"achievements": [
     {"name": "FIRST VICTORY", "description": "WIN A DUEL", "goal": {"duels_won": 1},
      "reward": {"sleeve": "CLASSIC"}},
     {"name": "COLLECTOR", "description": "BUY 25 CARDS", "goal": {"cards_bought": 25},
      "reward": {"card": "Harpie's Feather Duster"}}]}

  name, description  shown in the pop-up and the shop: capital letters, digits and spaces (the
                     game's window font), at most 20 characters each.
  goal               one of: duels_won, duels (campaign duels played, won or lost), cards_bought
                     (in the card shop), bosses_beaten (the new podiums of bosses.json), cards_owned
                     (different cards in the chest and decks); and the number to reach.
  reward             optional: {"sleeve": a sleeve's name, as the shop shows it (its file name in
                     sleeves/, capitals)} or {"card": a card's name or number}. A sleeve named as a
                     reward is locked until its achievement is earned; the others are always open.

At most 60 achievements (two 30-bit words of the save, card_save.c).
"""

import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

MAX = 60
GOALS = ["duels_won", "duels", "cards_bought", "bosses_beaten", "cards_owned"]
TEXT = re.compile(r"^[A-Z0-9 ]{1,20}$")


def load(mod, exe=None):
    path = Path(mod) / "achievements.json"
    if not path.is_file():
        return []
    import dlc_cards
    import dlc_sleeves
    from runtime import ROOT
    exe = exe or dlc_cards.game_exe()
    names = dlc_cards.card_names(dlc_cards.Text(exe))
    cards_path = Path(mod) / "cards.json"
    if cards_path.is_file():
        for card in dlc_cards.read_entries(cards_path):
            if "number" in card and card.get("name"):
                names.setdefault(card["name"].strip().lower(), int(card["number"]))
    sleeves = ["ORIGINAL"] + [dlc_sleeves.sleeve_name(p) for p in dlc_sleeves.sleeves(mod)]
    entries = json.loads(path.read_text())["achievements"]
    if len(entries) > MAX:
        raise ValueError(f"achievements.json: at most {MAX} achievements")
    result = []
    for k, a in enumerate(entries):
        name, desc = str(a.get("name", "")), str(a.get("description", ""))
        for field, value in (("name", name), ("description", desc)):
            if not TEXT.match(value):
                raise ValueError(f"achievement {k}: {field} {value!r} must be 1-20 capital letters, digits "
                                 f"or spaces (the game's window font has no others)")
        goal = a.get("goal", {})
        if len(goal) != 1 or next(iter(goal)) not in GOALS:
            raise ValueError(f"achievement {name}: goal must be one of {', '.join(GOALS)} with a number")
        kind, count = next(iter(goal.items()))
        if not 1 <= int(count) <= 9999:
            raise ValueError(f"achievement {name}: the goal's number must be 1-9999")
        sleeve, card = -1, -1
        reward = a.get("reward") or {}
        if "sleeve" in reward:
            want = str(reward["sleeve"]).strip().upper()
            if want not in sleeves or want == "ORIGINAL":
                raise ValueError(f"achievement {name}: no sleeve {want!r} (there are {', '.join(sleeves[1:])})")
            sleeve = sleeves.index(want)
        text = ""
        if sleeve >= 0:
            text = "SLEEVE " + sleeves[sleeve]
        if "card" in reward:
            value = reward["card"]
            card = int(value) if str(value).strip().isdigit() else names.get(str(value).strip().lower())
            if card is None or not 0 <= card < dlc_cards.END or card in (671, 999):
                raise ValueError(f"achievement {name}: no card {value!r}")
            text = font_text(card_name(card, exe, cards_path))
        result.append(dict(name=name, desc=desc, kind=GOALS.index(kind), count=int(count), sleeve=sleeve, card=card,
                           reward=text[:32]))
    return result


def font_text(s):
    """s in what the window font has: capitals, digits and single spaces."""
    s = re.sub(r"[^A-Z0-9 ]", " ", s.upper().replace("'", ""))
    return re.sub(r" +", " ", s).strip()


def card_name(card, exe, cards_path):
    import dlc_cards
    if card < 854:
        return dlc_cards.Text(exe).decode(dlc_cards.NAMES + card)
    if Path(cards_path).is_file():
        for c in dlc_cards.read_entries(cards_path):
            if c.get("number") == card:
                return c.get("name", "")
    return f"CARD {card}"


def generate(mod, out_path, exe=None):
    achievements = load(mod, exe)
    rows = [f'    {{"{a["name"]}", "{a["desc"]}", {a["kind"]}, {a["count"]}, {a["sleeve"]}, {a["card"]}, '
            f'"{a["reward"]}"}},' for a in achievements]
    Path(out_path).write_text(
        "/* Written by tools/dlc_achievements.py from achievements.json (dlc/achievements.c). */\n"
        '#include "dlc_achievements.h"\n\n'
        # A fixed size, for the live region (tools/mod.py).
        f"const struct DlcAchievement dlc_achievements[{MAX}] = {{\n" + "\n".join(rows) + "\n    {0},\n};\n"
        f"const int dlc_achievement_count = {len(achievements)};\n", newline="\n")


if __name__ == "__main__":
    from runtime import ROOT
    for a in load(ROOT / "mods" / "widescreen"):
        print(a)
