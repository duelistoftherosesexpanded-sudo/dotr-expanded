#!/usr/bin/env python3
"""Turn a mod's cards.json (new cards for its DLC) into C for the DLC build (tools/mod.py).

cards.json is a list of cards:

  [{"number": 871, "donor": 132, "name": "Slifer the Sky Dragon",
    "description": "A dragon of the heavens.", "attack": 4000, "defense": 4000}]

  number       871-998 or 1000-1199. 854-870 are the alternate pictures of the special cards and
               999 means "no card", so new cards start at 871 and skip 999.
  donor        an existing card (number, or its English name) the new one copies: its record
               (type, attribute, level, deck cost, effect), and for now its 3D model,
               animations and sounds.
  name         shown wherever the card's name is.
  description  the deck builder's text; a newline ("\\n") starts a new line.
  attack, defense   optional; otherwise the donor's.
  art          optional: a PNG (relative to cards.json) for the card's picture, cropped to the
               square 128 x 128 picture; otherwise the donor's picture.  password     optional: 8 digits or letters that unlock the card in the deck builder.
  monster_type, attribute, level   optional, monsters only: a name (MONSTER_TYPES, ATTRIBUTES)
               or number, and 0-12; otherwise the donor's.
  deck_cost    optional, 0-255; otherwise the donor's.
  effect_from  optional: a card (number or name) whose effect the card takes instead of the
               donor's; the donor still gives the 3D model, animations and sounds.

  model        optional, monsters only: a 3D model of the card's own, an FBX (binary) or OBJ file
               relative to cards.json, imported as one solid piece on the donor's skeleton (it moves
               with the donor's animations). With:
    model_texture     a PNG for it (scaled to the donor's first texture, 256 colours)
    model_up, model_front   which of the file's axes point up and to the front ("+y", "+z")
    model_turn        true to turn it round if it faces backwards
    model_arms_down   degrees to lower the arms of an FBX stored in a T-pose (e.g. 65)
    model_size        height relative to the donor's model (1.0)
    model_hide_uv     boxes [left, top, right, bottom] of the texture (0-1, top down) whose
                      triangles are left out (a second body in the same mesh)
  A mesh with more triangles than the donor's slot holds is reduced until it fits.
    model_animated    true: the FBX's own skeleton, skin weights and animation clips are used
                      (tools/model_skinned.py) instead of one solid piece on the donor's skeleton
    model_clips       with it: {"motion slot": "clip letter"}, default model_skinned.CLIP_SLOTS
    model_joint_scale with it: false drops the joints' own rest scales (default true)
    model_nested_roots with it, HSD models: false leaves out the nested skeleton root correction
    model_fit         with it, HSD models: a tools/hsd_fit.py file moving pieces into place
  Otherwise the card uses its donor's 3D model.

The card type (monster, magic, ...) and the effect always come from the donor.

An entry numbered 0-853 changes one of the game's own cards instead (load_edits): the same fields
(no donor; the card is its own), each one given replacing the game's value, "art" its picture,
"model" its 3D model, "password": "" taking its password away.
tools/card_maker.py is a window for editing this file and building the disc image.

An entry {"placeholders": [first, last], "donor": ..., ...} stands for every number in that range
not used by another entry: "Placeholder <number>" with the password 00000<number> and the entry's
other fields (art, attack, defense, description).

Text is written in the game's own character codes, which are read from the disc's message table
(the most common code for each character in the existing names and texts).
"""

import hashlib
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dotr.elf import file_offset, inventory  # noqa: E402
from runtime import ROOT  # noqa: E402

TABLE, STRINGS, WORDS, NAMES = 0x2d19d0, 3073, 74252, 320
BLOB = TABLE + 4 * STRINGS
FIRST, END, NONE = 871, 1532, 999      # dlc_cards.h DLC_CARD_FIRST, DLC_CARD_LIMIT, DLC_CARD_NONE
PICTURE_FIRST, PICTURE_SIZE = 900, 0x4800
# A new card's status (dlc_cards.h DLC_STATUS_*), by index; dlc_card_status in DLC.BIN.
STATUSES = ["normal", "unobtainable", "disabled"]
STATUS_SYMBOL = "dlc_card_status"
# The eras (cards.json "era", tools/card_eras.py), in order; dlc_card_era in DLC.BIN.
ERAS = ["og", "gx", "5ds", "zexal", "arc-v", "vrains", "modern"]
# New cards' save room (card_save.c): 6 bits each in 132 records of 30 bits.
SAVE_CARDS = 132 * 30 // 6


# A developer's own DLC folder next to cards.json (the setup tool's developer build makes it):
# its cards.json, art and models, and dlc/*.c (tools/mod.py), kept apart from the project's own
# until they are merged in. Its cards take the numbers from MY_DLC_FIRST.
MY_DLC = "my-dlc"
MY_DLC_FIRST = 1449


def _in_my_dlc(value, folder):
    """value with its file paths (relative to the folder's cards.json) made relative to the mod's."""
    if isinstance(value, list):
        return [_in_my_dlc(v, folder) for v in value]
    if isinstance(value, dict):
        return {k: _in_my_dlc(v, folder) for k, v in value.items()}
    if isinstance(value, str) and value and (folder / value).exists() and not Path(value).is_absolute():
        return f"{MY_DLC}/{value}"
    return value


def read_entries(cards_path):
    """cards.json's entries, then those of the my-dlc folder's cards.json (if any). A card number
    both give is an error: the developer's cards never replace the project's."""
    cards_path = Path(cards_path)
    entries = json.loads(cards_path.read_text(encoding="utf-8"))
    folder = cards_path.parent / MY_DLC
    mine = folder / "cards.json"
    if mine.is_file():
        taken = {int(e["number"]) for e in entries if "number" in e}
        for e in json.loads(mine.read_text(encoding="utf-8")):
            if "number" in e and int(e["number"]) in taken:
                raise ValueError(f"{mine}: card number {e['number']} is already one of the project's cards "
                                 f"(new cards here start at {MY_DLC_FIRST})")
            entries.append({k: (v if k in ("number", "donor", "name", "description", "effect_from")
                                else _in_my_dlc(v, folder)) for k, v in e.items()})
    return entries


def numbers():
    """Every number a new card may have, in order."""
    return [n for n in range(FIRST, END) if n != NONE]


def listed_numbers(cards_path):
    """The new card numbers in use: from FIRST to the highest cards.json gives a card or a
    placeholder range (999 skipped)."""
    last = FIRST
    for e in read_entries(cards_path):
        if "placeholders" in e:
            last = max(last, int(e["placeholders"][1]))
        elif "number" in e and int(e["number"]) >= FIRST:
            last = max(last, int(e["number"]))
    return [n for n in numbers() if n <= last]
PASSWORD_SHIFTS = 0x2bef79  # passwordKeyHannibal: "HANNIBAL"
RECORDS = 0x2bf080           # 854 card records of 20 bytes

# Values of the card record (docs/GAME_DATA.md, "Enumerations").
CARD_TYPES = ["Monster", "Magic", "Power-up", "Trap (limited)", "Trap (full)", "Ritual"]
MONSTER_TYPES = ["Dragon", "Spellcaster", "Zombie", "Warrior", "Beast-Warrior", "Beast", "Winged Beast",
                 "Fiend", "Fairy", "Insect", "Dinosaur", "Reptile", "Fish", "Sea Serpent", "Machine",
                 "Thunder", "Aqua", "Pyro", "Rock", "Plant", "Immortal"]
ATTRIBUTES = ["Light", "Dark", "Fire", "Earth", "Water", "Wind"]


_GAME_EXE = {}


def game_exe(path=None, mod=None):
    """The game's executable as Duelists of the Roses Expanded plays it: out/ntsc-u/SLUS_205.15 with
    the mod's base patch applied (community.json's first main patch, SPEX, which dlc/modoptions.c
    always applies), so the card records, names and effects the tools read are the ones the game
    has. Without community.json, the executable as it is."""
    path = Path(path or ROOT / "out/ntsc-u/SLUS_205.15")
    mod = Path(mod or ROOT / "mods" / "widescreen")
    settings_path = mod / "community.json"
    key = (str(path), path.stat().st_mtime_ns,
           settings_path.stat().st_mtime_ns if settings_path.is_file() else 0)
    if key not in _GAME_EXE:
        exe = bytearray(path.read_bytes())
        mains = json.loads(settings_path.read_text()).get("main", []) if settings_path.is_file() else []
        if mains:
            import dlc_community
            from research import verified_elf
            iso_offset = verified_elf()[1]["iso_extent"]["offset"]
            for off, value in dlc_community.ppf_bytes(mod / mains[0]["ppf"]).items():
                if 0 <= off - iso_offset < len(exe):
                    exe[off - iso_offset] = value
        _GAME_EXE.clear()
        _GAME_EXE[key] = bytes(exe)
    return _GAME_EXE[key]


def record(exe, card, meta=None):
    """The game's 20-byte record of card 0-853, as a dict of its fields."""
    r = exe[file_offset(meta or inventory(exe), RECORDS + 20 * card, 20):][:20]
    ap, dp = struct.unpack_from("<HH", r, 8)
    return dict(card_type=r[0] >> 5, monster_type=r[0] & 0x1f, attribute=r[2] & 0xf, level=r[2] >> 4,
                deck_cost=r[3], effect=struct.unpack_from("<H", r, 4)[0], attack=ap & 0x1fff, defense=dp & 0x1fff,
                monster=card < 683)


def _choice(card, field, names, limit):
    """A field given as a name from `names` or a number below `limit`; -1 when absent."""
    if field not in card or card[field] is None:
        return -1
    value = card[field]
    if isinstance(value, str):
        lowered = [n.lower() for n in names]
        if value.strip().lower() not in lowered:
            raise ValueError(f"card {card['number']}: {field} {value!r} is not one of {', '.join(names)}")
        return lowered.index(value.strip().lower())
    if not 0 <= int(value) < limit:
        raise ValueError(f"card {card['number']}: {field} must be 0-{limit - 1}")
    return int(value)


def character(code):
    if 0x56 <= code < 0x70:
        return chr(ord("A") + code - 0x56)
    if 0x73 <= code < 0x8d:
        return chr(ord("a") + code - 0x73)
    if 0x8f <= code < 0x98:
        return chr(ord("1") + code - 0x8f)
    return {0x70: " ", 0x98: "0", 0xa2: "-", 0xa4: ".", 0x9f: "'", 0x99: "!", 0x02: ",",
            0x9b: "#", 0x9e: "&"}.get(code)   # "#" as in Rock Ogre Grotto #1, "&" as in Master & Expert


class Text:
    def __init__(self, exe):
        meta = inventory(exe)
        self.offsets = struct.unpack_from(f"<{STRINGS}i", exe, file_offset(meta, TABLE, 4 * STRINGS))
        self.blob = struct.unpack_from(f"<{WORDS}H", exe, file_offset(meta, BLOB, 2 * WORDS))
        self.newline = self.blob[self.offsets[0]] & 0x1fff
        self.literal = {}
        for at in range(WORDS):
            word = self.blob[at]
            if word & 0x4000 or (at and self.blob[at - 1] & 0x4000 and not self.blob[at - 1] & 0x8000):
                continue
            c = character(word & 0x1fff)
            if c:
                counts = self.literal.setdefault(c, {})
                counts[word & 0x3fff] = counts.get(word & 0x3fff, 0) + 1

    def decode(self, index):
        """String `index` as text."""
        return self._trace(self.offsets[index], None, 0)

    def _trace(self, at, length, depth):
        """Text from word `at`, as lSzMsg_FontTrace walks it: up to `length` words (None: to the
        end mark). A pair of words copies from another string: in the first, the length in words
        in bits 0-5 and the start word in bits 6-12; in the second, the string's number."""
        text, used = "", 0
        while 0 <= at < WORDS and depth < 8 and (length is None or used < length) and len(text) < 400:
            word = self.blob[at]
            if word & 0x4000:
                second = self.blob[at + 1]
                text += self._trace(self.offsets[second & 0x3fff] + ((word >> 6) & 0x7f), word & 0x3f, depth + 1)
                at, used = at + 2, used + 2
                if second & 0x8000:
                    break
                continue
            text += "\n" if (word & 0x1fff) == self.newline else (character(word & 0x1fff) or "?")
            at, used = at + 1, used + 1
            if word & 0x8000:
                break
        return text

    def encode(self, text):
        words = []
        for c in text:
            if c == "\n":
                words.append(self.newline)
            elif c in self.literal:
                words.append(max(self.literal[c], key=self.literal[c].get))
            else:
                raise ValueError(f"no character code known for {c!r} (in {text!r})")
        if not words:
            raise ValueError("empty text")
        words[-1] |= 0x8000
        return words


def card_names(text):
    names = {}
    for card in range(854):
        name = text.decode(NAMES + card)
        if name:
            names.setdefault(name.strip().lower(), card)
    return names


def load_cards(cards_path, text):
    """The cards of cards.json, placeholder ranges expanded, donors as numbers, checked."""
    entries = read_entries(cards_path)
    names = card_names(text)
    cards, seen = [], set()
    explicit = {int(e["number"]) for e in entries if "number" in e}
    for entry in entries:
        if is_edit(entry):
            continue                      # a change to one of the game's cards (load_edits)
        if "placeholders" in entry:
            first, last = entry["placeholders"]
            for number in range(int(first), int(last) + 1):
                if number in explicit or number == NONE:
                    continue
                card = {k: v for k, v in entry.items() if k != "placeholders"}
                card.update(number=number, name=f"Placeholder {number}", password=f"{number:08d}")
                card.setdefault("description", f"Placeholder card {number}.\nTo be replaced.")
                cards.append(card)
        else:
            cards.append(dict(entry))
    model_count = 0
    for card in cards:
        number = int(card["number"])
        if not FIRST <= number < END or number == NONE or number in seen:
            raise ValueError(f"card number {number}: must be {FIRST}-{NONE - 1} or {NONE + 1}-{END - 1} and used once")
        seen.add(number)
        donor = card["donor"]
        if isinstance(donor, str):
            if donor.strip().lower() not in names:
                raise ValueError(f"no card named {donor!r}")
            donor = names[donor.strip().lower()]
        donor = int(donor)
        if not 0 <= donor < 854 or donor == 671:
            raise ValueError(f"donor {donor} is not an original card")
        card["donor"] = donor
        for field in ("attack", "defense"):
            if field in card and not 0 <= int(card[field]) <= 8191:
                raise ValueError(f"{field} must be 0-8191")
        password = str(card.get("password", "")).lower()
        if password and (len(password) != 8 or not all(c.isdigit() or "a" <= c <= "z" for c in password)):
            raise ValueError(f"card {number}: a password is 8 digits or letters")
        card["password"] = password
        card["_monster_type"] = _choice(card, "monster_type", MONSTER_TYPES, 21)
        card["_attribute"] = _choice(card, "attribute", ATTRIBUTES, 6)
        card["_level"] = _choice(card, "level", [], 13)
        card["_deck_cost"] = _choice(card, "deck_cost", [], 256)
        if donor >= 683 and (card["_monster_type"] >= 0 or card["_attribute"] >= 0 or card["_level"] >= 0):
            raise ValueError(f"card {number}: monster type, attribute and level are for monsters; "
                             f"the donor {donor} is not one")
        card["_effect_from"] = -1
        if card.get("effect_from") not in (None, ""):
            source = card["effect_from"]
            source = names.get(source.strip().lower()) if isinstance(source, str) else int(source)
            if source is None or not 0 <= source < 854 or source == 671:
                raise ValueError(f"card {number}: effect_from {card['effect_from']!r} is not one of the game's cards")
            if (source < 683) != (donor < 683):
                raise ValueError(f"card {number}: effect_from must be a monster when the donor is (and not otherwise)")
            card["_effect_from"] = source
        card["_model"] = -1
        if card.get("model"):
            if donor >= 683:
                raise ValueError(f"card {number}: a 3D model is for monsters; the donor {donor} is not one")
            for field in ("model", "model_texture"):
                if card.get(field) and not card.get("model_missing") and not (Path(cards_path).parent / card[field]).is_file():
                    raise ValueError(f"card {number}: {field} {card[field]} is not a file")
            card["_model"] = model_count
            model_count += 1
    if len(numbers()) > SAVE_CARDS:
        raise ValueError(f"the save keeps {SAVE_CARDS} new cards; card numbers {FIRST}-{END - 1} are more")
    return cards


ORIGINAL = 854         # the game's own cards: 0-853 (671 is not one)
DESCRIPTIONS = 1174    # card n's description is string 1174 + n (its name 320 + n)
EDIT_MAX = ORIGINAL


def is_edit(entry):
    """A cards.json entry for one of the game's own cards (0-853): changes to it, not a new card."""
    return "number" in entry and "placeholders" not in entry and int(entry["number"]) < ORIGINAL


def load_edits(cards_path, text, model_start=0):
    """The changes cards.json makes to the game's own cards, checked: the same fields as a new card
    has (the card is its own donor). A name or description left as the game's is dropped (the
    game's text stays). Own models are numbered on from model_start (after the new cards')."""
    entries = read_entries(cards_path)
    names = card_names(text)
    edits, seen, model_count = [], set(), model_start
    for entry in entries:
        if not is_edit(entry):
            continue
        card = dict(entry)
        number = int(card["number"])
        if number < 0 or number == 671 or number in seen:
            raise ValueError(f"card number {number}: not one of the game's cards, or listed twice")
        seen.add(number)
        card["donor"] = number
        monster = number < 683
        for field in ("attack", "defense"):
            if field in card and not 0 <= int(card[field]) <= 8191:
                raise ValueError(f"card {number}: {field} must be 0-8191")
        if card.get("name") and card["name"] == text.decode(NAMES + number):
            del card["name"]
        if card.get("description") and card["description"] == text.decode(DESCRIPTIONS + number):
            del card["description"]
        if "password" in card:
            password = str(card["password"]).lower()
            if password and (len(password) != 8 or not all(c.isdigit() or "a" <= c <= "z" for c in password)):
                raise ValueError(f"card {number}: a password is 8 digits or letters")
            card["password"] = password
        card["_monster_type"] = _choice(card, "monster_type", MONSTER_TYPES, 21)
        card["_attribute"] = _choice(card, "attribute", ATTRIBUTES, 6)
        card["_level"] = _choice(card, "level", [], 13)
        card["_deck_cost"] = _choice(card, "deck_cost", [], 256)
        if not monster and (card["_monster_type"] >= 0 or card["_attribute"] >= 0 or card["_level"] >= 0):
            raise ValueError(f"card {number}: monster type, attribute and level are for monsters")
        card["_effect_from"] = -1
        if card.get("effect_from") not in (None, ""):
            source = card["effect_from"]
            source = names.get(source.strip().lower()) if isinstance(source, str) else int(source)
            if source is None or not 0 <= source < ORIGINAL or source == 671:
                raise ValueError(f"card {number}: effect_from {card['effect_from']!r} is not one of the game's cards")
            if (source < 683) != monster:
                raise ValueError(f"card {number}: effect_from must be a monster when the card is (and not otherwise)")
            card["_effect_from"] = source
        card["_model"] = -1
        if card.get("model"):
            if not monster:
                raise ValueError(f"card {number}: a 3D model is for monsters")
            for field in ("model", "model_texture"):
                if card.get(field) and not card.get("model_missing") and not (Path(cards_path).parent / card[field]).is_file():
                    raise ValueError(f"card {number}: {field} {card[field]} is not a file")
            card["_model"] = model_count
            model_count += 1
        edits.append(card)
    edits.sort(key=lambda c: c["number"])
    return edits


def new_model_count(cards):
    return sum(1 for c in cards if c.get("model"))


def game_password(exe, number, meta=None):
    """The password of one of the game's cards as the deck builder takes it, or "" (none)."""
    meta = meta or inventory(exe)
    r = exe[file_offset(meta, RECORDS + 20 * number, 20):][:20]
    if not r[9] & 0x40:
        return ""
    shifts = struct.unpack_from("<8b", exe, file_offset(meta, PASSWORD_SHIFTS, 8))
    return "".join(chr((r[12 + i] + shifts[(number + i) % 8]) % 256) for i in range(8))


def password_code(number, password, shifts):
    """The 8 bytes a card record keeps for a password: lSzDuel_HitPassword adds the shift
    passwordKeyHannibal[(card + i) % 8] to byte i and compares with the typed character."""
    return [(ord(c) - shifts[(number + i) % 8]) % 256 for i, c in enumerate(password)]


def generate(cards_path, out_path, exe_path=None):
    exe = game_exe(exe_path)
    text = Text(exe)
    meta = inventory(exe)
    shifts = struct.unpack_from("<8b", exe, file_offset(meta, PASSWORD_SHIFTS, 8))
    cards = load_cards(cards_path, text)
    statuses = {c["number"]: str(c.get("status", "normal")).lower() for c in cards}
    eras = {}
    for c in cards:
        era = str(c.get("era", "og")).lower()
        if era not in ERAS:
            raise ValueError(f"card {c['number']}: era {era!r} is not one of {', '.join(ERAS)}")
        eras[c["number"]] = ERAS.index(era)
    for n, s in statuses.items():
        if s not in STATUSES:
            raise ValueError(f"card {n}: status {s!r} is not one of {', '.join(STATUSES)}")
    rows, arrays = [], []
    for k, card in enumerate(cards):
        number = card["number"]
        name_words = text.encode(card["name"])
        desc_words = text.encode(card.get("description", card["name"]))
        # Not const: the text goes to .data, apart from the fixed-size table in .rodata, so the
        # table stays where it is in DLC.BIN when names change (tools/mod.py's live region).
        arrays.append(f"static unsigned short name_{k}[] = {{{', '.join(f'{w:#x}' for w in name_words)}}};")
        arrays.append(f"static unsigned short desc_{k}[] = {{{', '.join(f'{w:#x}' for w in desc_words)}}};")
        code = password_code(number, card["password"], shifts) if card["password"] else [0] * 8
        rows.append(f"    {{{number}, {card['donor']}, {int(card.get('attack', -1))}, {int(card.get('defense', -1))}, "
                    f"{picture_first(cards_path) + k}, {card['_monster_type']}, {card['_attribute']}, {card['_level']}, "
                    f"{card['_deck_cost']}, {card['_model']}, {card['_effect_from']}, {1 if card['password'] else 0}, "
                    f"{{{', '.join(map(str, code))}}}, "
                    f"name_{k}, desc_{k}}},  /* {card['name']} */")
    # Changes to the game's own cards (cards.c applies them over the game's records and text).
    edit_rows = []
    for k, card in enumerate(load_edits(cards_path, text, new_model_count(cards))):
        number = card["number"]
        name, desc = "0", "0"
        if card.get("name"):
            arrays.append(f"static unsigned short edit_name_{k}[] = {{{', '.join(f'{w:#x}' for w in text.encode(card['name']))}}};")
            name = f"edit_name_{k}"
        if card.get("description"):
            arrays.append(f"static unsigned short edit_desc_{k}[] = "
                          f"{{{', '.join(f'{w:#x}' for w in text.encode(card['description']))}}};")
            desc = f"edit_desc_{k}"
        # has_password: 0 keeps the game's, 1 gives this one, 2 none.
        has, code = 0, [0] * 8
        if "password" in card:
            has = 1 if card["password"] else 2
            if card["password"]:
                code = password_code(number, card["password"], shifts)
        edit_rows.append(f"    {{{number}, {number}, {int(card.get('attack', -1))}, {int(card.get('defense', -1))}, -1, "
                         f"{card['_monster_type']}, {card['_attribute']}, {card['_level']}, {card['_deck_cost']}, "
                         f"{card['_model']}, {card['_effect_from']}, {has}, {{{', '.join(map(str, code))}}}, "
                         f"{name}, {desc}}},  /* {card.get('name') or text.decode(NAMES + number)} */")
    wallpaper = wallpaper_size(cards_path)
    width = 0
    colours = [0] * 8
    if wallpaper:
        # One tile is a half of the picture, 224 screen units high; a screen unit is 3 pixels wide
        # and 1080 / 448 high on a 16:9 picture, so this width keeps the picture's shape.
        w, h = wallpaper
        width = max(16, round(224 * (w / (h / 2)) * (1080 / 448) / 3))
        # The background while it loads: its average colour, darker and lighter (b, g, r, a with
        # 128 for full intensity, as the game's colours are).
        import model_texture as mt
        _, _, pixels = mt.read_png(wallpaper_path(cards_path))
        n = sum(len(r) for r in pixels)
        average = [sum(p[c] for r in pixels for p in r) / n for c in range(3)]
        for k, factor in enumerate((0.4, 0.55)):
            r, g, b = (min(255, int(v * factor)) for v in average)
            colours[4 * k:4 * k + 4] = [b, g, r, 128]
    Path(out_path).write_text(
        "/* Written by tools/dlc_cards.py from cards.json. */\n#include \"dlc_cards.h\"\n\n"
        + "\n".join(arrays) + f"\n\nconst struct DlcCardDef dlc_cards[{len(cards) + 1}] = {{\n" + "\n".join(rows)
        + "\n    {0},\n};\n" + f"const int dlc_card_count = {len(cards)};\n"
        # Each new card's status (dlc_cards.h): cards.json "status" ("unobtainable", "disabled"),
        # which the player's setup tool rewrites in DLC.BIN (STATUS_SYMBOL, tools/release.py).
        + f"const unsigned char dlc_card_status[{END}] = {{"
        + ", ".join(str(STATUSES.index(statuses.get(n, "normal"))) for n in range(END)) + "};\n"
        # Each new card's era (cards.json "era", tools/card_eras.py; 0 og ... 6 modern): the
        # title's MOD OPTIONS hides cards newer than the era chosen (dlc/eras.c).
        + f"const unsigned char dlc_card_era[{END}] = {{"
        + ", ".join(str(eras.get(n, 0)) for n in range(END)) + "};\n"
        + f"const struct DlcCardDef dlc_card_edits[{EDIT_MAX + 1}] = {{\n" + "".join(r + "\n" for r in edit_rows)
        + "    {0},\n};\n" + f"const int dlc_card_edit_count = {len(edit_rows)};\n"
        + f"const int dlc_wallpaper = {1 if wallpaper else 0};\nconst int dlc_wallpaper_width = {width};\n"
        + f"const unsigned char dlc_wallpaper_colours[8] = {{{', '.join(map(str, colours))}}};\n"
        # The memory card's "Saving..." messages (2516-2518) as one short line (cards.c).
        + "unsigned short dlc_text_saving[] = {" + ", ".join(f"{w:#x}" for w in text.encode("Saving......")) + "};\n"
        # The duel settings' SOUND row as AI SPEED (cards.c, dlc/aispeed.c): its name, its two
        # choices (122 the left, set; 121 the right, clear) and the tutorial's line about it.
        + "".join(f"unsigned short dlc_text_{name}[] = {{" + ", ".join(f"{w:#x}" for w in text.encode(words)) + "};\n"
                  for name, words in (("ai_speed", "AI Speed"), ("ai_normal", "Normal"), ("ai_fast", "Fast"),
                                      ("ai_help", "The AI Speed selection lets the\ncomputer play its turns twice as fast."))),
        newline="\n")


def picture_first(cards_path):
    """The first new card's picture: the deck builder's wallpaper, when there is one, comes first
    (pictures 900 and 901), then the cards' in cards.json order."""
    return PICTURE_FIRST + (2 if wallpaper_path(cards_path) else 0)


def wallpaper_path(cards_path):
    """The deck builder's wallpaper, deck_wallpaper.png next to cards.json, or None."""
    path = Path(cards_path).parent / "deck_wallpaper.png"
    return path if path.is_file() else None


def wallpaper_size(cards_path):
    import model_texture as mt
    path = wallpaper_path(cards_path)
    if path is None:
        return None
    width, height, _ = mt.read_png(path)
    return width, height


# A new card's model slot in DLC.BIN: 2 MiB, twice the game's, which the DLC loads into 2 MiB areas
# of its own (card_models.c DlcModel_Area). The donor's 1 MiB slot is copied to its start; the rest
# is free room for the mesh.
MODEL_SLOT = 0x200000


TOOL_SOURCES = ("dlc_cards.py", "model_import.py", "model_skinned.py", "model_texture.py", "card_edit.py")
# What a model slot depends on: only the model tools (not this file's other parts).
MODEL_SOURCES = ("model_import.py", "model_skinned.py", "model_texture.py")


def digest(*parts, tools=TOOL_SOURCES):
    """A key for cached results: the parts (files by their bytes, the rest as JSON) and the tools'
    own sources, so a change to either makes a new key."""
    h = hashlib.sha256()
    for p in list(parts) + [Path(__file__).parent / name for name in tools]:
        if isinstance(p, Path):
            h.update(p.read_bytes() if p.is_file() else b"-")
        else:
            h.update(json.dumps(p, sort_keys=True, default=str).encode())
        h.update(b"\0")
    return h.hexdigest()[:32]


def cached(cache, name, key, compute):
    """compute()'s bytes, kept as cache/<name>-<key>.bin (older ones of the same name removed)."""
    if cache is None:
        return compute()
    cache = Path(cache)
    cache.mkdir(parents=True, exist_ok=True)
    path = cache / f"{name}-{key}.bin"
    if path.is_file():
        return path.read_bytes()
    data = compute()
    for old in cache.glob(f"{name}-*.bin"):
        old.unlink()
    path.write_bytes(data)
    return data


def models(cards_path, read_slot, cache=None, without=()):
    """The 2 MiB model slots of the cards with a "model", in order: each made from its donor's slot
    (read_slot(card) gives the game's slot), with the donor's skeleton, animations and draw list and
    the new mesh and texture (tools/model_import.py, using the free end of the slot when needed).
    cache: a folder keeping each slot, made again only when its card's model settings or files
    change (a slot takes seconds to make)."""
    import model_import
    exe = game_exe()
    result, reports = [], []
    text = Text(exe)
    cards = load_cards(cards_path, text)
    # The new cards' models first, then those of the game's cards given one (load_edits numbers
    # them on from the new cards').
    for card in cards + load_edits(cards_path, text, new_model_count(cards)):
        if not card.get("model"):
            continue
        if card["number"] in without or card.get("model_missing"):
            # The card keeps its slot (the DLC numbers the slots in this order) but plays its
            # donor's model: the model's files are not there (the player's setup tool).
            result.append(read_slot(card["donor"]).ljust(MODEL_SLOT, b"\0"))
            reports.append(dict(card=card["number"], donor_model=True))
            continue
        base = Path(cards_path).parent
        if cache is not None:
            settings = {k: v for k, v in card.items() if k.startswith("model") or k == "donor"}
            key = digest(settings, *(base / card[f] for f in ("model", "model_texture", "model_fit") if card.get(f)),
                         tools=MODEL_SOURCES)
            name = f"model-{card['number']}"
            report_path = Path(cache) / f"{name}-{key}.json"
            if (Path(cache) / f"{name}-{key}.bin").is_file() and report_path.is_file():
                result.append(cached(cache, name, key, None))
                reports.append(json.loads(report_path.read_text()))
                continue
        donor = read_slot(card["donor"]).ljust(MODEL_SLOT, b"\0")
        donor = read_slot(card["donor"]).ljust(MODEL_SLOT, b"\0")
        texture = base / card["model_texture"] if card.get("model_texture") else None
        if card.get("model_animated"):
            # The FBX's own skeleton, skin and animation clips (tools/model_skinned.py).
            import model_skinned
            clips = {int(k): v for k, v in card["model_clips"].items()} if card.get("model_clips") else None
            slot, report = model_skinned.build(donor, base / card["model"], texture,
                                               fill=float(card.get("model_size", 1.0)), clip_slots=clips,
                                               joint_scale=card.get("model_joint_scale", True),
                                               nested_roots=card.get("model_nested_roots", True),
                                               fit=base / card["model_fit"] if card.get("model_fit") else None)
        else:
            slot, report = model_import.import_slot(
                donor, base / card["model"], texture,
                up=card.get("model_up", "+y"), front=card.get("model_front", "+z"), turn=bool(card.get("model_turn")),
                flip_v=not card.get("model_keep_v"), fill=float(card.get("model_size", 1.0)), grow=True,
                arms_down=float(card.get("model_arms_down", 0)), hide_uv=card.get("model_hide_uv"))
        if len(slot) != MODEL_SLOT:
            raise ValueError(f"card {card['number']}: the model slot is not {MODEL_SLOT >> 20} MiB")
        result.append(slot)
        reports.append(dict(card=card["number"], **{k: report[k] for k in ("triangles", "strip_bytes", "room", "parts",
                                                                            "bones", "motions") if k in report}))
        if cache is not None:
            cached(cache, name, key, lambda slot=slot: bytes(slot))
            for old in Path(cache).glob(f"{name}-*.json"):
                old.unlink()
            report_path.write_text(json.dumps(reports[-1], default=str))
    return result, reports


def pictures(cards_path, picture_mrg):
    """DATA/PICTURE.MRG extended with one record per new card from DLC_PICTURE_FIRST: the card's
    "art" (a PNG, relative to cards.json) encoded over a copy of the donor's record, else the
    donor's record as it is. Records between the game's and the new ones are left empty."""
    import model_texture as mt
    from card_edit import fit
    exe = game_exe()
    cards = load_cards(cards_path, Text(exe))
    data = bytearray(picture_mrg)
    if len(data) < PICTURE_FIRST * PICTURE_SIZE:
        data += bytes(PICTURE_FIRST * PICTURE_SIZE - len(data))
    del data[PICTURE_FIRST * PICTURE_SIZE:]
    # The deck builder's wallpaper first: its top and bottom halves as two pictures.
    wallpaper = wallpaper_path(cards_path)
    if wallpaper:
        width, height, rows = mt.read_png(wallpaper)
        half = height // 2
        for part in (rows[:half], rows[half:2 * half]):
            record = bytearray(picture_mrg[:PICTURE_SIZE])
            t = mt.info(record, 0)
            mt.encode(record, 0, mt.resize(width, half, part, t["width"], t["height"]))
            data += record
    made = {}
    for card in cards:
        donor, art = card["donor"], card.get("art")
        if (donor, art) not in made:
            record = bytearray(picture_mrg[donor * PICTURE_SIZE:(donor + 1) * PICTURE_SIZE])
            if record[:4] != b"RH2\0":
                raise ValueError(f"the donor {donor} has no picture record")
            if art:
                width, height, rows = mt.read_png(Path(cards_path).parent / art)
                t = mt.info(record, 0)
                mt.encode(record, 0, fit(rows, width, height, t["width"], t["height"]))
            made[(donor, art)] = bytes(record)
        data += made[(donor, art)]
    # The game's cards given art of their own: their own records (new cards copied the originals).
    for card in load_edits(cards_path, Text(exe)):
        replace_art(data, card["number"] * PICTURE_SIZE, PICTURE_SIZE, Path(cards_path).parent, card.get("art"))
    return bytes(data)


def replace_art(data, at, size, base, art):
    """The picture record at data[at:at + size] encoded again with the PNG `art` (when given)."""
    import model_texture as mt
    from card_edit import fit
    if not art or data[at:at + 4] != b"RH2\0":
        return
    record = bytearray(data[at:at + size])
    width, height, rows = mt.read_png(Path(base) / art)
    t = mt.info(record, 0)
    mt.encode(record, 0, fit(rows, width, height, t["width"], t["height"]))
    data[at:at + size] = record


PICPACK_TABLE, PICPACK_SLOTS, PICPACK_SIZE = 0x2ce9fc, 223, 0x4410


def picpack(cards_path, picpack_mrg, picture_mrg):
    """DATA/PICPACK.MRG with the pack slots of the game's cards given art of their own replaced: a
    slot (listed at 0x2ce9fc) is the start of the card's picture record, so it gets the start of
    the new one; a slot that is not a copy of the picture is left alone (as tools/card_edit.py)."""
    exe = game_exe()
    edits = [c for c in load_edits(cards_path, Text(exe)) if c.get("art")]
    data = bytearray(picpack_mrg)
    if not edits:
        return bytes(data)
    slots = struct.unpack(f"<{PICPACK_SLOTS}H", exe[file_offset(inventory(exe), PICPACK_TABLE, 2 * PICPACK_SLOTS):]
                          [:2 * PICPACK_SLOTS])
    for card in edits:
        n = card["number"]
        clean = picture_mrg[n * PICTURE_SIZE:(n + 1) * PICTURE_SIZE]
        record = bytearray(clean)
        replace_art(record, 0, PICTURE_SIZE, Path(cards_path).parent, card["art"])
        for slot, owner in enumerate(slots):
            at = slot * PICPACK_SIZE
            if owner == n and data[at:at + PICPACK_SIZE] == clean[:PICPACK_SIZE]:
                data[at:at + PICPACK_SIZE] = record[:PICPACK_SIZE]
    return bytes(data)


MINI_SIZE = 0x1000
MINI_EXCHANGED = 0xab   # SzText_LoadMerge modes 5-9: an exchanged picture p is read at (p - 0xab) << 12


def minis(cards_path, picmini_mrg):
    """DATA/PICMINI.MRG (the 40 x 32 leader portraits) extended so each new card's picture number p
    has a record at (p - 0xab): the card's "art" over a copy of the donor's record, else the
    donor's as it is. The duel's leader panes read it there (SzText_LoadMerge modes 5-9)."""
    import model_texture as mt
    from card_edit import fit
    exe = game_exe()
    cards = load_cards(cards_path, Text(exe))
    first = picture_first(cards_path) - MINI_EXCHANGED
    data = bytearray(picmini_mrg)
    if len(picmini_mrg) > first * MINI_SIZE:
        raise ValueError(f"PICMINI.MRG has {len(picmini_mrg) // MINI_SIZE} records; the new ones would "
                         f"start inside it, at {first}")
    if len(data) < first * MINI_SIZE:
        data += bytes(first * MINI_SIZE - len(data))
    del data[first * MINI_SIZE:]
    made = {}
    for card in cards:
        donor, art = card["donor"], card.get("art")
        if (donor, art) not in made:
            record = bytearray(picmini_mrg[donor * MINI_SIZE:(donor + 1) * MINI_SIZE])
            if record[:4] == b"RH2\0" and art:
                width, height, rows = mt.read_png(Path(cards_path).parent / art)
                t = mt.info(record, 0)
                mt.encode(record, 0, fit(rows, width, height, t["width"], t["height"]))
            made[(donor, art)] = bytes(record)
        data += made[(donor, art)]
    for card in load_edits(cards_path, Text(exe)):
        replace_art(data, card["number"] * MINI_SIZE, MINI_SIZE, Path(cards_path).parent, card.get("art"))
    return bytes(data)


if __name__ == "__main__":
    generate(sys.argv[1], sys.argv[2])
    print(Path(sys.argv[2]).read_text())
