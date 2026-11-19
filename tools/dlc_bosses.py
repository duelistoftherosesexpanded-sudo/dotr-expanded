#!/usr/bin/env python3
"""Turn a mod's bosses.json (new opponents on the campaign map, for its DLC) into C for the DLC build
(tools/mod.py).

bosses.json is a list of locations, each a podium of its own beside the game's 22:

  [{"name": "Deck Master T", "opponent": 24, "x": 160, "z": 30, "look": 21,
    "appears": "always", "campaign": "both", "gate": true},
   {"name": "Yugi", "opponent": 19, "world": 2, "location": 0}]

  name       shown over the podium (letters, digits, space and - . ' ! ,; capitals look best)
  opponent   the duelist number the duel is against (0-31): it picks the computer player, its
             built-in deck (26 + number), the terrain and the opponent's name in the duel.
             0 Simon ... 21 Manawyddan, 22-25 the four Deck Masters (K, I, T, S).
  x, z       where on the map (the map debug overlay, L3, shows coordinates under the cursor)
  location   instead of x and z: stand where the game's location 0-21 is (and look like it,
             unless "look" says otherwise)
  look       which of the game's 22 locations the podium looks like (its base and building)
  appears    "always", or {"after": n}: once the game's location n has been beaten (world 1 only)
  campaign   "both", "lancaster" or "york" (the side picked at the start of the campaign; world 1)
  world      1 (the game's map, default) or 2 (the second world, reached through a gate)
  gate       true: the podium also takes you between the worlds (SQUARE); it is shown in both
  action     "duel" (default), "deck": X opens the deck builder instead of a duel (no opponent
             needed), or "shop": X opens the card shop (dlc/shop.c)
  deck       optional: the name of a decks.json deck the boss always plays (in either world), its
             deck cost added up from the cards, its AI the deck's "ai"; that deck is then left out
             of the second world's random pool. Without it: the opponent's built-in deck in the
             game's world, a deck from the pool in the second.

At most 45 locations (their won/played marks are kept in the save, two bits each, 15 to a word).
"""

import json
import sys
from pathlib import Path

MAX = 45
ALLOWED = set("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 -.'!,")


def load(path):
    bosses = json.loads(Path(path).read_text())
    if not isinstance(bosses, list) or len(bosses) > MAX:
        raise ValueError(f"bosses.json must be a list of at most {MAX} locations")
    result = []
    for k, b in enumerate(bosses):
        name = str(b.get("name", "")).strip()
        if not name or len(name) > 24 or set(name) - ALLOWED:
            raise ValueError(f"boss {k}: name must be 1-24 letters, digits, spaces or - . ' ! ,")
        action = {"duel": 0, "deck": 1, "shop": 2}.get(str(b.get("action", "duel")).lower())
        if action is None:
            raise ValueError(f"boss {k}: action must be duel, deck or shop")
        opponent = int(b.get("opponent", 0) if action else b["opponent"])
        if not 0 <= opponent <= 31:
            raise ValueError(f"boss {k}: opponent must be 0-31")
        at = int(b.get("location", -1))
        if not -1 <= at <= 21:
            raise ValueError(f"boss {k}: location must be one of the game's locations, 0-21")
        if at < 0 and ("x" not in b or "z" not in b):
            raise ValueError(f"boss {k}: give x and z, or a location")
        look = int(b.get("look", at if at >= 0 else 21))
        if not 0 <= look <= 21:
            raise ValueError(f"boss {k}: look must be one of the game's locations, 0-21")
        appears = b.get("appears", "always")
        after = -1
        if isinstance(appears, dict):
            after = int(appears.get("after", -1))
            if not 0 <= after <= 21:
                raise ValueError(f"boss {k}: appears.after must be a location 0-21")
        elif appears != "always":
            raise ValueError(f"boss {k}: appears must be \"always\" or {{\"after\": n}}")
        campaign = {"both": -1, "lancaster": 0, "york": 1}.get(str(b.get("campaign", "both")).lower())
        if campaign is None:
            raise ValueError(f"boss {k}: campaign must be both, lancaster or york")
        world = int(b.get("world", 1))
        if world not in (1, 2):
            raise ValueError(f"boss {k}: world must be 1 or 2")
        result.append(dict(name=name.upper(), opponent=opponent, x=float(b.get("x", 0)), z=float(b.get("z", 0)),
                           look=look, after=after, campaign=campaign, world=world, at=at,
                           gate=1 if b.get("gate") else 0, action=action, deck=b.get("deck")))
    return result


def deck_index(bosses, decks):
    """Each boss's own deck ("deck": a decks.json deck's name) as its index there, or -1."""
    names = {name.lower(): k for k, (name, *_rest) in enumerate(decks)}
    for b in bosses:
        if b["deck"] in (None, ""):
            b["deck"] = -1
            continue
        k = names.get(str(b["deck"]).strip().lower())
        if k is None:
            raise ValueError(f"bosses.json, {b['name']}: deck {b['deck']!r} is not a deck of decks.json")
        b["deck"] = k


DECK_MAX = 32       # decks.json: the table keeps room for this many (a fixed size for the live region)
DECK_CARDS = 40
COPIES = 3
# decks.json "ai": the duelist whose computer player plays the deck in the second world (a name here
# or a number 0-25). The number picks an entry of the game's AI table (g_aLogFunc), which SPEX
# rearranges; the duelist's own routine has plays written for his own kind of deck.
DUELISTS = {"seto": 1, "kaiba": 1, "weevil": 2, "rex": 3, "keith": 4, "ishtar": 5, "necromancer": 6,
            "darkness-ruler": 7, "labyrinth-ruler": 8, "pegasus": 9, "richard": 10, "tea": 11, "tristan": 12,
            "mai": 13, "mako": 14, "joey": 15, "shadi": 16, "jasper": 17, "bakura": 18, "yugi": 19,
            "manawyddan": 20, "custom duel": 25}
# A deck of random cards, and a deck without "ai": the AI SPEX gives Custom Duel's computer player
# (made to play any deck).
DEFAULT_AI = 25


def ai_number(value, where):
    if value is None:
        return DEFAULT_AI
    if isinstance(value, int) or str(value).strip().isdigit():
        n = int(value)
    else:
        n = DUELISTS.get(str(value).strip().lower())
        if n is None:
            raise ValueError(f"decks.json, {where}: ai {value!r} is not one of {', '.join(DUELISTS)} or 0-25")
    if not 0 <= n <= 25:
        raise ValueError(f"decks.json, {where}: ai must be 0-25")
    return n


def load_decks(path, exe=None):
    """decks.json next to bosses.json: [(name, leader, [40 card numbers])], names checked against the
    game's cards. [] when there is no such file."""
    path = Path(path).parent / "decks.json"
    if not path.is_file():
        return []
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import dlc_cards
    from runtime import ROOT
    exe = exe or dlc_cards.game_exe()
    names = dlc_cards.card_names(dlc_cards.Text(exe))

    def number(card, deck):
        if isinstance(card, int) or str(card).strip().isdigit():
            n = int(card)
        else:
            n = names.get(str(card).strip().lower())
            if n is None:
                raise ValueError(f"decks.json, {deck}: no card named {card!r}")
        if not 0 <= n < 854 or n == 671:
            raise ValueError(f"decks.json, {deck}: {card!r} is not one of the game's cards")
        return n

    decks = json.loads(path.read_text())["decks"]
    if len(decks) > DECK_MAX:
        raise ValueError(f"decks.json: at most {DECK_MAX} decks")
    result = []
    for d in decks:
        name = d.get("name", "?")
        leader = number(d["leader"], name)
        if leader >= 683:
            raise ValueError(f"decks.json, {name}: the leader must be a monster")
        cards = []
        for card, count in d["cards"].items():
            if not 1 <= int(count) <= COPIES:
                raise ValueError(f"decks.json, {name}: {card} x{count} (1-{COPIES} copies)")
            cards += [number(card, name)] * int(count)
        if len(cards) != DECK_CARDS:
            raise ValueError(f"decks.json, {name}: {len(cards)} cards, not {DECK_CARDS}")
        if cards.count(leader) + 1 > COPIES:
            raise ValueError(f"decks.json, {name}: the leader counts as a copy; at most {COPIES - 1} more")
        result.append((name, leader, cards, ai_number(d.get("ai"), name)))
    return result


def random_deck_ai(path):
    """decks.json "random_ai": the AI of a second-world boss with a deck of random cards."""
    path = Path(path).parent / "decks.json"
    if not path.is_file():
        return DEFAULT_AI
    return ai_number(json.loads(path.read_text()).get("random_ai"), "random_ai")


def generate(path, out):
    bosses = load(path)
    decks = load_decks(path)
    deck_index(bosses, decks)
    deck_rows = [f"    {{{leader}, {', '.join(map(str, cards))}}},  /* {name} */" for name, leader, cards, _ in decks]
    ais = [ai for *_, ai in decks] + [DEFAULT_AI] * (DECK_MAX - len(decks))
    rows = [f'    {{"{b["name"]}", {b["opponent"]}, {b["x"]:.2f}f, {b["z"]:.2f}f, {b["look"]}, {b["after"]}, '
            f'{b["campaign"]}, {b["world"]}, {b["at"]}, {b["gate"]}, {b["action"]}, {b["deck"]}}},' for b in bosses]
    Path(out).write_text(
        "/* Written by tools/dlc_bosses.py from bosses.json. */\n#include \"dlc_bosses.h\"\n\n"
        # A fixed size (the save keeps 45 bosses, dlc_bosses.h DLC_BOSS_WORDS), so the table stays
        # where it is in DLC.BIN when bosses.json changes (tools/mod.py's live region).
        "const struct DlcBoss dlc_bosses[46] = {\n" + "\n".join(rows) + "\n    {0},\n};\n"
        f"const int dlc_boss_count = {len(bosses)};\n"
        # The second world's decks (decks.json): leader, then 40 cards.
        f"const short dlc_boss_decks[{DECK_MAX}][{DECK_CARDS + 1}] = {{\n" + "\n".join(deck_rows)
        + ("\n" if deck_rows else "") + "    {0},\n};\n"
        f"const int dlc_boss_deck_count = {len(decks)};\n"
        # The computer player of each deck, and of a deck of random cards (bosses.c DlcBosses_Control).
        f"const signed char dlc_boss_deck_ai[{DECK_MAX}] = {{{', '.join(map(str, ais))}}};\n"
        f"const int dlc_random_deck_ai = {random_deck_ai(path)};\n", newline="\n")


if __name__ == "__main__":
    generate(sys.argv[1], sys.argv[2])
    print(Path(sys.argv[2]).read_text())
