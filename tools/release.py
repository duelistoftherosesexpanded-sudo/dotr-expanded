#!/usr/bin/env python3
"""Makes the public release of Duelists of the Roses Expanded (no game data in it): the folder the
player's setup tool (tools/setup_tool.py, tools/setup_core.py) works from.

Run after a build (tools/mod.py ... --dev launcher/game: its SLUS_205.15, DLC.BIN and build.json),
with the developer's own discs, which give the expected SHA-256 of every model file a player's
tool will make:

  py -3.12 tools/release.py --dod DOD.iso --fbk FALSEBOUND.iso|.rvz --cross CROSS_DUEL.apk
                            --saikyo SAIKYO-decrypted.cci
                            [--out release/] [--iso DOTR.iso]

release/
  release.json        version, where the program and DLC.BIN's tables are
  SLUS_205.15.patch   the program's changes (DXP1, against the player's disc)
  DLC.BIN             the mod's code
  mods/widescreen/    cards.json and the mod's own files (title logo, wallpaper, placeholder
                      picture, the SPEX patch the tools apply when reading the game)
  manifest.json       each card: its model's source and recipe and the files' SHA-256; its
                      picture's recipe (Cross Duel illustration, card database, or none)
  pcsx2_graphics.json the developer's PCSX2 graphics settings, for the player's PCSX2 copy
                      (refresh only these: release.py --graphics)
  report.txt          what the release could not reproduce, and files that differ from the ones
                      this build used (the release's are what players get)

A card whose model comes from none of the three games ("other") is unobtainable for players.
"""

import argparse
import csv
import json
import re
import shutil
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import setup_core as core  # noqa: E402

ROOT = TOOLS.parent
MOD = ROOT / "mods" / "widescreen"
GAME = ROOT / "launcher" / "game"
VERSION = "0.55-beta"
# The mod's own files a player's build needs (no game data).
OWN_FILES = ["community.json", "title/expanded.png", "deck_wallpaper.png", "placeholder.png"]
OWN_DIRS = ["community"]


def recipe_of(model):
    """(game, recipe, folder relative to the project) of a cards.json model path."""
    rel = model[6:] if model.startswith("../../") else None
    if rel is None:
        return "other", None, None
    folder = rel.rsplit("/", 1)[0]
    if rel.startswith("dotr-dod-models/"):
        m = re.search(r"/(m\d{3})\.glb$", rel)
        return "dod", dict(game="dod", model=m.group(1)), folder
    if rel.startswith("picture frames/"):
        return "frame", dict(game="frame"), folder        # the art path is added by main()
    if rel.startswith("saikyo/"):
        m = re.search(r"/(Mo_\w+)\.glb$", rel)
        return "saikyo", dict(game="saikyo", model=m.group(1)), folder
    if rel.startswith("falsebound/"):
        m = re.search(r"/slot(\d{3})\.dat$", rel)
        return "fbk", dict(game="fbk", slot=int(m.group(1))), folder
    if rel.startswith("cross duel/"):
        m = re.search(r"/(\d{5,7}_\d\d[A-Z])\.glb$", rel)
        if m:
            return "cross", dict(game="cross", rig=m.group(1)), folder
    return "other", None, None


# The developer's PCSX2 graphics settings, which the player's PCSX2 copy is given: the [EmuCore/GS]
# section of their PCSX2.ini, then the game's own settings over it (gamesettings/SLUS-20515_*.ini,
# oldest first). Left out: what belongs to this computer, recording and dumping.
PCSX2_DATA = Path.home() / "Documents" / "PCSX2"
GRAPHICS_LEFT_OUT = re.compile(r"Adapter|.*Directory|ExclusiveFullscreenControl|UseDebugDevice|"
                               r"(Video|Audio)Capture.*|EnableVideoCapture.*|EnableAudioCapture.*|CaptureContainer|"
                               r"[Dd]ump.*|GSDumpCompression|save[a-z]?|savel|png_compression_level|Screenshot.*|"
                               r"Show_Messages.*|Global_Enable")


def pcsx2_graphics(data=PCSX2_DATA):
    """{key: value} of the developer's graphics settings (see GRAPHICS_LEFT_OUT)."""
    import configparser
    settings = {}
    files = [data / "inis" / "PCSX2.ini"] + sorted((data / "gamesettings").glob("SLUS-20515_*.ini"),
                                                     key=lambda p: p.stat().st_mtime)
    for path in files:
        if not path.is_file():
            continue
        ini = configparser.RawConfigParser(strict=False, interpolation=None, delimiters=("=",))
        ini.optionxform = str
        ini.read(path, encoding="utf-8-sig")
        if ini.has_section("EmuCore/GS"):
            settings.update({k: v for k, v in ini.items("EmuCore/GS") if not GRAPHICS_LEFT_OUT.fullmatch(k)})
    return settings


# Settings players get whatever the developer's are: 3x resolution, which most computers keep up with.
PLAYER_GRAPHICS = {"upscale_multiplier": "3"}


def write_graphics(out):
    graphics = {**pcsx2_graphics(), **PLAYER_GRAPHICS}
    (Path(out) / "pcsx2_graphics.json").write_text(json.dumps(graphics, indent=1), encoding="utf-8")
    print(f"PCSX2 graphics settings: {len(graphics)} (release/pcsx2_graphics.json)")


def main():
    if sys.argv[1:] and sys.argv[1] == "--graphics":
        # Only the graphics settings, into the release already made.
        write_graphics(sys.argv[2] if len(sys.argv) > 2 else ROOT / "release")
        return
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--dod", required=True)
    p.add_argument("--fbk", required=True)
    p.add_argument("--cross", required=True)
    p.add_argument("--saikyo", required=True, help="Saikyo Card Battle, decrypted (.cci)")
    p.add_argument("--iso", help="the clean Duelists of the Roses disc (default: baserom/)")
    p.add_argument("--out", default=str(ROOT / "release"))
    p.add_argument("--sync", action="store_true",
                   help="copy the model files the release makes into the project, so this build uses exactly "
                        "what players get (build again afterwards)")
    a = p.parse_args()
    out = Path(a.out)
    if out.exists():
        shutil.rmtree(out)
    (out / "mods" / "widescreen").mkdir(parents=True)
    report = []
    build = json.loads((GAME / "build.json").read_text())
    for symbol in ("dlc_card_status", "dlc_give_all"):
        if symbol not in build.get("symbols", {}):
            raise SystemExit(f"launcher/game/build.json has no {symbol}: build again (tools/mod.py ... --dev launcher/game)")

    # The program and the code.
    iso = Path(a.iso) if a.iso else next(ROOT.glob("baserom/*.iso"))
    changed = (GAME / "SLUS_205.15").read_bytes()
    with open(iso, "rb") as f:
        f.seek(build["iso_extent"])
        original = f.read(len(changed))
    (out / "SLUS_205.15.patch").write_bytes(core.make_patch(original, changed))
    code = bytearray((GAME / "DLC.BIN").read_bytes())
    code[0x44:0x50] = bytes(12)        # the models' place is set when the player's disc is made
    # Players start without the free cards (give-all stays as the Card Maker sets it in the dev build).
    give_all = build["symbols"]["dlc_give_all"]
    code[give_all:give_all + 2] = bytes(2)
    (out / "DLC.BIN").write_bytes(bytes(code))
    (out / "release.json").write_text(json.dumps(dict(
        version=VERSION,
        slus=dict(iso_extent=build["iso_extent"], size=len(changed), original_sha256=core.sha256(original)),
        dlc=dict(info_offset=build["info_offset"], status_offset=build["symbols"]["dlc_card_status"])), indent=1))
    write_graphics(out)

    # The mod's own files.
    for f in OWN_FILES:
        if (MOD / f).is_file():
            (out / "mods/widescreen" / f).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(MOD / f, out / "mods/widescreen" / f)
    for d in OWN_DIRS:
        if (MOD / d).is_dir():
            shutil.copytree(MOD / d, out / "mods/widescreen" / d)
    # The description of the game's program (sections, addresses; no game data), which the
    # project's tools read (research.verified_elf).
    (out / "out/ntsc-u").mkdir(parents=True)
    for f in ("elf.json", "disc.json"):
        shutil.copy2(ROOT / "out/ntsc-u" / f, out / "out/ntsc-u" / f)
    cards = json.loads((MOD / "cards.json").read_text(encoding="utf-8"))
    (out / "mods/widescreen/cards.json").write_text(json.dumps(cards, indent=2) + "\n", encoding="utf-8", newline="\n")

    # Each card's model: its recipe, run here as the player's tool will, and the files' hashes.
    sources = {k: core.open_source(k, v, out.parent / "release-work") for k, v in
               (("dod", a.dod), ("fbk", a.fbk), ("cross", a.cross), ("saikyo", a.saikyo))}
    sources["frame"] = MOD                # picture frames are made from the card's art
    # Rigs of the Cross Duel data, then those only the Cross Duel app (APK) has (the app holds
    # every bundle of the data too; a player who gives only the data cannot obtain the app's).
    cross_rigs, app_only = {}, set()
    for export, app in (("cross duel export", False), ("cross duel export apk", True)):
        path = ROOT / "build" / export / "models.json"
        if path.is_file():
            for rig, info in json.loads(path.read_text(encoding="utf-8")).items():
                if rig not in cross_rigs:
                    cross_rigs[rig] = info["bundle"]
                    if app:
                        app_only.add(rig)
    entries, done = [], {}
    work = Path(tempfile.mkdtemp(prefix="dotr-release-"))
    for c in cards:
        if "number" not in c or not c.get("model"):
            continue
        game, recipe, folder = recipe_of(c["model"])
        e = dict(number=c["number"], name=c.get("name") or "", game=game)
        if game == "frame":
            recipe["art"] = c.get("art") or "placeholder.png"
            if isinstance(c.get("model_frame"), list):
                recipe["size"] = c["model_frame"]      # the donor's texture the frame was made for
        if game == "cross":
            recipe["bundle"] = cross_rigs.get(recipe["rig"])
            if recipe["bundle"] is None:
                game = e["game"] = "other"
            elif recipe["rig"] in app_only:
                recipe["app"] = True
        if game != "other":
            e["recipe"], e["folder"] = recipe, folder
            key = json.dumps(recipe, sort_keys=True)
            if key not in done:
                target = work / folder
                print(f"{c['number']} {e['name']}: {game} {recipe}", flush=True)
                core.run_recipe(recipe, sources[game], target)
                done[key] = target
            files = {}
            for field in ("model", "model_texture"):
                if c.get(field):
                    rel = c[field][6:]
                    made = work / rel
                    if not made.is_file():
                        report.append(f"card {c['number']} {e['name']}: the recipe does not make {rel}")
                        e["game"] = "other"
                        break
                    files[rel] = core.sha256_file(made)
                    mine = ROOT / rel
                    if mine.is_file() and core.sha256_file(mine) != files[rel]:
                        if a.sync:
                            shutil.copy2(made, mine)
                            report.append(f"card {c['number']} {e['name']}: {rel} replaced by the release's")
                        else:
                            report.append(f"card {c['number']} {e['name']}: {rel} differs from this build's "
                                          f"(players get the release's; --sync copies it here)")
            e["files"] = files
        entries.append(e)

    # Each new card's picture.
    import card_lookup
    match = card_lookup.Matcher(card_lookup.database())
    illus = json.loads((ROOT / "cross duel/illustrations.json").read_text()) if (ROOT / "cross duel/illustrations.json").is_file() else {}
    by_number = {e["number"]: e for e in entries}
    for c in cards:
        if "number" not in c or not c.get("art") or c["art"] == "placeholder.png":
            continue
        e = by_number.setdefault(c["number"], dict(number=c["number"], name=c.get("name") or ""))
        if not e.get("game"):
            entries.append(e)
        m = re.search(r"cross duel/(\d{5,7}) ", c.get("model", ""))
        name = c.get("name") or ""
        real, exact = match.find(card_lookup.ALIASES.get(name.strip().lower(), name)) if name else (None, False)
        if m and m.group(1) in illus and c["art"] == f"art/{core.slug(name)}.png" and not exact:
            e["art"] = dict(path=c["art"], cross_illust=illus[m.group(1)])
        elif real is not None and exact:
            e["art"] = dict(path=c["art"], ygoprodeck=real["id"])
        else:
            e["art"] = dict(path=c["art"])
            report.append(f"card {c['number']} {name}: its picture {c['art']} has no source players have "
                          f"(they see the placeholder)")
    (out / "manifest.json").write_text(json.dumps(dict(version=VERSION, cards=entries), indent=1), encoding="utf-8")
    counts = {}
    for e in entries:
        if e.get("game"):
            counts[e["game"]] = counts.get(e["game"], 0) + 1
    report.insert(0, f"Duelists of the Roses Expanded {VERSION}: card models by source {counts}")
    (out / "report.txt").write_text("\n".join(report) + "\n", encoding="utf-8")
    shutil.rmtree(work, ignore_errors=True)
    print("\n".join(report[:40]))
    print(f"release written to {out}")


if __name__ == "__main__":
    main()
