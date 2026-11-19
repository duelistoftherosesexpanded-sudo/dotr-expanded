#!/usr/bin/env python3
"""The monster models of Yu-Gi-Oh! The Falsebound Kingdom (the player's own disc), the same way as
tools/dod_models.py does Dawn of Destiny's: every model, its card, whether Duelists of the Roses
or the DLC has it, and the models of the cards DotR lacks kept for new cards.

The models are the 187 slots of Model/MONSTER.MRG (0x80000 bytes each), each a HAL "HSD" .dat
(tools/hsd_anim.py reads them, with their animations). The game keeps no card numbers and its
monster names are not plain text on the disc, so each slot's card was named by sight, from the
snapshots: falsebound/names.json, {"slot": "card name"} (the database's English name); a slot not
named there is listed as unknown. Slots 177-186 are the gods' cutscene models.

  fbk_models.py MONSTER.MRG --extract [--gallery]   every slot to build/fbk_slots/slotNNN.dat (+ atlas),
                                                     a snapshot of each to falsebound/gallery/
  fbk_models.py MONSTER.MRG                          falsebound/model_list.csv and gallery.html
  fbk_models.py MONSTER.MRG --export                 also a folder for each card DotR lacks, as the
                                                     Dawn of Destiny ones: falsebound/slotNNN <name>/
                                                     with slotNNN.dat and slotNNN_atlas.png (what
                                                     cards.json uses)
"""

import argparse
import csv
import html
import json
import re
import shutil
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
FBK = ROOT / "falsebound"
SLOT = 0x80000
SLOTS = 187
SIZE = 240
ALL = ROOT / "build" / "fbk_slots"      # every slot, extracted (not kept with the project)


def slot_bytes(mrg, n):
    raw = mrg[n * SLOT:(n + 1) * SLOT]
    size = struct.unpack_from(">I", raw, 0)[0]
    return raw[:size] if 0x20 < size <= SLOT else None


def pieces(n):
    """The number of meshes (DObjs) of slot n (falsebound/monster_slots.json), 1 if unknown."""
    path = FBK / "monster_slots.json"
    if not path.is_file():
        return 1
    for s in json.loads(path.read_text()):
        if s["slot"] == n and s["models"]:
            return s["models"][0]["joints"].get("dobjs", 1)
    return 1


def posed(dat):
    """(positions, coords, triangles, nested_roots) of the model in its first motion's first frame,
    with tools/hsd_anim.py's nested skeleton root handling on or off, whichever keeps the model
    together (the smaller bounding box: a piece left out of place lies far off)."""
    import fbx_anim as fa
    import hsd_anim
    best = None
    for nested in (True, False):
        m = hsd_anim.load(str(dat), nested_roots=nested)
        pos, uv, tris = fa.posed_mesh(m, m["clips"][0] if m["clips"] else None, 0.0)
        span = sum((max(p[a] for p in pos) - min(p[a] for p in pos)) ** 2 for a in range(3))
        if best is None or span < best[0] * 0.9:
            best = (span, pos, uv, tris, nested)
    return best[1:]


def snapshot(dat, atlas, out, n=None):
    """A slot of one mesh in its bind pose (tools/fbk_hsd_render.py); a slot of several rigid
    pieces (152 on), whose bind pose leaves them out of place, posed by its first motion as the
    game shows it. Answers the nested_roots setting used, or None."""
    import model_texture as mt
    if n is not None and pieces(n) > 1:
        import model_preview as mp
        pos, uv, tris, nested = posed(dat)
        image = mp.draw(pos, uv, tris, mt.read_png(str(atlas)), size=SIZE, yaw=-25, pitch=8)
        mt.write_png(str(out), SIZE, SIZE, [list(r) for r in image])
        return nested
    import fbk_hsd_render as fr
    image = fr.render(fr.load_model(fr.Dat(Path(dat).read_bytes())), SIZE)
    mt.write_png(str(out), SIZE, SIZE, [list(r) for r in image])
    return None


def extract(mrg_path, gallery):
    import hsd_anim
    mrg = Path(mrg_path).read_bytes()
    out = ALL
    out.mkdir(parents=True, exist_ok=True)
    (FBK / "gallery").mkdir(exist_ok=True)
    # The nested_roots setting each slot of several pieces looked right with (a card made from it
    # takes it as "model_nested_roots").
    settings_path = FBK / "nested_roots.json"
    settings = json.loads(settings_path.read_text()) if settings_path.is_file() else {}
    for n in range(SLOTS):
        if gallery not in (True, False) and n not in gallery:
            continue
        data = slot_bytes(mrg, n)
        if data is None:
            print(f"slot {n}: empty")
            continue
        dat = out / f"slot{n:03d}.dat"
        dat.write_bytes(data)
        try:
            atlas = hsd_anim.atlas(dat, out / f"slot{n:03d}_atlas.png")
            if gallery and (gallery is True or n in gallery):
                nested = snapshot(dat, atlas, FBK / "gallery" / f"slot{n:03d}.png", n)
                if nested is not None:
                    settings[str(n)] = nested
            print(f"slot {n}: ok", flush=True)
        except Exception as error:      # one slot that does not read is reported, the rest go on
            print(f"slot {n}: FAILED {error}", flush=True)
    settings_path.write_text(json.dumps(dict(sorted(settings.items(), key=lambda kv: int(kv[0]))), indent=1))


def key(name):
    return re.sub(r"[^a-z0-9]", "", name.lower())


def listing():
    import dod_models
    names = json.loads((FBK / "names.json").read_text(encoding="utf-8")) if (FBK / "names.json").is_file() else {}
    base, dlc = dod_models.dotr_names(), dod_models.dlc_names()
    rows = []
    for n in range(SLOTS):
        if not (ALL / f"slot{n:03d}.dat").is_file():
            continue
        name = names.get(str(n), "")
        k = key(name) if name else ""
        rows.append(dict(slot=f"slot{n:03d}", name=name, dotr=base.get(k, "") if k else "",
                         dlc=dlc.get(k, "") if k else ""))
    with open(FBK / "model_list.csv", "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    return rows


def page(rows):
    cards = json.loads((ROOT / "mods/widescreen/cards.json").read_text(encoding="utf-8"))
    used_by = {}
    for c in cards:
        m = re.search(r"falsebound/[^\"]*?(slot\d+)\.dat", str(c.get("model", "")))
        if m:
            used_by.setdefault(m.group(1), []).append(f"{c['number']} {c.get('name', '')}")
    cells = []
    for r in rows:
        if r["dotr"] != "":
            status, cls = f"DotR has it (card {r['dotr']})", "have"
        elif not r["name"]:
            status, cls = "not named yet", "unused"
        else:
            status, cls = "DotR lacks it" + (f" (the DLC has card {r['dlc']})" if r["dlc"] != "" else ""), "new"
        card = ", ".join(used_by.get(r["slot"], [])) or "-"
        cells.append(f'<figure class="{cls}"><img src="gallery/{r["slot"]}.png" loading="lazy" alt="{r["slot"]}">'
                     f'<figcaption><b>{r["slot"]}</b> {html.escape(r["name"] or "(unnamed)")}<br>'
                     f'<span>{html.escape(status)}</span><br><span>our card: {html.escape(card)}</span></figcaption></figure>')
    style = ("body{background:#1b1b1f;color:#ddd;font:13px sans-serif;margin:16px}"
             "main{display:grid;grid-template-columns:repeat(auto-fill,minmax(250px,1fr));gap:10px}"
             "figure{margin:0;background:#26262c;border-radius:6px;padding:6px;border-left:4px solid #555}"
             "figure.new{border-color:#4caf50} figure.have{border-color:#2f6db5}"
             "img{width:100%;aspect-ratio:1;object-fit:contain;background:#555} span{color:#aaa}")
    (FBK / "gallery.html").write_text(f'<!doctype html><meta charset="utf-8"><title>Falsebound Kingdom models</title>'
                                      f'<style>{style}</style><h3>{len(rows)} models</h3><main>{"".join(cells)}</main>',
                                      encoding="utf-8")


def folder_name(r):
    return f"{r['slot']} {re.sub(r'[^A-Za-z0-9 .-]', '', r['name']).lower()}"


def export(rows):
    """A folder for each named card DotR lacks (its first slot only): falsebound/slotNNN <name>/."""
    seen, kept = set(), []
    for r in rows:
        k = key(r["name"]) if r["name"] else ""
        if not k or r["dotr"] != "" or k in seen:
            continue
        seen.add(k)
        out = FBK / folder_name(r)
        out.mkdir(exist_ok=True)
        for suffix in (".dat", "_atlas.png"):
            shutil.copy2(ALL / f"{r['slot']}{suffix}", out / f"{r['slot']}{suffix}")
        kept.append(r)
    return kept


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("mrg")
    p.add_argument("--extract", action="store_true")
    p.add_argument("--gallery", nargs="?", const="all", default=None,
                   help="draw the snapshots (all, or slots FIRST-LAST)")
    p.add_argument("--export", action="store_true")
    a = p.parse_args()
    if a.extract:
        if a.gallery in (None, "all"):
            gallery = a.gallery == "all"
        else:
            first, last = (int(x) for x in a.gallery.split("-"))
            gallery = set(range(first, last + 1))
        extract(a.mrg, gallery)
        return
    rows = listing()
    page(rows)
    named = [r for r in rows if r["name"]]
    print(f"{len(rows)} slots, {len(named)} named; DotR has {sum(1 for r in named if r['dotr'] != '')}; "
          f"lacks {sum(1 for r in named if r['dotr'] == '')}")
    if a.export:
        for r in export(rows):
            print("kept", r["slot"], r["name"], f"(DLC card {r['dlc']})" if r["dlc"] != "" else "")


if __name__ == "__main__":
    main()
