#!/usr/bin/env python3
"""Sizes for the new cards' own 3D models (cards.json "model_size"), taken from the game's monsters.

A new model is imported at model_size x its donor's height (tools/model_import.py,
tools/model_skinned.py: the standing-pose mesh, tools/model_research.py mesh). This measures every
game monster's model the same way (once; kept in launcher/model_heights.json) and gives each new
card with a model the size that makes it as tall as the game's monsters like it: the median height
of the monsters of its monster type within one level of it (if there are fewer than three, of its
type; failing that, of its level).

  python tools/model_sizes.py            the table: card, references, target height, model_size
  python tools/model_sizes.py --write    also write model_size into cards.json (a card that has
                                          "model_size_fixed": true keeps its own)
"""

import argparse
import json
import statistics
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import dlc_cards  # noqa: E402
import iso_files  # noqa: E402
import model_research as mr  # noqa: E402
import project  # noqa: E402
from dotr.elf import file_offset, inventory  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CARDS = ROOT / "mods" / "widescreen" / "cards.json"
CACHE = ROOT / "launcher" / "model_heights.json"
RECORDS = 0x2bf080
SLOT_SECTORS = 512   # a monster's slot of DATA/MONSTER.MRG: 1 MiB


def game_records():
    exe = dlc_cards.game_exe()
    meta = inventory(exe)
    out = {}
    for n in range(683):
        r = exe[file_offset(meta, RECORDS + 20 * n, 20):][:20]
        out[n] = dict(type=r[0] & 0x1f, level=r[2] >> 4)
    return exe, out


def measure(say=print):
    """{card: model height} of the game's monsters (cached)."""
    if CACHE.is_file():
        return {int(k): v for k, v in json.loads(CACHE.read_text()).items()}
    disc = iso_files.Image(project.find_iso(None), read_only=True)
    heights = {}
    try:
        _, _, entry = disc.lookup("/DATA/MONSTER.MRG")
        first = struct.unpack_from("<I", entry, 2)[0]
        for n in range(683):
            if n == 671:
                continue
            data = disc.read(first + n * SLOT_SECTORS, SLOT_SECTORS)
            try:
                ys = [v[1] for v in mr.mesh(data)[0]]
            except (ValueError, struct.error, IndexError, KeyError):
                continue
            if ys:
                heights[n] = max(ys) - min(ys)
            if n % 50 == 0:
                say(f"measured {n} / 683")
    finally:
        disc.close()
    CACHE.parent.mkdir(parents=True, exist_ok=True)
    CACHE.write_text(json.dumps(heights, indent=1))
    return heights


def suggestions():
    exe, records = game_records()
    heights = measure()
    # A few models measure far off (a stray node far from the body); they are left out, and a
    # card whose donor is one of them keeps its size.
    typical = statistics.median(heights.values())
    heights = {n: h for n, h in heights.items() if typical / 5 <= h <= typical * 5}
    text = dlc_cards.Text(exe)
    cards = dlc_cards.load_cards(CARDS, text)
    names = {v: k for k, v in dlc_cards.card_names(text).items()}
    rows = []
    for c in cards:
        if not c.get("model"):
            continue
        donor = c["donor"]
        kind, level = c["_monster_type"], c["_level"]
        if kind < 0:
            kind = records[donor]["type"]
        if level < 0:
            level = records[donor]["level"]
        pool = [n for n, r in records.items() if n in heights and r["type"] == kind and abs(r["level"] - level) <= 1]
        rule = "same type, level +-1"
        if len(pool) < 3:
            pool = [n for n, r in records.items() if n in heights and r["type"] == kind]
            rule = "same type"
        if len(pool) < 3:
            pool = [n for n, r in records.items() if n in heights and r["level"] == level]
            rule = "same level"
        target = statistics.median(heights[n] for n in pool)
        size = round(target / heights[donor], 2) if heights.get(donor) else None
        rows.append(dict(number=c["number"], name=c["name"], type=kind, level=level, donor=donor,
                         donor_height=round(heights.get(donor, 0)), target=round(target), rule=rule,
                         references=len(pool), examples=[names.get(n, str(n)) for n in pool[:4]],
                         current=c.get("model_size", 1.0), size=size))
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()
    rows = suggestions()
    for r in rows:
        print(f"{r['number']} {r['name'][:24]:24} type {r['type']:2} lv {r['level']:2}  donor {r['donor']:3} "
              f"h {r['donor_height']:6}  target {r['target']:6} ({r['rule']}, {r['references']})  "
              f"size {r['current']} -> {r['size']}")
    if args.write:
        entries = json.loads(CARDS.read_text())
        by_number = {r["number"]: r for r in rows}
        changed = 0
        for e in entries:
            r = by_number.get(e.get("number"))
            if r and r["size"] and not e.get("model_size_fixed") and e.get("model_size") != r["size"]:
                e["model_size"] = r["size"]
                changed += 1
        CARDS.write_text(json.dumps(entries, indent=2) + "\n")
        print(f"cards.json: {changed} model sizes written")


if __name__ == "__main__":
    main()
