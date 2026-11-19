#!/usr/bin/env python3
"""A full dump of Yu-Gi-Oh! Saikyo Card Battle's monster models, each as glTF with every animation
as the game has it (tools/bch_export.py, dump mode: no parts left out) and a PNG preview:
  3D/Mo_*.bch     the monsters with a 3D model (named by the game's table, build/saikyo/
                  model_table.json)
  3D/MBB_*.bch    the monsters the game shows as their card picture on an animated board (the
                  file is named by the card's Konami id): "(card picture)" in the folder name

  models/saikyo/<Konami id> <name>/<file>.glb, _atlas.png, _preview.png
  models/saikyo/gallery.html, models.csv

--all dumps every model instead, in groups: monsters (Mo_), monster stand-ins (MBB_),
characters (Ch_), arenas (Bt_), stages (St_), common (Co_).

  saikyo_dump.py ROMFS [--out "models/saikyo"] [--jobs N] [--all]
"""
import argparse
import csv
import html
import json
import re
import sys
import traceback
from multiprocessing import Pool
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
GROUPS = {"Mo": "monsters", "MBB": "monster stand-ins", "Ch": "characters", "Bt": "arenas", "St": "stages",
          "Co": "common"}


def slug(name):
    return re.sub(r"\s+", " ", re.sub(r'[<>:"/\\|?*]', "", name)).strip()


def preview(glb, atlas, out):
    import fbx_anim as fa
    import glb_anim
    import model_preview as mp
    import model_texture as mt
    model = glb_anim.load(glb)
    clips = model["clips"]
    clip = next((c for c in clips if c["name"].endswith("MotionB")), clips[0] if clips else None)
    pos, uv, tris = fa.posed_mesh(model, clip, 0.0)
    # The picture frames the body: parts the pose parks far away (props hidden until a summon
    # shows them) stay in the model but out of the picture.
    if pos:
        lo, hi = [], []
        for i in range(3):
            axis = sorted(p[i] for p in pos)
            lo.append(axis[len(axis) // 10])
            hi.append(axis[len(axis) * 9 // 10])
        size = max(hi[i] - lo[i] for i in range(3)) or 1.0
        far = lambda p: any(p[i] < lo[i] - size or p[i] > hi[i] + size for i in range(3))
        kept = [t for t in tris if not all(far(pos[c[0] if isinstance(c, tuple) else c]) for c in t)] or tris
        # Only the points the kept triangles use (the picture is sized to the points it is given).
        renum = {}
        new_pos, new_tris = [], []
        for t in kept:
            tri = []
            for c in t:
                pi, ui = (c if isinstance(c, tuple) else (c, c))
                if pi not in renum:
                    renum[pi] = len(new_pos)
                    new_pos.append(pos[pi])
                tri.append((renum[pi], ui))
            new_tris.append(tuple(tri))
        pos, tris = new_pos, new_tris
    tex = mt.read_png(str(atlas)) if atlas.is_file() else (1, 1, [[(200, 200, 200)]])
    image = mp.draw(pos, uv, tris, tex, size=256, yaw=-25, pitch=8)
    mt.write_png(str(out), 256, 256, [list(r) for r in image])


def one(job):
    path, folder, index = job
    import bch_export
    try:
        glb, report = bch_export.export(path, folder, dump=True, model_index=index)
        preview(glb, folder / f"{glb.stem}_atlas.png", folder / f"{glb.stem}_preview.png")
        clips = next((r for r in report if r.startswith("clips")), "")
        return dict(ok=True, glb=str(glb), clips=clips.split("(motions: ")[-1].rstrip(")") if clips else "")
    except Exception as error:
        return dict(ok=False, error=repr(error), trace=traceback.format_exc()[-600:])


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("romfs")
    p.add_argument("--out", default=str(ROOT / "models" / "saikyo"))
    p.add_argument("--jobs", type=int, default=6)
    p.add_argument("--all", action="store_true", help="every model, not only the monsters (Mo_, MBB_)")
    a = p.parse_args()
    import bch_read as br
    import dod_models
    konami = {int(k): v["name"] for k, v in dod_models.konami_names().items()}
    out = Path(a.out)
    table = {}
    tpath = ROOT / "build" / "saikyo" / "model_table.json"
    if tpath.is_file():
        for r in json.loads(tpath.read_text(encoding="utf-8")):
            table.setdefault(Path(r["model"]).stem, r)
    jobs, rows = [], []
    for path in sorted((Path(a.romfs) / "3D").glob("*.bch")):
        prefix = path.stem.split("_")[0]
        if prefix not in ("Mo", "MBB") and not a.all:
            continue
        group = GROUPS.get(prefix, "other")
        r = table.get(path.stem)
        if prefix == "MBB":
            # The file is named by the card's Konami id (the table's own id for it is another).
            cid = int(path.stem.split("_")[1])
            r = dict(cid=cid, name=konami.get(cid, "?"))
            label = f"{cid:05d} {r['name'] if r['name'] != '?' else 'unknown'} (card picture)"
        elif r:
            label = f"{r['cid']:05d} {r['name'] if r['name'] != '?' else 'unknown'}"
        else:
            label = path.stem
        try:
            count = len(br.Bch(path).dict("models"))
        except Exception:
            count = 0
        for i in range(count):
            folder = (out / group if a.all else out) / slug(label + (f" ({i})" if i else ""))
            jobs.append((path, folder, i))
            rows.append(dict(group=group, name=label, file=path.name, model=i, folder=str(folder.relative_to(out)),
                             cid=r["cid"] if r else "", card=r["name"] if r else ""))
    with Pool(a.jobs) as pool:
        results = pool.map(one, jobs, chunksize=2)
    failed = 0
    cells = {}
    for row, res in zip(rows, results):
        row["ok"] = res["ok"]
        row["clips"] = res.get("clips", "")
        if not res["ok"]:
            failed += 1
            row["error"] = res["error"]
            print("FAILED", row["file"], row["model"], res["error"], res.get("trace", ""))
            continue
        glb = Path(res["glb"])
        row["glb"] = str(glb.relative_to(out))
        # A monster of the game's table: the card export's picture (saikyo/gallery, made without
        # the props the summon brings, which a picture of the whole dump would shrink to fit).
        card_picture = (ROOT / "saikyo" / "gallery" / f"{row['cid']:05d}.png"
                        if row["cid"] != "" and row["group"] == "monsters" else None)
        if card_picture is not None and card_picture.is_file():
            import shutil
            shutil.copyfile(card_picture, glb.parent / f"{glb.stem}_preview.png")
        cells.setdefault(row["group"], []).append(
            f'<figure><img src="{html.escape(str((glb.parent / (glb.stem + "_preview.png")).relative_to(out)).replace(chr(92), "/"))}" '
            f'loading="lazy"><figcaption>{html.escape(row["name"])}<br><small>{html.escape(row["file"])} - '
            f'{html.escape(row["clips"] or "no animation")}</small></figcaption></figure>')
    with open(out / "models.csv", "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["group", "name", "cid", "card", "file", "model", "folder", "glb", "clips", "ok", "error"])
        w.writeheader()
        w.writerows(rows)
    body = "".join(f"<h2>{g} ({len(c)})</h2>" + "".join(c) for g, c in cells.items())
    (out / "gallery.html").write_text(
        "<!doctype html><meta charset=utf-8><title>Saikyo Card Battle models</title><style>body{background:#222;"
        "color:#ddd;font:13px sans-serif;margin:16px}figure{display:inline-block;width:256px;margin:4px;vertical-align:top}"
        "img{width:256px;height:256px}small{color:#999}</style>" + body, encoding="utf-8")
    print(f"{len(rows) - failed} models dumped, {failed} failed -> {out}")


if __name__ == "__main__":
    main()
