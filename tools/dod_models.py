#!/usr/bin/env python3
"""Lists the monster models of Yu-Gi-Oh! The Dawn of Destiny (the player's own disc) by card, and
which of them Duelists of the Roses lacks; with --export, exports those (tools/dod_export.py).

Each card's model comes from the game's own table (default.xbe, read by the function at 0x7d520):
1017 pairs (Konami card number, model number), model mNNN being monster/<hundreds>/mNNN.igb. Up to
m729 the model is Konami number - 4007; later cards were fitted in where there was room. A model no
card uses (20: parts and effects) is listed as unused. Names are the game's own (its name table,
in the order of its card list), the card database's (tools/card_lookup.py, Konami numbers in
build/card_data/konami.json) giving the type.

A card counts as in DotR when a base card of DotR has its name (letters and digits compared, and
DotR's short names: tools/card_lookup.py ALIASES); in the DLC when a card of cards.json has it.

  dod_models.py ISO [--out LIST.csv] [--export OUT_DIR]

  LIST.csv  model, Konami number, name, type, in DotR (card number), in the DLC (card number)
"""

import argparse
import csv
import json
import re
import struct
import sys
import urllib.request
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import card_lookup  # noqa: E402
import dlc_cards  # noqa: E402
import igb_read  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CARD_LIST = 0x5d0c44
CARD_LIST_FIRST = (4007, 4008, 4009)
NAME_TABLE = 0x5740a4       # the cards' English names, one after another, in card list order
MODEL_TABLE = 0x44d5f0      # virtual address of the (Konami, model) pairs
MODEL_PAIRS = 0x3f9
RDATA_VA, RDATA_RAW = 0x420c60, 0x411000
KONAMI = ROOT / "build" / "card_data" / "konami.json"


def key(name):
    return re.sub(r"[^a-z0-9]", "", name.lower())


def card_list(xbe):
    """The game's card list (Konami numbers): found at CARD_LIST, or searched for by its start."""
    at = CARD_LIST
    if struct.unpack_from("<3H", xbe, at) != CARD_LIST_FIRST:
        at = xbe.find(struct.pack("<3H", *CARD_LIST_FIRST))
        if at < 0:
            raise ValueError("the card list was not found in default.xbe")
    ids = []
    while at + 2 <= len(xbe):
        v = struct.unpack_from("<H", xbe, at)[0]
        if not 3900 <= v <= 9999:
            break
        ids.append(v)
        at += 2
    return ids


def model_table(xbe):
    """{model number: [Konami numbers]} from the game's table."""
    at = MODEL_TABLE - RDATA_VA + RDATA_RAW
    if struct.unpack_from("<HH", xbe, at) != (4007, 0):
        at = xbe.find(struct.pack("<4H", 4007, 0, 4008, 1))
        if at < 0:
            raise ValueError("the card-to-model table was not found in default.xbe")
    out = {}
    for i in range(MODEL_PAIRS):
        kid, model = struct.unpack_from("<HH", xbe, at + 4 * i)
        out.setdefault(model, []).append(kid)
    return out


def game_names(xbe):
    """{Konami number: the game's own English name}."""
    ids = card_list(xbe)
    at = NAME_TABLE
    if not xbe[at:at + 23] == b"Blue-Eyes White Dragon\0":
        at = xbe.find(b"Blue-Eyes White Dragon\0Mystical Elf\0")
    names = xbe[at:].split(b"\0", len(ids))[:len(ids)]
    return {kid: n.decode("latin-1") for kid, n in zip(ids, names)}


def konami_names(refresh=False):
    """{Konami number: dict(name, type)} from the card database."""
    if refresh or not KONAMI.is_file():
        KONAMI.parent.mkdir(parents=True, exist_ok=True)
        req = urllib.request.Request(card_lookup.API + "?misc=yes", headers={"User-Agent": card_lookup.AGENT})
        data = json.loads(urllib.request.urlopen(req, timeout=120).read())["data"]
        out = {}
        for c in data:
            for mi in c.get("misc_info", []):
                if mi.get("konami_id"):
                    out[str(mi["konami_id"])] = dict(name=c["name"], type=c["type"])
        KONAMI.write_text(json.dumps(out))
    return {int(k): v for k, v in json.loads(KONAMI.read_text()).items()}


def dotr_names():
    """{name key: DotR card number} of the base game's cards, with its short names' full forms."""
    exe = dlc_cards.game_exe()
    t = dlc_cards.Text(exe)
    out = {}
    for n in range(dlc_cards.ORIGINAL):
        if n == 671:
            continue
        name = t.decode(dlc_cards.NAMES + n)
        out.setdefault(key(name), n)
        alias = card_lookup.ALIASES.get(name.strip().lower())
        if alias:
            out.setdefault(key(alias), n)
    return out


def dlc_names():
    entries = json.loads((ROOT / "mods" / "widescreen" / "cards.json").read_text())
    out = {}
    for e in entries:
        if e.get("number", 0) >= dlc_cards.FIRST and e.get("name"):
            out.setdefault(key(e["name"]), e["number"])
            alias = card_lookup.ALIASES.get(e["name"].strip().lower())
            if alias:
                out.setdefault(key(alias), e["number"])
    return out


def listing(iso_path):
    iso = igb_read.XIso(iso_path)
    xbe = iso.read("/default.xbe")
    table, own = model_table(xbe), game_names(xbe)
    names = konami_names()
    base, dlc = dotr_names(), dlc_names()
    rows = []
    for path in sorted(p for p in iso.files() if re.search(r"/monster/\d+/m\d+\.igb$", p)):
        n = int(re.search(r"m(\d+)\.igb$", path).group(1))
        kids = table.get(n, [])
        if not kids:
            rows.append(dict(model=f"m{n:03d}", konami="", name="", type="(unused: no card)", dotr="", dlc=""))
            continue
        for kid in kids:      # a model two cards share is listed for each
            db = names.get(kid, {})
            name = own.get(kid) or db.get("name", "")
            found = [k for k in (key(name), key(db.get("name", ""))) if k]
            rows.append(dict(model=f"m{n:03d}", konami=kid, name=name, type=db.get("type", ""),
                             dotr=next((base[k] for k in found if k in base), ""),
                             dlc=next((dlc[k] for k in found if k in dlc), "")))
    return rows


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("iso")
    p.add_argument("--out", default=str(ROOT / "dotr-dod-models" / "model_list.csv"))
    p.add_argument("--export", help="export the models of cards DotR lacks to this folder")
    a = p.parse_args()
    rows = listing(a.iso)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    with open(a.out, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    in_dotr = {r["model"] for r in rows if r["dotr"] != ""}     # a shared model DotR has once is not new
    unique = [r for r in rows if r["name"] and r["model"] not in in_dotr]
    seen = set()
    unique = [r for r in unique if not (r["model"] in seen or seen.add(r["model"]))]
    print(f"{len(rows)} models; {sum(1 for r in rows if r['dotr'] != '')} cards DotR has; {len(unique)} it lacks "
          f"({sum(1 for r in unique if r['dlc'] != '')} of those already in the DLC); "
          f"{sum(1 for r in rows if not r['name'])} without a name; list: {a.out}")
    if a.export:
        import dod_export
        for k, r in enumerate(unique):
            folder = Path(a.export) / f"{r['model']} {re.sub(r'[^A-Za-z0-9 .-]', '', r['name']).lower()}"
            for old in Path(a.export).glob(f"{r['model']} *"):      # exported before under another name
                if old != folder and not folder.exists():
                    old.rename(folder)
            if (folder / f"{r['model']}.glb").is_file():
                continue
            folder.mkdir(parents=True, exist_ok=True)
            try:
                dod_export.export(a.iso, r["model"], folder)
                print(f"{k + 1}/{len(unique)} {r['model']} {r['name']}", flush=True)
            except Exception as error:   # one model that does not read is reported, the rest go on
                print(f"{k + 1}/{len(unique)} {r['model']} {r['name']}: FAILED {error}", flush=True)


if __name__ == "__main__":
    main()
