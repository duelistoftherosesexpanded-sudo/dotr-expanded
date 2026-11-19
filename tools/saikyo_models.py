#!/usr/bin/env python3
"""Every Yu-Gi-Oh! Saikyo Card Battle (3DS) monster as glTF, laid out like the other games' folders
(dotr-dod-models/, cross duel/, falsebound/):

  saikyo/<Konami id> <name>/Mo_<model>.glb (+ _atlas.png)   one folder a monster (tools/bch_export.py)
  saikyo/gallery/<Konami id>.png                             a snapshot of each (posed by its idle)
  saikyo/model_list.csv, gallery.html                        id, name, model, scale, whether DotR or
                                                             the DLC has the card

Which card each model is comes from the game's own table (build/saikyo/model_table.json: the
program's list of Konami id, display scale, model file; tools/saikyo_table.py).

  saikyo_models.py ROMFS [--only NAME,NAME] [--no-gallery]
"""
import argparse
import csv
import json
import re
import sys
import traceback
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
OUT = ROOT / "saikyo"
TABLE = ROOT / "build" / "saikyo" / "model_table.json"


def slug(name):
    return re.sub(r"\s+", " ", re.sub(r'[<>:"/\\|?*]', "", name)).strip().lower()


def snapshot(glb, atlas, out):
    import fbx_anim as fa
    import glb_anim
    import model_preview as mp
    import model_texture as mt
    model = glb_anim.load(glb)
    clip = next((c for c in model["clips"] if c["name"].endswith("_b")), None)
    pos, uv, tris = fa.posed_mesh(model, clip, 0.0)
    image = mp.draw(pos, uv, tris, mt.read_png(str(atlas)), size=256, yaw=-25, pitch=8)
    mt.write_png(str(out), 256, 256, [list(r) for r in image])


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("romfs")
    p.add_argument("--only")
    p.add_argument("--no-gallery", action="store_true")
    a = p.parse_args()
    import bch_export
    import dod_models
    table = [r for r in json.loads(TABLE.read_text(encoding="utf-8")) if "/Mo_" in r["model"]]
    if a.only:
        want = set(a.only.split(","))
        table = [r for r in table if Path(r["model"]).stem in want or str(r["cid"]) in want]
    base, dlc = dod_models.dotr_names(), dod_models.dlc_names()
    (OUT / "gallery").mkdir(parents=True, exist_ok=True)
    rows, failed = [], []
    for r in table:
        name = r["name"] if r["name"] != "?" else f"unknown {r['cid']}"
        folder = OUT / f"{r['cid']:05d} {slug(name)}"
        src = Path(a.romfs) / r["model"]
        try:
            glb, report = bch_export.export(src, folder)
            if not a.no_gallery:
                snapshot(glb, folder / f"{glb.stem}_atlas.png", OUT / "gallery" / f"{r['cid']:05d}.png")
            print(f"{r['cid']:05d} {name}: ok", flush=True)
        except Exception as error:
            failed.append((r["cid"], name, repr(error)))
            print(f"{r['cid']:05d} {name}: FAILED {error!r}", flush=True)
            traceback.print_exc()
            continue
        k = dod_models.key(name)
        rows.append(dict(id=f"{r['cid']:05d}", name=name, model=Path(r["model"]).stem, scale=r["scale"],
                         dotr=base.get(k, ""), dlc=dlc.get(k, ""), folder=folder.name))
    if not a.only:
        with open(OUT / "model_list.csv", "w", newline="", encoding="utf-8") as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)
        cells = "".join(f'<figure><img src="gallery/{r["id"]}.png" loading="lazy"><figcaption>{r["id"]} {r["name"]}'
                        f'{" (DotR " + str(r["dotr"]) + ")" if r["dotr"] else ""}{" (DLC " + str(r["dlc"]) + ")" if r["dlc"] else ""}'
                        f'</figcaption></figure>' for r in rows)
        (OUT / "gallery.html").write_text(
            "<!doctype html><meta charset=utf-8><title>Saikyo Card Battle models</title><style>body{background:#222;color:#ddd;"
            "font:13px sans-serif}figure{display:inline-block;width:256px;margin:4px}img{width:256px}</style>" + cells,
            encoding="utf-8")
    print(f"{len(rows)} exported, {len(failed)} failed")
    for f in failed:
        print("  ", *f)


if __name__ == "__main__":
    main()
