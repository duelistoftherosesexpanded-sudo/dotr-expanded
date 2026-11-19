#!/usr/bin/env python3
"""Gives each new card of cards.json its era (cards.json "era"), from the year the card first came
out in English (the card database's TCG date; its Japanese date for a card never released in
English). The title screen's MOD OPTIONS chooses the newest era to play with (dlc/eras.c): a card
of a later era is hidden and cannot be obtained, and plays as its donor in duels.

  og       before 2004 (the game's own cards are all og)
  gx       2004-2007
  5ds      2008-2010
  zexal    2011-2013
  arc-v    2014-2016
  vrains   2017-2019
  modern   2020 and later (and cards the database lacks, such as Rush Duel cards)

A card whose era was chosen by hand in the Card Maker ("era_manual") keeps it.
The database with dates is kept in build/card_data/cardinfo_misc.json (fetched once; --refresh).

  card_eras.py [--dry-run] [--refresh]
"""
import argparse
import json
import sys
from collections import Counter
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
CARDS = ROOT / "mods" / "widescreen" / "cards.json"
DATA = ROOT / "build" / "card_data" / "cardinfo_misc.json"
ERAS = ["og", "gx", "5ds", "zexal", "arc-v", "vrains", "modern"]
STARTS = [0, 2004, 2008, 2011, 2014, 2017, 2020]     # the first year of each era


def era_of_year(year):
    return max(k for k, start in enumerate(STARTS) if year >= start)


def database(refresh=False):
    import card_lookup
    if refresh or not DATA.is_file():
        DATA.parent.mkdir(parents=True, exist_ok=True)
        DATA.write_bytes(card_lookup.fetch(card_lookup.API + "?misc=yes"))
    return json.loads(DATA.read_text(encoding="utf-8"))["data"]


def year_of(card):
    misc = (card.get("misc_info") or [{}])[0]
    date = misc.get("tcg_date") or misc.get("ocg_date")
    return int(date[:4]) if date else None


_MATCH = None


def era_for(name):
    """The era of a card by its name (as main() gives it), for the Card Maker."""
    global _MATCH
    import card_lookup
    if _MATCH is None:
        _MATCH = card_lookup.Matcher(database())
    real, _ = _MATCH.find(card_lookup.ALIASES.get(name.strip().lower(), name))
    year = year_of(real) if real and name.strip().lower() not in card_lookup.NOT_IN_DATABASE else None
    return ERAS[era_of_year(year)] if year is not None else ERAS[-1]


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--refresh", action="store_true")
    a = p.parse_args()
    import card_lookup
    import dlc_cards
    match = card_lookup.Matcher(database(a.refresh))
    cards = json.loads(CARDS.read_text(encoding="utf-8"))
    counts, unknown = Counter(), []
    for c in cards:
        if "number" not in c or c["number"] < dlc_cards.FIRST or not c.get("name"):
            continue
        if c.get("era_manual"):          # chosen by hand in the Card Maker
            counts[c.get("era", "og")] += 1
            continue
        name = c["name"]
        real, exact = match.find(card_lookup.ALIASES.get(name.strip().lower(), name))
        year = year_of(real) if real and name.strip().lower() not in card_lookup.NOT_IN_DATABASE else None
        if year is None:
            unknown.append(f"{c['number']} {name}")
            era = len(ERAS) - 1
        else:
            era = era_of_year(year)
        c["era"] = ERAS[era]
        counts[ERAS[era]] += 1
    print({e: counts[e] for e in ERAS})
    print(f"{len(unknown)} not in the database (modern):", unknown)
    if not a.dry_run:
        CARDS.write_text(json.dumps(cards, indent=2, ensure_ascii=False) + "\n", encoding="utf-8", newline="\n")


if __name__ == "__main__":
    main()
