#!/usr/bin/env python3
"""Duelists of the Roses Expanded, put together on a player's computer from the player's own discs:
what tools/release.py (the release) and tools/setup_tool.py (the player's window) share.

The release ships no game data. It has the mod's own work: the program's changes as a patch
(SLUS_205.15.patch, against the player's Duelists of the Roses), DLC.BIN (the mod's code), the mod's
files (cards.json, its own pictures, ...), and manifest.json, which says for each card where its
3D model and picture come from (a "recipe") and the SHA-256 of every file the recipe makes, as
this build has them.

The model sources, each optional:
  dod    Yu-Gi-Oh! The Dawn of Destiny (Xbox disc image, .iso)
  fbk    Yu-Gi-Oh! The Falsebound Kingdom (GameCube disc image, .iso/.gcm; .rvz is converted with
         Dolphin's DolphinTool when it is installed)
  cross  Yu-Gi-Oh! Cross Duel: the app (.apk), which holds every bundle of its data and more
  saikyo Yu-Gi-Oh! Duel Monsters Saikyo Card Battle: its eShop download (decrypted by the player's
         own decryptor, decrypt_saikyo) or a decrypted image
A card whose source is not given cannot be obtained in the game and plays its donor's model; one
whose files differ from this build's is either disabled or kept without its model (the player's
choice). Pictures come from the same sources or from the card database (YGOPRODeck, online).

The workspace is laid out as the project (DOTR_ROOT), so the project's own tools build the disc:
mods/widescreen/ (the release's mod files), config/, out/ntsc-u/SLUS_205.15 (the player's original
program) and the extracted models at the paths cards.json names.
"""

import hashlib
import io
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import zipfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

SOURCES = {"dod": "Yu-Gi-Oh! The Dawn of Destiny (Xbox)",
           "fbk": "Yu-Gi-Oh! The Falsebound Kingdom (GameCube)",
           "cross": "Yu-Gi-Oh! Cross Duel (app or data)",
           "saikyo": "Yu-Gi-Oh! Duel Monsters Saikyo Card Battle (3DS)"}
PATCH_MAGIC = b"DXP1"
MODEL_SLOT = 0x200000


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def slug(name):
    return re.sub(r"[^a-z0-9]+", "_", name.lower()).strip("_")


# ---------------------------------------------------------------------------- patches

def make_patch(original, changed):
    """The runs where `changed` differs from `original` (same length), as DXP1 bytes."""
    if len(original) != len(changed):
        raise ValueError("the patch needs files of the same size")
    runs, i, n = [], 0, len(original)
    while i < n:
        if original[i] == changed[i]:
            i += 1
            continue
        j = i
        # A run ends after 16 equal bytes in a row (shorter gaps are cheaper inside the run).
        while j < n:
            if original[j] != changed[j]:
                j += 1
                continue
            k = j
            while k < n and k - j < 16 and original[k] == changed[k]:
                k += 1
            if k - j >= 16 or k == n:
                break
            j = k
        runs.append((i, changed[i:j]))
        i = j
    out = bytearray(PATCH_MAGIC + struct.pack("<II", n, len(runs)))
    for at, data in runs:
        out += struct.pack("<II", at, len(data)) + data
    return bytes(out)


def apply_patch(original, patch):
    if patch[:4] != PATCH_MAGIC:
        raise ValueError("not a DXP1 patch")
    size, count = struct.unpack_from("<II", patch, 4)
    if len(original) != size:
        raise ValueError("the patch is for a file of another size")
    out, at = bytearray(original), 12
    for _ in range(count):
        offset, length = struct.unpack_from("<II", patch, at)
        at += 8
        out[offset:offset + length] = patch[at:at + length]
        at += length
    return bytes(out)


# ---------------------------------------------------------------------------- sources

class GameCubeDisc:
    """A GameCube disc image (.iso/.gcm): its files by path, from the file system table."""

    def __init__(self, path):
        self.path = Path(path)
        with open(self.path, "rb") as f:
            head = f.read(0x440)
            self.game_id = head[:6].decode("ascii", "replace")
            self.title = head[0x20:0x60].split(b"\0")[0].decode("ascii", "replace")
            fst_at, fst_size = struct.unpack_from(">II", head, 0x424)
            f.seek(fst_at)
            fst = f.read(fst_size)
        count = struct.unpack_from(">I", fst, 8)[0]
        names = fst[12 * count:]
        self.files = {}

        def name_at(i):
            off = struct.unpack_from(">I", fst, 12 * i)[0] & 0xFFFFFF
            return names[off:names.index(b"\0", off)].decode("latin-1")

        def walk(first, end, prefix):
            i = first
            while i < end:
                flags = fst[12 * i]
                a, b = struct.unpack_from(">II", fst, 12 * i + 4)
                name = prefix + "/" + name_at(i)
                if flags:
                    walk(i + 1, b, name)
                    i = b
                else:
                    self.files[name] = (a, b)
                    i += 1
        walk(1, count, "")

    def read(self, name):
        at, size = self.files[name]
        with open(self.path, "rb") as f:
            f.seek(at)
            return f.read(size)


def find_dolphin_tool():
    for p in (os.environ.get("ProgramFiles", r"C:\Program Files"), os.environ.get("ProgramFiles(x86)", ""),
              os.path.join(os.environ.get("LOCALAPPDATA", ""), "Programs")):
        for name in ("Dolphin", "Dolphin-x64", "Dolphin Emulator"):
            tool = Path(p) / name / "DolphinTool.exe"
            if tool.is_file():
                return tool
    found = shutil.which("DolphinTool")
    if found:
        return Path(found)
    # A portable Dolphin unpacked somewhere under the Desktop or Downloads (a few folders deep).
    for top in (Path.home() / "Desktop", Path.home() / "Downloads"):
        for depth in range(1, 5):
            for tool in top.glob("/".join(["*"] * depth) + "/DolphinTool.exe"):
                return tool
    return None


DOLPHIN_TOOL = None     # set by the player's window when they point at DolphinTool.exe


def open_falsebound(path, workspace, say=print):
    """The Falsebound Kingdom disc as a GameCubeDisc; an .rvz is converted to an .iso first."""
    path = Path(path)
    if path.suffix.lower() in (".rvz", ".wia", ".gcz"):
        out = Path(workspace) / "falsebound.iso"
        if not out.is_file():
            tool = DOLPHIN_TOOL or find_dolphin_tool()
            if tool is None:
                raise ValueError("This disc is compressed (.rvz). Install Dolphin, or convert it to .iso in "
                                 "Dolphin (right-click the game > Convert File... > ISO), and choose the .iso.")
            say("Converting the Falsebound Kingdom disc to .iso with Dolphin (a minute)...")
            subprocess.run([str(tool), "convert", "-i", str(path), "-o", str(out), "-f", "iso"], check=True,
                           creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
        path = out
    disc = GameCubeDisc(path)
    if "/Model/MONSTER.MRG" not in disc.files:
        raise ValueError(f"{path.name} is not Yu-Gi-Oh! The Falsebound Kingdom (no Model/MONSTER.MRG)")
    return disc


class CrossData:
    """Cross Duel's bundles: the app (.apk, a zip), or a .zip or folder of its data."""

    def __init__(self, path):
        self.path = Path(path)
        self.zip = None
        self.bundles = {}
        if self.path.is_dir():
            for p in self.path.rglob("*.bundle"):
                self.bundles[p.name] = p
        else:
            self.zip = zipfile.ZipFile(self.path)
            for n in self.zip.namelist():
                if n.lower().endswith(".bundle"):
                    self.bundles[n.rsplit("/", 1)[-1]] = n
        if not self.bundles:
            raise ValueError(f"{self.path.name} holds no Cross Duel .bundle files")

    def read(self, name):
        where = self.bundles[name]
        return where.read_bytes() if self.zip is None else self.zip.read(where)


# ---------------------------------------------------------------------------- Saikyo Card Battle
# The game as bought (the eShop download) is encrypted. The setup tool does not decrypt anything
# itself: it runs the player's own decryptor (a folder with decrypt.exe and makerom.exe and the
# batch file that drives them), and only on Saikyo Card Battle, by its title id.
SAIKYO_TITLE_ID = 0x0004000000188500
SAIKYO_DECRYPTOR = None              # the player's decryptor folder, once found or chosen


def saikyo_title_id(path):
    """The title id of a 3DS file: the eShop download's (its title metadata) or a decrypted image's
    (its first partition's program id); None for anything else."""
    with open(path, "rb") as f:
        head = f.read(0x20)
        f.seek(0x100)
        if f.read(4) == b"NCSD":
            f.seek(0x120)
            part = struct.unpack("<I", f.read(4))[0] * 0x200
            f.seek(part + 0x118)
            return struct.unpack("<Q", f.read(8))[0]
        header_size, _, _, cert, ticket, tmd = struct.unpack_from("<IHHIII", head)
        if header_size != 0x2020:
            return None
        align = lambda x: (x + 63) & ~63
        f.seek(align(header_size) + align(cert) + align(ticket))
        t = f.read(tmd)
    sig = struct.unpack(">I", t[:4])[0]
    skip = {0x10000: 0x23C, 0x10001: 0x13C, 0x10002: 0x7C, 0x10003: 0x23C, 0x10004: 0x13C, 0x10005: 0x7C}.get(sig)
    if skip is None or len(t) < skip + 0x54:
        return None
    return struct.unpack(">Q", t[skip + 0x4C:skip + 0x54])[0]


def is_decryptor(folder):
    folder = Path(folder)
    return (folder / "decrypt.exe").is_file() and (folder / "makerom.exe").is_file() and any(folder.glob("*.bat"))


def find_decryptor():
    """The player's decryptor folder: the one chosen before, or one on the Desktop or in Downloads."""
    global SAIKYO_DECRYPTOR
    if SAIKYO_DECRYPTOR and is_decryptor(SAIKYO_DECRYPTOR):
        return Path(SAIKYO_DECRYPTOR)
    for top in (Path.home() / "Desktop", Path.home() / "Downloads"):
        for exe in top.glob("**/decrypt.exe") if top.is_dir() else []:
            if len(exe.relative_to(top).parts) <= 4 and is_decryptor(exe.parent):
                SAIKYO_DECRYPTOR = exe.parent
                return exe.parent
    return None


def decrypt_saikyo(path, workspace, say=print):
    """The decrypted image of the player's Saikyo Card Battle download, made once in the workspace
    with the player's own decryptor (find_decryptor)."""
    path = Path(path)
    if saikyo_title_id(path) != SAIKYO_TITLE_ID:
        raise ValueError(f"{path.name} is not Yu-Gi-Oh! Duel Monsters Saikyo Card Battle")
    out = Path(workspace) / "saikyo" / f"saikyo-{path.stat().st_size}.cci"
    if out.is_file():
        return out
    tool = find_decryptor()
    if tool is None:
        raise ValueError("Saikyo Card Battle needs decrypting: choose your decryptor's folder")
    work = Path(workspace) / "saikyo" / "decrypt"
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    for f in tool.iterdir():
        if f.is_file() and f.suffix.lower() in (".exe", ".bat"):
            shutil.copy2(f, work / f.name)
    shutil.copy2(path, work / "saikyo.cia")
    say("Decrypting Saikyo Card Battle with your decryptor (a few minutes)...")
    script = next(work.glob("*.bat"))
    subprocess.run(["cmd", "/c", script.name], cwd=work, input=b"\r\n", capture_output=True, timeout=3600)
    made = work / "saikyo-decrypted.cci"
    if not made.is_file():
        raise ValueError("the decryptor made no decrypted image (see its log.txt in " + str(work) + ")")
    out.parent.mkdir(parents=True, exist_ok=True)
    shutil.move(str(made), out)
    shutil.rmtree(work, ignore_errors=True)
    return out


def open_source(kind, path, workspace, say=print):
    if kind == "dod":
        import igb_read
        iso = igb_read.XIso(path)
        files = iso.files()
        if not any(re.search(r"/monster/\d+/m\d+\.igb$", p) for p in files):
            raise ValueError(f"{Path(path).name} is not Yu-Gi-Oh! The Dawn of Destiny (no monster models)")
        return str(path)          # dod_export reads the disc by its path
    if kind == "fbk":
        return open_falsebound(path, workspace, say)
    if kind == "cross":
        return CrossData(path)
    if kind == "saikyo":
        import ctr_read
        if Path(path).suffix.lower() == ".cia":   # the eShop download: decrypted first
            path = decrypt_saikyo(path, workspace, say)
        cci = ctr_read.Cci(path)              # says so if the image is still encrypted
        if "/3D/Mo_Kuriboh.bch" not in cci.files():
            raise ValueError(f"{Path(path).name} is not Yu-Gi-Oh! Saikyo Card Battle (no monster models)")
        return cci
    raise ValueError(kind)


# ---------------------------------------------------------------------------- recipes

def has_model(card, source):
    """Whether an opened source holds the card's model: Cross Duel's data has fewer bundles than
    the Cross Duel app (APK), whose own models' recipes say "app"."""
    if card.get("game") == "cross" and isinstance(source, CrossData):
        return card["recipe"]["bundle"] in source.bundles
    return True


def run_recipe(recipe, source, folder):
    """Makes a model's files in `folder` from its source (an opened source of recipe["game"])."""
    folder = Path(folder)
    folder.mkdir(parents=True, exist_ok=True)
    game = recipe["game"]
    if game == "dod":
        import dod_export
        dod_export.export(source, recipe["model"], folder)
    elif game == "fbk":
        import hsd_anim
        slot = recipe["slot"]
        mrg = source.read("/Model/MONSTER.MRG")
        raw = mrg[slot * 0x80000:(slot + 1) * 0x80000]
        size = struct.unpack_from(">I", raw, 0)[0]
        dat = folder / f"slot{slot:03d}.dat"
        dat.write_bytes(raw[:size])
        hsd_anim.atlas(dat, folder / f"slot{slot:03d}_atlas.png")
    elif game == "cross":
        import UnityPy
        import crossduel_export
        env = UnityPy.load(source.read(recipe["bundle"]))
        for o in env.objects:
            if o.type.name != "Animator":
                continue
            a = o.read()
            if a.m_GameObject.read().m_Name == recipe["rig"]:
                crossduel_export.export_rig(a, str(folder), recipe["rig"])
                break
        else:
            raise ValueError(f"rig {recipe['rig']} is not in {recipe['bundle']}")
    elif game == "saikyo":
        import bch_export
        name = f"{recipe['model']}.bch"
        bch_export.export(name, folder, data=source.read(f"3D/{name}"))
    elif game == "frame":
        # A picture frame made from the card's art (source: the mod folder the art path is in).
        import frame_model
        art = Path(source) / recipe["art"]
        if not art.is_file():
            raise ValueError(f"no picture {recipe['art']} to frame")
        frame_model.make(art, folder, size=tuple(recipe.get("size") or (256, 128)))
    else:
        raise ValueError(f"unknown recipe {recipe}")


def recipe_art(recipe, sources, out):
    """A card's picture by its art recipe, saved as PNG at out; False when it cannot be made."""
    from PIL import Image
    if "ygoprodeck" in recipe:
        import urllib.request
        url = f"https://images.ygoprodeck.com/images/cards_cropped/{recipe['ygoprodeck']}.jpg"
        req = urllib.request.Request(url, headers={"User-Agent": "dotr-expanded-setup/1.0"})
        data = urllib.request.urlopen(req, timeout=60).read()
        image = Image.open(io.BytesIO(data)).convert("RGB")
        image.thumbnail((512, 512))
    elif "cross_illust" in recipe:
        if "cross" not in sources:
            return False
        import UnityPy
        env = UnityPy.load(sources["cross"].read(recipe["cross_illust"]["bundle"]))
        image = next(o.read().image for o in env.objects if o.path_id == recipe["cross_illust"]["path_id"]).convert("RGB")
    else:
        return False
    Path(out).parent.mkdir(parents=True, exist_ok=True)
    image.save(out, "PNG")
    return True


def check_files(workspace, files):
    """{path: (expected, found or None)} of the files that differ from the release's."""
    bad = {}
    for rel, want in files.items():
        p = Path(workspace) / rel
        got = sha256_file(p) if p.is_file() else None
        if got != want:
            bad[rel] = (want, got)
    return bad


# ---------------------------------------------------------------------------- a player's build

def gather(release, workspace, dotr_iso, paths, ask, say=print):
    """Extracts every card's model and picture from the games given and sets the workspace's
    cards.json up for the build. paths: {"dod"/"fbk"/"cross": path}. ask({number: (name, files)})
    answers {number: "disabled" | "keep"} for cards whose files differ from the release's.
    Returns statuses {number: "unobtainable" | "disabled"} (the others are normal)."""
    release, workspace = Path(release), Path(workspace)
    cards = json.loads((release / "manifest.json").read_text(encoding="utf-8"))["cards"]
    prepare_workspace(release, workspace, dotr_iso)
    sources = {k: open_source(k, p, workspace, say) for k, p in paths.items() if p}
    statuses, missing, bad, made = {}, set(), {}, {}
    for c in cards:
        game = c.get("game")
        if not game or game == "frame":          # frames are made from the pictures, below
            continue
        if game not in sources or not has_model(c, sources[game]):
            statuses[c["number"]] = "unobtainable"
            missing.add(c["number"])
            continue
        key = json.dumps(c["recipe"], sort_keys=True)
        if key not in made:
            say(f"Extracting {c['name'] or c['number']} from {SOURCES[game]}...")
            try:
                run_recipe(c["recipe"], sources[game], workspace / c["folder"])
                made[key] = True
            except Exception as error:
                made[key] = False
                say(f"   could not: {error}")
        differ = check_files(workspace, c["files"]) if made[key] else {f: None for f in c["files"]}
        if differ:
            bad[c["number"]] = (c["name"], differ)
    if bad:
        say(f"{len(bad)} cards' models differ from this release.")
        for number, choice in ask(bad).items():
            missing.add(number)
            if choice == "disabled":
                statuses[number] = "disabled"
    pictures = 0
    for c in cards:
        art = c.get("art")
        if not art or not art.get("ygoprodeck") and not art.get("cross_illust"):
            continue
        try:
            if recipe_art(art, sources, workspace / "mods/widescreen" / art["path"]):
                pictures += 1
        except Exception as error:
            say(f"No picture for {c['name']}: {error}")
    say(f"Card pictures: {pictures}.")
    # Picture-frame models (tools/frame_model.py) from the pictures just made. Not checked byte for
    # byte (a picture from the card database may differ a little from the release's); a card
    # whose picture could not be had keeps its donor's model.
    frames = 0
    for c in cards:
        if c.get("game") != "frame":
            continue
        try:
            run_recipe(c["recipe"], workspace / "mods/widescreen", workspace / c["folder"])
            frames += 1
        except Exception as error:
            missing.add(c["number"])
            say(f"No picture frame for {c['name']}: {error}")
    if frames:
        say(f"Picture-frame models: {frames}.")
    path = workspace / "mods/widescreen/cards.json"
    listed = json.loads(path.read_text(encoding="utf-8"))
    for e in listed:
        n = e.get("number")
        if n in missing and e.get("model"):
            e["model_missing"] = True
        if n in statuses and n >= 871:
            e["status"] = statuses[n]
        if e.get("art") and e["art"] != "placeholder.png" and not (workspace / "mods/widescreen" / e["art"]).is_file():
            e["art"] = "placeholder.png"
    path.write_text(json.dumps(listed, indent=2) + "\n", encoding="utf-8")
    return {n: s for n, s in statuses.items() if n >= 871}


# ---------------------------------------------------------------------------- the disc

def prepare_workspace(release, workspace, dotr_iso):
    """The workspace laid out as the project: the release's mod files and settings, and the
    player's original program (checked against the release)."""
    release, workspace = Path(release), Path(workspace)
    info = json.loads((release / "release.json").read_text())
    if (workspace / "mods").exists():
        shutil.rmtree(workspace / "mods")
    shutil.copytree(release / "mods", workspace / "mods")
    with open(dotr_iso, "rb") as f:
        f.seek(info["slus"]["iso_extent"])
        exe = f.read(info["slus"]["size"])
    if sha256(exe) != info["slus"]["original_sha256"]:
        raise ValueError("This is not the North American Yu-Gi-Oh! The Duelists of the Roses (SLUS-20515), "
                         "or the disc image was changed.")
    (workspace / "out/ntsc-u").mkdir(parents=True, exist_ok=True)
    (workspace / "out/ntsc-u/SLUS_205.15").write_bytes(exe)
    for f in ("elf.json", "disc.json"):
        shutil.copy2(release / "out/ntsc-u" / f, workspace / "out/ntsc-u" / f)
    return info, exe


def build_disc(release, workspace, dotr_iso, target, statuses, say=print):
    """The finished disc image at `target`: the player's Duelists of the Roses with the mod's
    program, DLC.BIN (each new card's status written in, the models after the code), the new
    cards' pictures, the title and the second world's map. statuses: {card: "normal" |
    "unobtainable" | "disabled"}. Cards flagged "model_missing" in the workspace's cards.json play
    their donor's model."""
    os.environ["DOTR_ROOT"] = str(workspace)
    import dlc_cards
    import dlc_title
    import dlc_world
    import iso_files
    release, workspace = Path(release), Path(workspace)
    info = json.loads((release / "release.json").read_text())
    mod = workspace / "mods" / "widescreen"
    cards_path = mod / "cards.json"
    say("Copying your Duelists of the Roses disc...")
    shutil.copyfile(dotr_iso, target)
    original = (workspace / "out/ntsc-u/SLUS_205.15").read_bytes()
    image = bytearray(apply_patch(original, (release / "SLUS_205.15.patch").read_bytes()))
    code = bytearray((release / "DLC.BIN").read_bytes())
    at = info["dlc"]["status_offset"]
    for number, status in statuses.items():
        code[at + int(number)] = dlc_cards.STATUSES.index(status)
    disc = iso_files.Image(Path(target))
    try:
        def whole(path):
            _, _, entry = disc.lookup(path)
            start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
            return disc.read(start, (size + 2047) // 2048)[:size]
        _, _, entry = disc.lookup("/DATA/MONSTER.MRG")
        monsters = struct.unpack_from("<I", entry, 2)[0]
        say("Building the 3D models (several minutes)...")
        slots, _ = dlc_cards.models(cards_path, lambda card: disc.read(monsters + card * 512, 512))
        contents = bytes(code)
        if slots:
            start = (len(code) + 2047) & ~2047
            struct.pack_into("<I", code, 0x44, start)
            contents = bytes(code) + bytes(start - len(code)) + b"".join(slots)
        sector = disc.put("/DLC.BIN", contents)
        say("Building the card pictures...")
        clean_pictures = whole("/DATA/PICTURE.MRG")
        disc.put("/DATA/PICPACK.MRG", dlc_cards.picpack(cards_path, whole("/DATA/PICPACK.MRG"), clean_pictures))
        disc.put("/DATA/PICTURE.MRG", dlc_cards.pictures(cards_path, clean_pictures))
        disc.put("/DATA/PICMINI.MRG", dlc_cards.minis(cards_path, whole("/DATA/PICMINI.MRG")))
        if (mod / "title" / "expanded.png").is_file():
            disc.put("/DATA/TEXETC.MRG", dlc_title.compose(whole("/DATA/TEXETC.MRG"), mod / "title" / "expanded.png"))
        say("Building the second world's map...")
        models_mrg, _ = dlc_world.extend(whole("/DATA/MODEL.MRG"), bytes(image))
        disc.put("/DATA/MODEL.MRG", models_mrg)
    finally:
        disc.close()
    struct.pack_into("<III", image, info["dlc"]["info_offset"], sector, len(code), 0)
    with open(target, "r+b") as f:
        f.seek(info["slus"]["iso_extent"])
        f.write(image)
    say(f"Done: {target}")
    return target
