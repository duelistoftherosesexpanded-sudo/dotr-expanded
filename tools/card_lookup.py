#!/usr/bin/env python3
"""Lines the cards of cards.json up with the real cards (the YGOPRODeck card database).

  frame        every monster, the game's and the new ones: an effect monster gets the effect frame,
               a normal monster the normal one, fusion purple, synchro white, xyz black (pendulum
               ones as their kind). Ritual, link and the rest are left as they are, and so is a
               frame set by hand to another colour (--keep).
  type, attribute   the new cards: the real card's (types the game does not have become the
               nearest it has, ATTRIBUTE_MAP / TYPE_MAP); --game-types does the game's cards too.
  name         a new card whose name is misspelt or not capitalised gets the real card's spelling
               (less any characters the game's font lacks); ALIASES are short names kept as they are.
  art          a new card still on the placeholder picture gets the real card's art (the cropped
               picture, saved as art/<name>.png).

The database (one file) is kept in build/card_data/ and fetched again only with --refresh; the art
is downloaded once per card. Nothing is written with --dry-run (a report only).

  --fill        a new card still on the placeholder's text gets the real card's text (wrapped to
                the deck builder's lines, in the font's characters), level, attack and defense
  --full-names  new cards get the real card's full name, short names too
  --game-frames the game's cards get the real card's frame even over one set by hand

  python tools/card_lookup.py [--dry-run] [--game-types] [--game-frames] [--full-names] [--fill] [--refresh]
"""

import argparse
import difflib
import io
import json
import re
import sys
import time
import urllib.request
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import dlc_cards  # noqa: E402

ROOT = TOOLS.parent
MOD = ROOT / "mods" / "widescreen"
CARDS = MOD / "cards.json"
DATA = ROOT / "build" / "card_data"
API = "https://db.ygoprodeck.com/api/v7/cardinfo.php"
ART_URL = "https://images.ygoprodeck.com/images/cards_cropped/{id}.jpg"
AGENT = "dotr-card-tools/1.0"
PLACEHOLDER_ART = "placeholder.png"

FRAMES = {"normal": "normal", "normal_pendulum": "normal", "effect": "effect", "effect_pendulum": "effect",
          "fusion": "purple", "fusion_pendulum": "purple", "synchro": "white", "synchro_pendulum": "white",
          "xyz": "black", "xyz_pendulum": "black", "ritual": "ritual", "ritual_pendulum": "ritual", "link": "link"}
# Frames this tool sets; a card set by hand to another (blue, red ...) keeps it.
OWN_FRAMES = set(FRAMES.values()) | {"automatic"}
TYPE_MAP = {"Divine-Beast": "Immortal", "Creator-God": "Immortal", "Psychic": "Spellcaster", "Wyrm": "Dragon",
            "Cyberse": "Machine", "Illusion": "Spellcaster"}
ATTRIBUTE_MAP = {"DIVINE": "Light"}
# Short names the new cards go by: the real card's full name (the card keeps its own name).
ALIASES = {"black luster": "Black Luster Soldier", "odd eyes": "Odd-Eyes Dragon",
           "vwxyz": "VWXYZ-Dragon Catapult Cannon", "burstinatrix": "Elemental HERO Burstinatrix",
           "chaos emperor dragon": "Chaos Emperor Dragon - Envoy of the End",
           # The game's own short names.
           "b. skull dragon": "Black Skull Dragon", "mask of shine ? dark": "Mask of Shine & Dark",
           "red-eyes b. dragon": "Red-Eyes Black Dragon", "meteor b. dragon": "Meteor Black Dragon",
           "oscillo hero ?2": "Oscillo Hero #2",
           # The game's old (OCG era) names of cards the TCG renamed.
           "air marmot of nefariousness": "Archfiend Marmot of Nefariousness", "dryad": "Doriado",
           "wicked mirror": "Archfiend Mirror", "swordsman from a foreign land": "Swordsman from a Distant Land",
           "saber slasher": "Sword Slasher", "stone d.": "Stone Dragon", "guardian of the sea": "Sentinel of the Seas",
           "frog the jam": "Slime Toad", "flame snake": "Flame Viper", "kaminarikozou": "Thunder Kid",
           "trakadon": "Trakodon", "larva of moth": "Larvae Moth", "rigras leever": "Rigorous Reaver",
           "spike clubber": "Giganto"}
# New cards the database lacks whose names are close to another card's: left alone (a near match
# would give them that card's name, text and art).
NOT_IN_DATABASE = {"hero of the yeast"}
SMALL_WORDS = {"the", "of", "a", "an", "and", "from", "in", "to", "with"}


LINE = 30        # description characters a line (the game's own go up to 32)
MAX_LINES = 10


def plain(text):
    """Text in the characters the game's font has: letters, digits, space and - . ' ! ,"""
    text = text.replace("\r", " ").replace("\n", " ").replace("&", " and ").replace(":", ",").replace(";", ",")
    text = text.replace("/", " or ").replace("?", "").replace("’", "'").replace("●", "")
    text = re.sub(r"[^A-Za-z0-9 \-.'!,]", "", text)
    return re.sub(r"\s+", " ", text).strip()


def full_name(real):
    return plain(real["name"])


def description(real):
    """The real card's text for the deck builder: wrapped to LINE characters, at most MAX_LINES
    lines, cut at the end of a sentence when it is longer."""
    # The real text's lines (fusion materials, then the effect) as sentences of their own.
    parts = [p.strip().replace(" + ", " and ") for p in real.get("desc", "").replace("\r", "").split("\n") if p.strip()]
    joined = " ".join(p if p[-1] in ".!?)" else p + "." for p in parts)
    words, lines, line = plain(joined).split(), [], ""
    for w in words:
        if line and len(line) + 1 + len(w) > LINE:
            lines.append(line)
            line = w
        else:
            line = f"{line} {w}".strip()
    if line:
        lines.append(line)
    if len(lines) > MAX_LINES:
        lines = lines[:MAX_LINES]
        whole = "\n".join(lines)
        end = whole.rfind(".")
        lines = (whole[:end + 1] if end > len(whole) // 3 else whole.rstrip(",") + "...").split("\n")
    return "\n".join(lines) or real["name"]


def respelled(name, real, text):
    """The name to give a card the user named: the real card's when the user's is a misspelling
    or not capitalised, without characters the game's font lacks; else None (keep the user's)."""
    if key(name) == key(real) and not any(w[:1].islower() for w in name.split() if w.lower() not in SMALL_WORDS):
        return None
    try:
        text.encode(real)
        return real
    except ValueError:
        pass
    # The real name less the characters the font lacks ("&", quotes): only to fix the user's
    # capitals, since dropping a character may lose a word the user wrote out ("and").
    plain = re.sub(r"\s+", " ", re.sub(r"[^A-Za-z0-9 \-.'!,]", "", real)).strip()
    if key(plain) != key(name):
        return None
    try:
        text.encode(plain)
    except ValueError:
        return None
    return plain if plain != name else None


def key(name):
    return re.sub(r"[^a-z0-9]", "", name.lower())


def fetch(url):
    request = urllib.request.Request(url, headers={"User-Agent": AGENT})
    with urllib.request.urlopen(request, timeout=60) as response:
        return response.read()


def database(refresh=False):
    DATA.mkdir(parents=True, exist_ok=True)
    path = DATA / "cardinfo.json"
    if refresh or not path.is_file():
        path.write_bytes(fetch(API))
    return json.loads(path.read_text(encoding="utf-8"))["data"]


class Matcher:
    def __init__(self, cards):
        self.by_key = {}
        for c in cards:
            self.by_key.setdefault(key(c["name"]), c)
        self.keys = list(self.by_key)

    def find(self, name):
        """(card, exact) or (None, False)."""
        k = key(name)
        if k in self.by_key:
            return self.by_key[k], True
        close = difflib.get_close_matches(k, self.keys, n=1, cutoff=0.86)
        return (self.by_key[close[0]], False) if close else (None, False)


def game_type(real):
    race = TYPE_MAP.get(real.get("race"), real.get("race"))
    return race if race in dlc_cards.MONSTER_TYPES else None


def game_attribute(real):
    a = ATTRIBUTE_MAP.get(real.get("attribute"), (real.get("attribute") or "").title())
    return a if a in dlc_cards.ATTRIBUTES else None


def save_art(real, name):
    """The real card's cropped art as art/<name>.png (at most 512 pixels a side)."""
    from PIL import Image
    slug = re.sub(r"[^a-z0-9]+", "_", name.lower()).strip("_")
    out = MOD / "art" / f"{slug}.png"
    data = fetch(ART_URL.format(id=real["id"]))
    image = Image.open(io.BytesIO(data)).convert("RGB")
    image.thumbnail((512, 512))
    out.parent.mkdir(exist_ok=True)
    image.save(out, "PNG")
    time.sleep(0.2)   # the database asks for few requests a second
    return out.relative_to(MOD).as_posix()


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--game-types", action="store_true", help="also give the game's cards the real type and attribute")
    parser.add_argument("--game-frames", action="store_true",
                        help="the game's cards get the real card's frame even when one was set by hand")
    parser.add_argument("--full-names", action="store_true",
                        help="new cards get the real card's full name, short names (ALIASES) too")
    parser.add_argument("--fill", action="store_true",
                        help="new cards still on the placeholder's text get the real card's text, level, attack, defense")
    parser.add_argument("--refresh", action="store_true")
    args = parser.parse_args()

    exe = dlc_cards.game_exe()
    text = dlc_cards.Text(exe)
    meta = dlc_cards.inventory(exe)
    match = Matcher(database(args.refresh))
    entries = json.loads(CARDS.read_text())
    by_number = {int(e["number"]): e for e in entries if "number" in e}
    report = {"frames": {}, "types": [], "names": [], "filled": [], "art": [], "unmatched": [], "kept": []}

    def entry(n):
        if n not in by_number:
            by_number[n] = {"number": n}
            entries.append(by_number[n])
        return by_number[n]

    def set_frame(n, real, label):
        frame = FRAMES.get(real.get("frameType"))
        if frame is None:
            return
        have = by_number.get(n, {}).get("frame")
        if have and have not in OWN_FRAMES and not (args.game_frames and n < dlc_cards.ORIGINAL):
            report["kept"].append(f"{n} {label}: {have} (set by hand; the real card is {real['frameType']})")
            return
        entry(n)["frame"] = frame
        report["frames"][frame] = report["frames"].get(frame, 0) + 1

    def line_up(n, real, label, card):
        changes = []
        t, a = game_type(real), game_attribute(real)
        if t and card.get("monster_type") != t:
            changes.append(f"type {card.get('monster_type')} -> {t}")
            entry(n)["monster_type"] = t
        if a and card.get("attribute") != a:
            changes.append(f"attribute {card.get('attribute')} -> {a}")
            entry(n)["attribute"] = a
        if changes:
            report["types"].append(f"{n} {label}: " + ", ".join(changes))

    # The game's cards.
    for n in range(dlc_cards.ORIGINAL):
        if n == 671:
            continue
        name = text.decode(dlc_cards.NAMES + n)
        r = dlc_cards.record(exe, n, meta)
        if not r["monster"]:
            continue
        alias = ALIASES.get(name.strip().lower())
        real, exact = match.find(alias or name)
        if real is None or "Monster" not in real.get("type", ""):
            report["unmatched"].append(f"{n} {name}")
            continue
        label = name if exact and not alias else f"{name} (= {real['name']})"
        set_frame(n, real, label)
        # The game's Immortals (a type of DotR's own, with rules of its own) keep it.
        if args.game_types and r["monster_type"] != dlc_cards.MONSTER_TYPES.index("Immortal"):
            game = {"monster_type": dlc_cards.MONSTER_TYPES[r["monster_type"]] if r["monster_type"] < 21 else None,
                    "attribute": dlc_cards.ATTRIBUTES[r["attribute"]] if r["attribute"] < 6 else None}
            game.update({k: v for k, v in by_number.get(n, {}).items() if k in game})
            line_up(n, real, label, game)

    # The new cards that have a name of their own.
    for n in sorted(k for k in by_number if k >= dlc_cards.FIRST):
        card = by_number[n]
        name = card.get("name", "")
        if not name or name.lower().startswith(("placeholder", "falsebound")) or name.strip().lower() in NOT_IN_DATABASE:
            continue
        alias = ALIASES.get(name.strip().lower())
        real, exact = match.find(alias or name)
        if real is None or "Monster" not in real.get("type", ""):
            report["unmatched"].append(f"{n} {name}")
            continue
        label = name if exact and not alias else f"{name} (= {real['name']})"
        set_frame(n, real, label)
        line_up(n, real, label, card)
        better = None if alias else respelled(name, real["name"], text)
        if args.full_names and full_name(real) != name:
            better = full_name(real)
        if better:
            report["names"].append(f"{n}: {name} -> {better}")
            card["name"] = better
        if args.fill and str(card.get("description", "")).startswith("Placeholder card"):
            # Still the placeholder's values: the real card's text, level, attack and defense.
            card["description"] = description(real)
            if 1 <= int(real.get("level") or 0) <= 12:
                card["level"] = int(real["level"])
            for field, value in (("attack", real.get("atk")), ("defense", real.get("def"))):
                if value is not None:
                    card[field] = max(0, min(8191, int(value)))
            report["filled"].append(f"{n} {card['name']}: level {card.get('level', '-')}, "
                                    f"{card.get('attack')} / {card.get('defense')}")
        if card.get("art") in (None, "", PLACEHOLDER_ART):
            if args.dry_run:
                report["art"].append(f"{n} {real['name']} (would download)")
            else:
                card["art"] = save_art(real, real["name"])
                report["art"].append(f"{n} {real['name']}: {card['art']}")

    if not args.dry_run:
        explicit = sorted((e for e in entries if "number" in e), key=lambda e: e["number"])
        rest = [e for e in entries if "number" not in e]
        CARDS.write_text(json.dumps(explicit + rest, indent=2) + "\n", newline="\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
