#!/usr/bin/env python3
"""The Dawn of Destiny and Falsebound Kingdom parts of the full model dump in models/ (the other
parts: tools/saikyo_dump.py -> models/saikyo, tools/cross_dump.py -> models/cross):

  models/dod/<model> <card name>/<model>.glb, _atlas.png, _preview.png   every monster model of the
                                    Xbox disc (tools/dod_export.py), named by the game's table
  models/falsebound/<slot> <name>/<slot>.dat, _atlas.png, _preview.png   every slot of
                                    Model/MONSTER.MRG (build/fbk_slots, tools/fbk_models.py), named
                                    where falsebound/names.json names it
  and a gallery.html and models.csv in each.

  models_dump.py --dod DOD.iso [--falsebound] [--jobs N]
"""
import argparse
import csv
import html
import json
import re
import shutil
import sys
import traceback
from multiprocessing import Pool
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
OUT = ROOT / "models"


def slug(name):
    return re.sub(r"\s+", " ", re.sub(r'[<>:"/\\|?*]', "", name)).strip()


def page(folder, title, cells):
    (folder / "gallery.html").write_text(
        f"<!doctype html><meta charset=utf-8><title>{title}</title><style>body{{background:#222;color:#ddd;"
        "font:13px sans-serif;margin:16px}figure{display:inline-block;width:256px;margin:4px;vertical-align:top}"
        f"img{{width:256px;height:256px}}small{{color:#999}}</style><h2>{len(cells)} models</h2>" + "".join(cells),
        encoding="utf-8")


def dod_one(job):
    iso, model, folder = job
    import dod_export
    import dod_gallery
    try:
        Path(folder).mkdir(parents=True, exist_ok=True)
        glb, _ = dod_export.export(iso, model, folder)
        dod_gallery.snapshot(glb, Path(folder) / f"{model}_atlas.png", Path(folder) / f"{model}_preview.png")
        return None
    except Exception as error:
        return f"{model}: {error!r} {traceback.format_exc()[-300:]}"


def dump_dod(iso, jobs):
    import dod_models
    out = OUT / "dod"
    out.mkdir(parents=True, exist_ok=True)
    rows, todo, seen = [], [], set()
    for r in dod_models.listing(iso):
        if r["model"] in seen:
            continue                      # a model two cards share: once, by its first card
        seen.add(r["model"])
        folder = out / slug(f"{r['model']} {r['name'] or 'unused'}")
        rows.append(dict(r, folder=folder.name))
        todo.append((iso, r["model"], str(folder)))
    with Pool(jobs) as pool:
        failed = [e for e in pool.map(dod_one, todo, chunksize=4) if e]
    for f in failed:
        print("FAILED", f)
    with open(out / "models.csv", "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    page(out, "Dawn of Destiny models", [
        f'<figure><img src="{html.escape(r["folder"])}/{r["model"]}_preview.png" loading="lazy"><figcaption>'
        f'{html.escape(r["model"] + " " + (r["name"] or "(unused)"))}<br><small>Konami {r["konami"] or "-"}</small>'
        f'</figcaption></figure>' for r in rows])
    print(f"dod: {len(rows) - len(failed)} models, {len(failed)} failed")


def dump_falsebound():
    import fbk_models
    out = OUT / "falsebound"
    out.mkdir(parents=True, exist_ok=True)
    rows, cells = [], []
    for r in fbk_models.listing():
        folder = out / slug(f"{r['slot']} {r['name'] or 'unnamed'}")
        folder.mkdir(exist_ok=True)
        for suffix in (".dat", "_atlas.png"):
            src = fbk_models.ALL / f"{r['slot']}{suffix}"
            if src.is_file():
                shutil.copy2(src, folder / src.name)
        picture = fbk_models.FBK / "gallery" / f"{r['slot']}.png"
        if picture.is_file():
            shutil.copy2(picture, folder / f"{r['slot']}_preview.png")
        rows.append(dict(r, folder=folder.name))
        cells.append(f'<figure><img src="{html.escape(folder.name)}/{r["slot"]}_preview.png" loading="lazy">'
                     f'<figcaption>{html.escape(r["slot"] + " " + (r["name"] or "(unnamed)"))}</figcaption></figure>')
    with open(out / "models.csv", "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    page(out, "Falsebound Kingdom models", cells)
    print(f"falsebound: {len(rows)} slots")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--dod")
    p.add_argument("--falsebound", action="store_true")
    p.add_argument("--jobs", type=int, default=6)
    a = p.parse_args()
    if a.falsebound:
        dump_falsebound()
    if a.dod:
        dump_dod(a.dod, a.jobs)


if __name__ == "__main__":
    main()
