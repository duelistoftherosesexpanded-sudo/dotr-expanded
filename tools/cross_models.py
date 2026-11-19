#!/usr/bin/env python3
"""The Yu-Gi-Oh! Cross Duel models in one place, laid out as the Dawn of Destiny ones
(dotr-dod-models): a folder for each card, "<model id> <name>", a snapshot of each in gallery/,
gallery.html and model_list.csv (whether Duelists of the Roses or the DLC has the card).

Two sources:
  - the game's bundles, exported with their animations by tools/crossduel_export.py (its
    models.json): a .glb per rig, all clips a-h (and more for the big monsters);
  - folders of FBX rips added by hand (an old layout's folders that the export did not make), kept
    as they are; their clips hold no keys, so they are listed as without animation.

A model id is the card's Konami number (07991 Vice Dragon); _01J and _01A rigs of a card share
its folder. A card is matched to DotR's and the DLC's by name (tools/dod_models.py).

  cross_models.py EXPORT_DIR HAND_DIR OUT_DIR     (OUT_DIR is made new; the sources are left alone)
"""

import csv
import html
import json
import re
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
RIG = re.compile(r"(\d{5,7})_(\d\d[A-Z]?)")
SIZE = 240


def slug(name):
    return re.sub(r"[^A-Za-z0-9 .'-]", "", name).strip().lower()


def snapshot(model_path, texture_png, out):
    import fbx_anim as fa
    import model_preview as mp
    import model_texture as mt
    if str(model_path).lower().endswith(".glb"):
        import glb_anim
        model = glb_anim.load(str(model_path))
        clips = {c["name"][-1:]: c for c in model["clips"]}
        clip = clips.get("b") or (model["clips"][0] if model["clips"] else None)
    else:
        model = fa.load(str(model_path))
        clip = None
    tex = mt.read_png(str(texture_png))
    pos, uv, tris = fa.posed_mesh(model, clip, 0.0)
    image = mp.draw(pos, uv, tris, tex, size=SIZE, yaw=-25, pitch=8)
    mt.write_png(str(out), SIZE, SIZE, [list(r) for r in image])


def main():
    export_dir, hand_dir, out = (Path(a) for a in sys.argv[1:4])
    import dod_models
    import glb_anim
    names = {int(k): v["name"] for k, v in dod_models.konami_names().items()}
    base, dlc = dod_models.dotr_names(), dod_models.dlc_names()
    out.mkdir(parents=True, exist_ok=False)
    (out / "gallery").mkdir()
    rows = {}

    def row(ident, name, source):
        if ident not in rows:
            k = dod_models.key(name)
            rows[ident] = dict(id=ident, name=name, source=source, rigs=[], clips=0, animated="",
                               dotr=base.get(k, ""), dlc=dlc.get(k, ""), folder=f"{ident} {slug(name)}")
        return rows[ident]

    # The game's bundles.
    report = json.loads((export_dir / "models.json").read_text(encoding="utf-8"))
    for rig, info in sorted(report.items()):
        ident = RIG.match(rig).group(1)
        name = names.get(int(ident)) or info["card"]
        r = row(ident, name if not name.startswith("Unknown") else f"unknown {ident}", "game bundles")
        src = export_dir / re.sub(r'[<>:"/\\|?*]', "", info["card"]).strip()
        dst = out / r["folder"]
        dst.mkdir(exist_ok=True)
        for f in [info["file"]] + info.get("textures", []):
            if (src / f).is_file():
                shutil.copy2(src / f, dst / f)
        r["rigs"].append(rig)
        r["clips"] = max(r["clips"], len(info["clips"]))
        r["animated"] = "yes"
    # Folders added by hand, for cards the bundles do not have.
    old = json.loads((hand_dir / "models.json").read_text(encoding="utf-8")) if (hand_dir / "models.json").is_file() else {}
    exported_names = {re.sub(r'[<>:"/\\|?*]', "", v["card"]).strip() for v in old.values()}
    for folder in sorted(p for p in hand_dir.iterdir() if p.is_dir() and p.name not in exported_names):
        fbx = sorted(folder.glob("*.fbx"))
        if not fbx:
            continue
        m = RIG.match(fbx[0].stem)
        ident = m.group(1) if m else None
        if ident is None:          # named by the folder only (boomboxen.fbx): its texture names the id
            t = next((RIG.search(p.name) for p in folder.glob("tex_*_def.png")), None)
            ident = t.group(1) if t else folder.name
        if ident in rows and rows[ident]["source"] == "game bundles":
            continue               # the bundles have it, with its animations
        r = row(ident, names.get(int(ident)) if ident.isdigit() and int(ident) in names else folder.name.title(), "added by hand")
        dst = out / r["folder"]
        shutil.copytree(folder, dst, dirs_exist_ok=True)
        r["rigs"] += [p.stem for p in fbx]
        r["animated"] = "no (its clips are empty)"
    # Snapshots.
    for r in rows.values():
        dst = out / r["folder"]
        model = next(iter(sorted(dst.glob("*.glb"))), None) or next(iter(sorted(dst.glob("*.fbx"))), None)
        tex = next(iter(sorted(dst.glob("tex_*_def.png"))), None)
        try:
            if model is not None and model.suffix == ".glb" and tex is None:
                tex = dst / "_atlas.png"
                glb_anim.atlas(model, tex)
            snapshot(model, tex, out / "gallery" / f"{r['id']}.png")
            print(r["id"], r["name"], "ok", flush=True)
        except Exception as error:   # one model that does not draw is reported, the rest go on
            print(r["id"], r["name"], "snapshot FAILED", error, flush=True)
    ordered = sorted(rows.values(), key=lambda r: r["id"])
    with open(out / "model_list.csv", "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["id", "name", "source", "rigs", "clips", "animated", "dotr", "dlc", "folder"])
        w.writeheader()
        for r in ordered:
            w.writerow({**r, "rigs": " ".join(r["rigs"])})
    cells = []
    for r in ordered:
        if r["dotr"] != "":
            status, cls = f"DotR has it (card {r['dotr']})", "have"
        else:
            status, cls = "DotR lacks it" + (f" (DLC card {r['dlc']})" if r["dlc"] != "" else ""), "new"
        anim = "animated" if r["animated"] == "yes" else "no animation"
        cells.append(f'<figure class="{cls}"><img src="gallery/{r["id"]}.png" loading="lazy" alt="{r["id"]}">'
                     f'<figcaption><b>{r["id"]}</b> {html.escape(r["name"])}<br><span>{html.escape(status)}</span>'
                     f'<br><span>{html.escape(r["source"])}, {anim}</span></figcaption></figure>')
    style = ("body{background:#1b1b1f;color:#ddd;font:13px sans-serif;margin:16px}"
             "main{display:grid;grid-template-columns:repeat(auto-fill,minmax(250px,1fr));gap:10px}"
             "figure{margin:0;background:#26262c;border-radius:6px;padding:6px;border-left:4px solid #555}"
             "figure.new{border-color:#4caf50} figure.have{border-color:#2f6db5}"
             "img{width:100%;aspect-ratio:1;object-fit:contain;background:#555} span{color:#aaa}")
    (out / "gallery.html").write_text(f'<!doctype html><meta charset="utf-8"><title>Cross Duel models</title>'
                                      f'<style>{style}</style><h3>{len(ordered)} models</h3><main>{"".join(cells)}</main>',
                                      encoding="utf-8")
    print(len(ordered), "models;", sum(1 for r in ordered if r["dotr"] == ""), "DotR lacks;",
          sum(1 for r in ordered if r["animated"] == "yes"), "animated")


if __name__ == "__main__":
    main()
