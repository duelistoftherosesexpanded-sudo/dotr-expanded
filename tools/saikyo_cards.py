#!/usr/bin/env python3
"""Adds Saikyo Card Battle's monsters that neither Duelists of the Roses nor the DLC has to
cards.json as new cards (numbers from FIRST on), each with its 3D model from saikyo/ (tools/
saikyo_models.py) and its clips.

A new card's donor is one of the game's monsters without an effect, of the card's type (the card
database's, mapped as tools/card_lookup.py maps them) and the nearest level, the least used first
so donors spread out. Text, level, attack, defense, frame and art are left to
tools/card_lookup.py --fill (the cards start with the placeholder's text and picture).

  saikyo_cards.py [--dry-run]        then: card_lookup.py --fill
"""
import argparse
import csv
import json
import re
import sys
from collections import Counter
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
CARDS = ROOT / "mods" / "widescreen" / "cards.json"
SAIKYO = ROOT / "saikyo"
FIRST = 1116
PICTURES_FIRST = 1360          # the card-picture monsters (--card-pictures), up to MY_DLC_FIRST
FONT_FIXES = {":": "", "/": "-", "’": "'", "☆": "", "★": ""}


def norm(s):
    return re.sub(r"[^a-z0-9]", "", s.lower())


def font_name(name):
    for a, b in FONT_FIXES.items():
        name = name.replace(a, b)
    return re.sub(r"\s+", " ", name).strip()


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--card-pictures", action="store_true",
                   help="the monsters the game shows as their card picture (3D/MBB_*.bch), from number "
                        "PICTURES_FIRST, without a model (tools/frame_cards.py gives them picture frames)")
    a = p.parse_args()
    import card_lookup
    import dlc_cards
    import dod_models
    cards = json.loads(CARDS.read_text(encoding="utf-8"))
    taken = {c["number"] for c in cards if "number" in c}
    have = {norm(k) for k in dod_models.dotr_names()} | {norm(k) for k in dod_models.dlc_names()}
    have |= {norm(c.get("name") or "") for c in cards if c.get("name")}
    if a.card_pictures:
        konami = {int(k): v["name"] for k, v in dod_models.konami_names().items()}
        table = json.loads((ROOT / "build" / "saikyo" / "model_table.json").read_text(encoding="utf-8"))
        rows = []
        for t in table:
            m = re.search(r"/MBB_(\d+)\.bch$", t["model"])
            if m and int(m.group(1)) in konami:
                rows.append(dict(name=konami[int(m.group(1))], dotr="", dlc="", folder="", model=""))
    else:
        rows = list(csv.DictReader(open(SAIKYO / "model_list.csv", encoding="utf-8")))
    new, seen = [], set()
    for r in rows:
        n = norm(r["name"])
        if r["dotr"] or r["dlc"] or n in have or n in seen or r["name"].startswith("unknown"):
            continue
        seen.add(n)
        new.append(r)
    # Donors: the game's effect-less monsters by type.
    exe = dlc_cards.game_exe()
    pool = []
    for k in range(683):
        rec = dlc_cards.record(exe, k)
        if rec["effect"] == 0xFFFF and k != 671:          # 0xffff: no effect
            pool.append((k, rec))
    used = Counter(c.get("donor") for c in cards if "donor" in c)
    match = card_lookup.Matcher(card_lookup.database())
    added, report = [], []
    number = PICTURES_FIRST if a.card_pictures else FIRST
    for r in new:
        while number in taken or number == dlc_cards.NONE:
            number += 1
        if number >= dlc_cards.MY_DLC_FIRST:
            report.append(f"no card numbers left for {r['name']} and later")
            break
        real, exact = match.find(r["name"])
        race = card_lookup.TYPE_MAP.get(real.get("race"), real.get("race")) if real else None
        level = int(real.get("level") or real.get("linkval") or 4) if real else 4
        mtype = dlc_cards.MONSTER_TYPES.index(race) if race in dlc_cards.MONSTER_TYPES else None
        same = [(k, rec) for k, rec in pool if mtype is None or rec["monster_type"] == mtype] or pool
        donor = min(same, key=lambda kr: (abs(kr[1]["level"] - level), used[kr[0]], kr[0]))[0]
        used[donor] += 1
        folder = SAIKYO / r["folder"]
        name = font_name(r["name"])
        entry = {"number": number, "donor": donor, "name": name,
                 "description": f"Placeholder card {number}.", "password": f"{number:08d}"}
        if not a.card_pictures:
            entry.update({"model": f"../../saikyo/{r['folder']}/{r['model']}.glb",
                          "model_texture": f"../../saikyo/{r['folder']}/{r['model']}_atlas.png",
                          "model_size": 1.0, "model_animated": True})
        if not a.card_pictures and not (folder / f"{r['model']}.glb").is_file():
            report.append(f"{name}: no model file, skipped")
            continue
        added.append(entry)
        taken.add(number)
        report.append(f"{number} {name}: donor {donor} ({race or '?'} level {level})"
                      + ("" if exact else f"  [database match: {real['name'] if real else 'none'}]"))
    print("\n".join(report))
    print(f"{len(added)} cards, numbers {added[0]['number']}-{added[-1]['number']}" if added else "nothing to add")
    if not a.dry_run and added:
        cards += added
        explicit = sorted((e for e in cards if "number" in e), key=lambda e: e["number"])
        rest = [e for e in cards if "number" not in e]
        CARDS.write_text(json.dumps(explicit + rest, indent=2, ensure_ascii=False) + "\n", encoding="utf-8", newline="\n")


if __name__ == "__main__":
    main()
