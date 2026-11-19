#!/usr/bin/env python3
"""A full dump of Yu-Gi-Oh! Cross Duel's monster models: every monster rig (an Animator named like
07991_01J; tokens 6-7 digits) in every bundle of the app (.apk) or its data (.zip or folder),
each as glTF with all its clips (tools/crossduel_export.py) and a PNG preview:

  models/cross/<Konami id> <name>/<rig>.glb, its textures, <rig>_preview.png
  models/cross/gallery.html, models.csv

  cross_dump.py APK_OR_DATA [--out "models/cross"] [--jobs N]
"""
import argparse
import csv
import html
import json
import re
import sys
import traceback
import zipfile
from multiprocessing import Pool
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
SOURCE = None


def slug(name):
    return re.sub(r"\s+", " ", re.sub(r'[<>:"/\\|?*]', "", name)).strip()


def bundles(source):
    source = Path(source)
    if source.is_dir():
        return [str(p) for p in sorted(source.rglob("*.bundle"))]
    with zipfile.ZipFile(source) as z:
        return [n for n in z.namelist() if n.endswith(".bundle")]


def read(name):
    if Path(name).is_file():
        return Path(name).read_bytes()
    with zipfile.ZipFile(SOURCE) as z:
        return z.read(name)


def init(source):
    global SOURCE
    SOURCE = source


def scan(name):
    """[(rig, bundle)] of the monster rigs in a bundle."""
    import UnityPy
    import crossduel_export as ce
    try:
        env = UnityPy.load(read(name))
        found = []
        for o in env.objects:
            if o.type.name == "Animator":
                try:
                    rig = o.read().m_GameObject.read().m_Name
                except Exception:
                    continue
                if ce.RIG.match(rig):
                    found.append((rig, name))
        return found
    except Exception:
        return []


def export(job):
    rig, name, folder = job
    import UnityPy
    import crossduel_export as ce
    import cross_models
    try:
        env = UnityPy.load(read(name))
        for o in env.objects:
            if o.type.name != "Animator":
                continue
            a = o.read()
            if a.m_GameObject.read().m_Name != rig:
                continue
            Path(folder).mkdir(parents=True, exist_ok=True)
            info = ce.export_rig(a, str(folder), rig)
            if not info:
                return dict(ok=False, error="nothing exported")
            textures = [t for t in info.get("textures", []) if t.endswith("_def.png")] or info.get("textures", [])
            preview = Path(folder) / f"{rig}_preview.png"
            if textures:
                cross_models.snapshot(Path(folder) / info["file"], Path(folder) / textures[0], preview)
            return dict(ok=True, file=info["file"], clips=len(info.get("clips", [])), meshes=info.get("meshes"),
                        preview=preview.is_file())
        return dict(ok=False, error="rig not found")
    except Exception as error:
        return dict(ok=False, error=repr(error), trace=traceback.format_exc()[-500:])


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("source")
    p.add_argument("--out", default=str(ROOT / "models" / "cross"))
    p.add_argument("--jobs", type=int, default=6)
    a = p.parse_args()
    import dod_models
    out = Path(a.out)
    names = {int(k): v["name"] for k, v in dod_models.konami_names().items()}
    # Names the database lacks (Rush Duel cards): the project's cards that use the rig.
    for c in json.loads((ROOT / "mods/widescreen/cards.json").read_text(encoding="utf-8")):
        m = re.search(r"/(\d{5,7})_\d\d[A-Z]\.", c.get("model") or "")
        if m and c.get("name"):
            names.setdefault(int(m.group(1)), c["name"])
    with Pool(a.jobs, initializer=init, initargs=(a.source,)) as pool:
        found = {}
        for hits in pool.imap_unordered(scan, bundles(a.source), chunksize=16):
            for rig, name in hits:
                found.setdefault(rig, name)
        print(f"{len(found)} monster rigs", flush=True)
        jobs, rows = [], []
        for rig in sorted(found):
            ident = int(rig.split("_")[0])
            label = names.get(ident, f"unknown {ident}")
            folder = out / slug(f"{rig.split('_')[0]} {label}")
            jobs.append((rig, found[rig], str(folder)))
            rows.append(dict(rig=rig, id=rig.split("_")[0], name=label, bundle=Path(found[rig]).name,
                             folder=folder.name))
        results = pool.map(export, jobs, chunksize=2)
    cells, failed = [], 0
    for row, res in zip(rows, results):
        row.update(ok=res["ok"], clips=res.get("clips", ""), error=res.get("error", ""))
        if not res["ok"]:
            failed += 1
            print("FAILED", row["rig"], res["error"], res.get("trace", ""))
            continue
        img = f"{row['folder']}/{row['rig']}_preview.png"
        cells.append(f'<figure><img src="{html.escape(img)}" loading="lazy"><figcaption>{html.escape(row["id"] + " " + row["name"])}'
                     f'<br><small>{row["rig"]} - {row["clips"]} clips</small></figcaption></figure>')
    out.mkdir(parents=True, exist_ok=True)
    with open(out / "models.csv", "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["rig", "id", "name", "bundle", "folder", "clips", "ok", "error"])
        w.writeheader()
        w.writerows(rows)
    (out / "gallery.html").write_text(
        "<!doctype html><meta charset=utf-8><title>Cross Duel models</title><style>body{background:#222;color:#ddd;"
        "font:13px sans-serif;margin:16px}figure{display:inline-block;width:256px;margin:4px;vertical-align:top}"
        f"img{{width:256px;height:256px}}small{{color:#999}}</style><h2>{len(cells)} models</h2>" + "".join(cells),
        encoding="utf-8")
    print(f"{len(rows) - failed} rigs dumped, {failed} failed -> {out}")


if __name__ == "__main__":
    main()
