#!/usr/bin/env python3
"""A snapshot of every Dawn of Destiny monster model (the player's own disc), for checking which
cards have one: dotr-dod-models/gallery/mNNN.png and gallery.html, a page listing each model with
its card, whether DotR has it, and the DLC card (cards.json) that uses it.

Models already exported to dotr-dod-models/ are drawn from there (re-exported with --refresh);
the rest are exported to a temporary folder first.

  dod_gallery.py ISO [--part K/N] [--refresh]    the snapshots (part K of N, to run several at once)
  dod_gallery.py ISO --page                        the page, from model_list.csv and cards.json
"""

import argparse
import csv
import html
import json
import re
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import dod_export  # noqa: E402
import igb_read  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
MODELS = ROOT / "dotr-dod-models"
GALLERY = MODELS / "gallery"
SIZE = 240


def snapshot(glb, atlas, out):
    import fbx_anim as fa
    import glb_anim
    import model_preview as mp
    import model_texture as mt
    model = glb_anim.load(str(glb))
    tex = mt.read_png(str(atlas))
    pos, uv, tris = fa.posed_mesh(model, model["clips"][0], 0.0) if model.get("clips") else fa.posed_mesh(model, None, 0.0)
    image = mp.draw(pos, uv, tris, tex, size=SIZE, yaw=-25, pitch=8)
    mt.write_png(str(out), SIZE, SIZE, [list(r) for r in image])


def snapshots(iso_path, part, refresh):
    iso = igb_read.XIso(iso_path)
    names = sorted(int(re.search(r"m(\d+)\.igb$", p).group(1)) for p in iso.files() if re.search(r"/monster/\d+/m\d+\.igb$", p))
    k, n = part
    GALLERY.mkdir(parents=True, exist_ok=True)
    for number in names[k - 1::n]:
        model = f"m{number:03d}"
        out = GALLERY / f"{model}.png"
        folder = next(iter(MODELS.glob(f"{model} *")), None)
        if out.is_file():
            continue                     # done already (delete it to draw it again)
        try:
            if folder is not None:
                if refresh:
                    dod_export.export(iso_path, model, folder)
                snapshot(folder / f"{model}.glb", folder / f"{model}_atlas.png", out)
            else:
                with tempfile.TemporaryDirectory() as tmp:
                    dod_export.export(iso_path, model, tmp)
                    snapshot(Path(tmp) / f"{model}.glb", Path(tmp) / f"{model}_atlas.png", out)
            print(model, "ok", flush=True)
        except Exception as error:      # one model that does not read is reported, the rest go on
            print(model, "FAILED", error, flush=True)


def page():
    rows = list(csv.DictReader(open(MODELS / "model_list.csv", encoding="utf-8")))
    cards = json.loads((ROOT / "mods/widescreen/cards.json").read_text(encoding="utf-8"))
    used_by = {}
    for c in cards:
        m = re.search(r"dotr-dod-models/(m\d+) ", str(c.get("model", "")))
        if m:
            used_by.setdefault(m.group(1), []).append(f"{c['number']} {c.get('name', '')}")
    cells = []
    for r in rows:
        model = r["model"]
        if r["dotr"]:
            status, cls = f"DotR has it (card {r['dotr']})", "have"
        elif not r["name"]:
            status, cls = "no card uses it (a part or effect)", "unused"
        else:
            status, cls = "DotR lacks it", "new"
        card = ", ".join(used_by.get(model, [])) or "-"
        img = f"gallery/{model}.png"
        cells.append(f'<figure class="{cls}"><img src="{img}" loading="lazy" alt="{model}">'
                     f'<figcaption><b>{model}</b> {html.escape(r["name"] or "(unnamed)")}<br>'
                     f'<span>{html.escape(status)}</span><br><span>our card: {html.escape(card)}</span></figcaption></figure>')
    counts = {k: sum(1 for c in cells if f'class="{k}"' in c) for k in ("new", "have", "unused")}
    (MODELS / "gallery.html").write_text(f"""<!doctype html><meta charset="utf-8"><title>Dawn of Destiny models</title>
<style>
body{{background:#1b1b1f;color:#ddd;font:13px sans-serif;margin:16px}}
.bar{{position:sticky;top:0;background:#1b1b1f;padding:8px 0}}
label{{margin-right:14px}}
main{{display:grid;grid-template-columns:repeat(auto-fill,minmax(250px,1fr));gap:10px}}
figure{{margin:0;background:#26262c;border-radius:6px;padding:6px;border-left:4px solid #555}}
figure.new{{border-color:#4caf50}} figure.have{{border-color:#2f6db5}} figure.unused{{border-color:#777}}
img{{width:100%;aspect-ratio:1;object-fit:contain;background:#555}}
span{{color:#aaa}}
</style>
<div class="bar"><b>{len(cells)} models</b> &nbsp;
<label><input type="checkbox" data-k="new" checked> DotR lacks ({counts['new']})</label>
<label><input type="checkbox" data-k="have" checked> DotR has ({counts['have']})</label>
<label><input type="checkbox" data-k="unused" checked> no card ({counts['unused']})</label>
<input id="q" placeholder="search name or number"></div>
<main>{''.join(cells)}</main>
<script>
const f=()=>{{const q=document.getElementById('q').value.toLowerCase();
document.querySelectorAll('figure').forEach(e=>{{const k=e.className;
const on=document.querySelector('[data-k='+k+']').checked;e.style.display=on&&e.textContent.toLowerCase().includes(q)?'':'none'}})}};
document.querySelectorAll('input').forEach(i=>i.addEventListener('input',f));
</script>""", encoding="utf-8")
    print("page:", MODELS / "gallery.html", counts)


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("iso")
    p.add_argument("--part", default="1/1")
    p.add_argument("--refresh", action="store_true")
    p.add_argument("--page", action="store_true")
    a = p.parse_args()
    if a.page:
        page()
    else:
        snapshots(a.iso, tuple(int(x) for x in a.part.split("/")), a.refresh)


if __name__ == "__main__":
    main()
