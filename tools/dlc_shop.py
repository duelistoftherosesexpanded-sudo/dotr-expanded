#!/usr/bin/env python3
"""The card shop of the DLC (dlc/shop.c): card rarities and the shop's pictures.

Rarity. Every card has one of six tiers. Each of the shop's 25 places first draws a tier by its
chance (CHANCES, in percent), then a card of that tier at random. A card's tier is the one
mods/<mod>/rarity.json gives it ({"<card number>": "<tier name>"}, written by the Card Maker;
its "not_in_shop" list holds the cards the shop never offers, the Card Maker's checkbox);
a card it does not name gets one from its deck cost: the cheapest half Common, then Rare, Super
Rare, Ultra Rare and Secret Rare by AUTO_SHARES. Ultra Secret is only ever given by hand.

  rarity_gen.c   dlc_rarity[1000] (255: not a card) and the tiers' chances, in the DLC's live
                 region (tools/mod.py), so a change in rarity.json is applied to the running game.
  shop_gen.c     the pictures, as 128 x 128 picture textures of the game's own kind (a copy of
                 PICTURE.MRG's first record with new pixels): shop/background.png in 4 x 2 tiles,
                 shop/keeper.png (the shop keeper, where the deck builder shows the deck leader) and
                 a small picture of each card sleeve (tools/dlc_sleeves.py) for the shop's SLEEVE bar.

  python3 tools/dlc_shop.py           the tier of every card, as the build would give them
"""

import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import model_texture as mt  # noqa: E402

TIERS = ["Common", "Rare", "Super Rare", "Ultra Rare", "Secret Rare", "Ultra Secret"]
CHANCES = [50, 25, 15, 8, 2, 0.01]          # percent of a shop place
AUTO_SHARES = [0.50, 0.25, 0.15, 0.08, 0.02]  # cards without a tier of their own, by deck cost
CARD_LIMIT = 1532          # dlc_cards.h DLC_CARD_LIMIT
TILES_X, TILES_Y = 4, 2
PICTURE = 128


def tier_index(value):
    if isinstance(value, int):
        if not 0 <= value < len(TIERS):
            raise ValueError(f"rarity {value} is not 0-{len(TIERS) - 1}")
        return value
    lowered = [t.lower() for t in TIERS]
    if str(value).strip().lower() not in lowered:
        raise ValueError(f"rarity {value!r} is not one of {', '.join(TIERS)}")
    return lowered.index(str(value).strip().lower())


def rarity_path(mod):
    return Path(mod) / "rarity.json"


NOT_IN_SHOP = "not_in_shop"   # rarity.json: [card numbers] the shop never offers


def explicit(mod):
    """{card number: tier} from rarity.json."""
    path = rarity_path(mod)
    if not path.is_file():
        return {}
    return {int(k): tier_index(v) for k, v in json.loads(path.read_text()).items() if k.isdigit()}


def not_in_shop(mod):
    """The cards rarity.json keeps out of the shop (the Card Maker's "Sold in the card shop")."""
    path = rarity_path(mod)
    if not path.is_file():
        return set()
    return {int(n) for n in json.loads(path.read_text()).get(NOT_IN_SHOP, [])}


def card_costs(mod, exe):
    """{card number: (deck cost, attack)} of every card the game knows: the game's 0-853 (not 671)
    and the new ones of cards.json."""
    import dlc_cards
    meta = dlc_cards.inventory(exe)
    costs = {}
    for n in range(854):
        if n != 671:
            r = dlc_cards.record(exe, n, meta)
            costs[n] = (r["deck_cost"], r["attack"])
    cards_path = Path(mod) / "cards.json"
    if cards_path.is_file():
        for card in dlc_cards.load_cards(cards_path, dlc_cards.Text(exe)):
            donor = costs[card["donor"]]
            cost = card["_deck_cost"] if card["_deck_cost"] >= 0 else donor[0]
            costs[card["number"]] = (cost, int(card.get("attack", donor[1])))
        # The game's cards cards.json changes.
        for card in dlc_cards.load_edits(cards_path, dlc_cards.Text(exe)):
            game = costs[card["number"]]
            costs[card["number"]] = (card["_deck_cost"] if card["_deck_cost"] >= 0 else game[0],
                                     int(card.get("attack", game[1])))
    return costs


def automatic(costs):
    """{card number: tier} from the deck cost (then attack, then number), cheapest first."""
    order = sorted(costs, key=lambda n: (costs[n], n))
    tiers, at = {}, 0
    for tier, share in enumerate(AUTO_SHARES):
        count = round(share * len(order)) if tier < len(AUTO_SHARES) - 1 else len(order) - at
        for n in order[at:at + count]:
            tiers[n] = tier
        at += count
    return tiers


def rarities(mod, exe):
    """{card number: tier} of every card: rarity.json's, else the automatic one."""
    costs = card_costs(mod, exe)
    tiers = automatic(costs)
    for n, t in explicit(mod).items():
        if n in tiers:
            tiers[n] = t
    return tiers


def generate_rarity(mod, out_path, exe):
    tiers = rarities(mod, exe)
    kept_out = not_in_shop(mod)
    table = [255 if n in kept_out else tiers.get(n, 255) for n in range(CARD_LIMIT)]
    chances = [round(c * 100) for c in CHANCES]
    lines = ["/* Written by tools/dlc_shop.py from rarity.json and cards.json: each card's tier (255: not a",
             " * card, or not sold in the shop) and the tiers' chances in 1/100 percent (dlc/shop.c). */",
             f"extern const unsigned char dlc_rarity[{CARD_LIMIT}];",
             f"extern const unsigned short dlc_rarity_chances[{len(TIERS)}];",
             f"const unsigned char dlc_rarity[{CARD_LIMIT}] = {{"]
    for at in range(0, CARD_LIMIT, 40):
        lines.append("    " + ", ".join(str(v) for v in table[at:at + 40]) + ",")
    lines.append("};")
    lines.append(f"const unsigned short dlc_rarity_chances[{len(TIERS)}] = {{{', '.join(map(str, chances))}}};")
    Path(out_path).write_text("\n".join(lines) + "\n", newline="\n")


# ------------------------------------------------------------------------------------------ pictures

def picture_record(iso=None):
    """PICTURE.MRG's first record, the picture texture the shop's pictures are written into."""
    import iso_files
    import project
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup("/DATA/PICTURE.MRG")
        start = struct.unpack_from("<I", entry, 2)[0]
        data = disc.read(start, 0x4800 // 2048 + 1)
    finally:
        disc.close()
    t = mt.info(data, 0)
    if (t["width"], t["height"]) != (PICTURE, PICTURE):
        raise ValueError("PICTURE.MRG: the first picture is not 128 x 128")
    return data[:t["size"]]


def picture(record, rows):
    """The record with these 128 x 128 rows of (r, g, b)."""
    data = bytearray(record)
    mt.encode(data, 0, rows)
    return bytes(data)


def scaled(rows, width, height, new_width, new_height):
    return mt.resize(width, height, [[p[:3] for p in r] for r in rows], new_width, new_height)


def crop(rows, x0, y0, x1, y1):
    return [r[x0:x1] for r in rows[y0:y1]]


def shop_pictures(mod, iso=None):
    """(background tiles left to right, top to bottom; the keeper; [(sleeve name, picture)])."""
    import dlc_sleeves
    record = picture_record(iso)
    folder = Path(mod) / "shop"
    w, h, rows = mt.read_png(folder / "background.png")
    whole = scaled(rows, w, h, PICTURE * TILES_X, PICTURE * TILES_Y)
    tiles = [picture(record, crop(whole, x * PICTURE, y * PICTURE, (x + 1) * PICTURE, (y + 1) * PICTURE))
             for y in range(TILES_Y) for x in range(TILES_X)]
    w, h, rows = mt.read_png(folder / "keeper.png")
    keeper = picture(record, scaled(rows, w, h, PICTURE, PICTURE))
    sleeves = []
    x0, y0, x1, y1 = dlc_sleeves.BACK
    for name, texture in dlc_sleeves.textures(mod, iso):
        tw, th, rows = mt.decode(texture, 0)
        back = crop(rows, x0, y0, x1, y1)
        sleeves.append((name, picture(record, scaled(back, x1 - x0, y1 - y0, PICTURE, PICTURE))))
    return tiles, keeper, sleeves


TEXSYS_WINDOW = 7          # TEXSYS.MRG's eighth texture: the window parts and the system icons
HELP_ICON = (192, 32)      # the campaign map's "?" icon there, 32 x 32 (tex_uv_data row 50)
GOLD = [(255, 236, 140), (240, 196, 48), (196, 140, 16), (128, 84, 8)]   # light, mid, shade, deep
OUTLINE = (56, 32, 8)


def window_texture(iso=None):
    import iso_files
    import project
    disc = iso_files.Image(project.find_iso(iso), read_only=True)
    try:
        _, _, entry = disc.lookup("/DATA/TEXSYS.MRG")
        start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
        data = disc.read(start, (size + 2047) // 2048)[:size]
    finally:
        disc.close()
    at = -1
    for _ in range(TEXSYS_WINDOW + 1):
        at = data.find(b"RH2\0", at + 1)
    return data, at


def trophy_rows(iso=None):
    """The "?" icon (32 x 32) with a gold trophy in place of the question mark."""
    data, at = window_texture(iso)
    _, _, rows = mt.decode(data, at)
    x0, y0 = HELP_ICON
    icon = [list(r[x0:x0 + 32]) for r in rows[y0:y0 + 32]]
    # The icon's own face colour: the commonest grey inside it.
    counts = {}
    for y in range(6, 26):
        for x in range(6, 26):
            p = icon[y][x]
            if abs(p[0] - p[1]) < 16 and abs(p[1] - p[2]) < 16:
                counts[p] = counts.get(p, 0) + 1
    face = max(counts, key=counts.get)
    # Take the question mark away: everything inside the face that is not the face's grey.
    for y in range(2, 30):
        for x in range(6, 26):
            p = icon[y][x]
            if not (abs(p[0] - p[1]) < 16 and abs(p[1] - p[2]) < 16) or sum(p) < sum(face) - 120:
                icon[y][x] = face
    # The trophy: a cup with two handles, a stem and a two-step base (1 = gold).
    shape = [
        "..........................",
        "....##################....",
        "....##################....",
        "..##.################.##..",
        ".#...################...#.",
        ".#...################...#.",
        ".#...################...#.",
        "..#...##############...#..",
        "...##.##############.##...",
        ".......############.......",
        "........##########........",
        "..........######..........",
        "...........####...........",
        "...........####...........",
        "...........####...........",
        ".........########.........",
        "........##########........",
        "......##############......",
        "......##############......",
    ]
    ox, oy = 3, 5
    gold = set()
    for y, row in enumerate(shape):
        for x, c in enumerate(row):
            if c == "#":
                gold.add((ox + x, oy + y))
    for (x, y) in gold:
        # Light on the left and top, shade on the right, deep at the far right of each run.
        left = (x - 1, y) not in gold
        right = (x + 1, y) not in gold
        top = (x, y - 1) not in gold
        tone = 0 if left or top else 3 if right else 2 if (x + 1, y) not in gold or (x + 2, y) not in gold else 1
        icon[y][x] = GOLD[tone]
    for (x, y) in list(gold):
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            q = (x + dx, y + dy)
            if q not in gold and 0 <= q[0] < 32 and 0 <= q[1] < 32:
                icon[q[1]][q[0]] = OUTLINE
    return icon


TRANSPARENT = (255, 0, 255)


def trophy_picture(record, iso=None):
    """The trophy icon at 4 x (128 x 128, each pixel a 4 x 4 block); its four corner pixels, clear in
    the game's icon, clear here too."""
    icon = trophy_rows(iso)
    for x, y in ((0, 0), (31, 0), (0, 31), (31, 31)):
        icon[y][x] = TRANSPARENT
    big = [[icon[y // 4][x // 4] for x in range(PICTURE)] for y in range(PICTURE)]
    data = bytearray(picture(record, big))
    t = mt.info(data, 0)
    for i in range(256):
        v, = struct.unpack_from("<H", data, t["palette"] + 2 * i)
        if v & 0x7fff == (31 | 0 << 5 | 31 << 10):
            struct.pack_into("<H", data, t["palette"] + 2 * i, 0)
    return bytes(data)


SECRET_TEXTURE = 3                 # TEXSYS.MRG's fourth texture: the large secret ("?") card faces
SECRET_ART = (30, 84, 226, 280)    # the "?" over the rose shield, on its left face


def question_picture(record, iso=None):
    """The secret card's "?" art (128 x 128), the portrait of an opponent who has none (the Deck
    Masters' panel, bosses.c)."""
    data, _ = window_texture(iso)
    at = -1
    for _ in range(SECRET_TEXTURE + 1):
        at = data.find(b"RH2\0", at + 1)
    _, _, rows = mt.decode(data, at)
    x0, y0, x1, y1 = SECRET_ART
    return picture(record, scaled(crop(rows, x0, y0, x1, y1), x1 - x0, y1 - y0, PICTURE, PICTURE))


def c_bytes(name, data):
    body = ",".join(str(b) for b in data)
    return f"static const unsigned char {name}[{len(data)}] __attribute__((aligned(64))) = {{{body}}};"


def generate_pictures(mod, out_path, iso=None):
    tiles, keeper, sleeves = shop_pictures(mod, iso)
    lines = ["/* Written by tools/dlc_shop.py: the card shop's pictures (dlc/shop.c). */",
             # Declared first: a const definition alone would be the file's own.
             "extern const int dlc_shop_tiles_x, dlc_shop_tiles_y, dlc_shop_sleeve_count;",
             "extern const unsigned char *const dlc_shop_tiles[];",
             "extern const unsigned char *const dlc_shop_keeper;",
             "extern const unsigned char *const dlc_shop_sleeves[];",
             "extern const unsigned char *const dlc_trophy_icon;"]
    # The campaign map's trophy icon (achievements.c DlcAch_SysIcon).
    lines.append(c_bytes("trophy", trophy_picture(picture_record(iso), iso)))
    lines.append("const unsigned char *const dlc_trophy_icon = trophy;")
    lines.append("extern const unsigned char *const dlc_question_face;")
    lines.append(c_bytes("question", question_picture(picture_record(iso), iso)))
    lines.append("const unsigned char *const dlc_question_face = question;")
    for k, data in enumerate(tiles):
        lines.append(c_bytes(f"tile_{k}", data))
    lines.append(c_bytes("keeper", keeper))
    for k, (_, data) in enumerate(sleeves):
        lines.append(c_bytes(f"sleeve_{k}", data))
    lines.append(f"const int dlc_shop_tiles_x = {TILES_X};")
    lines.append(f"const int dlc_shop_tiles_y = {TILES_Y};")
    lines.append("const unsigned char *const dlc_shop_tiles[] = {"
                 + ", ".join(f"tile_{k}" for k in range(len(tiles))) + "};")
    lines.append("const unsigned char *const dlc_shop_keeper = keeper;")
    lines.append(f"const int dlc_shop_sleeve_count = {len(sleeves)};")
    lines.append("const unsigned char *const dlc_shop_sleeves[] = {"
                 + ", ".join(f"sleeve_{k}" for k in range(len(sleeves))) + "};")
    Path(out_path).write_text("\n".join(lines) + "\n", newline="\n")


if __name__ == "__main__":
    from runtime import ROOT
    mod = ROOT / "mods" / "widescreen"
    import dlc_cards
    exe = dlc_cards.game_exe(mod=mod)
    tiers = rarities(mod, exe)
    counts = [sum(1 for t in tiers.values() if t == k) for k in range(len(TIERS))]
    for name, count in zip(TIERS, counts):
        print(f"{name:13} {count}")
