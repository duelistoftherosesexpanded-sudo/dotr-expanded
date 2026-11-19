#!/usr/bin/env python3
"""Build a modified SLUS_205.15 from the replacement sources of one mod (not a matching build).

A mod is a directory `mods/<name>/` holding replacement sources at the same relative paths as the
originals (`mods/<name>/src/ee/view_state/func_0020df90.c` replaces that function; the same path
under `asm/` with `.s` replaces a C function by whole-function assembly). Each replaced function is
compiled with the pinned compiler exactly as `make match` does, must fit between its original
address and the next thing in the code span, and is written over the original bytes in a copy of
your prepared executable. Nothing else moves: this is the stage-1 layout, so a function may only
grow into the padding that follows it.

A replacement source may define further functions after the replaced one; they are placed with it
and must fit in the same room. An optional `mods/<name>/mod.json` holds:

  "globals":  {"<replaced function>": {"<symbol>": "<address>"}}   extra symbols for its link
  "redirect": [{"from": "<function>", "to": "<function the mod defines>", "in": ["<caller>", ...]}]
              every call (jal) to `from` inside the listed callers is pointed at `to`

Output goes to the ignored `build/ntsc-u/mods/<name>[-<variant>]/`: the patched executable, a
report, and with `--make-iso` a patched copy of your disc image (your own image is only read).
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dotr.elf import file_offset, inventory, symbol_table  # noqa: E402
import iso_files  # noqa: E402
import library_asm  # noqa: E402
import match  # noqa: E402
import project  # noqa: E402
from rebuild_exe import CODE_END  # noqa: E402
from research import verified_elf  # noqa: E402
from runtime import ROOT  # noqa: E402


def boundaries(functions):
    """Every address in the code span where something starts: functions, their tables, the library block."""
    starts = {CODE_END, library_asm.START}
    for function in functions:
        starts.add(int(function["address"], 0))
        starts.update(int(address, 0) for address in function.get("placements", {}).values())
        starts.update(int(entry["address"], 0) for entry in function.get("read_only_data", []))
    return sorted(starts)


def linked_text(path, address):
    """The whole .text of a linked comparison ELF and the functions it defines."""
    linked = path.read_bytes()
    metadata = inventory(linked)
    text = [s for s in metadata["sections"] if s["name"] == ".text" and s["size"]]
    if len(text) != 1 or text[0]["address"] != address:
        raise ValueError(f"{path}: expected one .text at 0x{address:x}")
    data = linked[file_offset(metadata, address, text[0]["size"]):][:text[0]["size"]]
    defined = {s["name"]: s["value"] for s in symbol_table(linked, metadata)
               if s["type"] == 2 and address <= s["value"] < address + len(data)}
    return data, defined


def redirect(image, metadata, rule, registry, defined):
    """Point every jal to rule["from"] inside the listed callers at rule["to"]; returns the count per caller."""
    old = 0x0C000000 | (int(registry[rule["from"]]["address"], 0) >> 2)
    if rule["to"] not in defined:
        raise ValueError(f"redirect target {rule['to']} is not defined by the mod")
    new = 0x0C000000 | (defined[rule["to"]] >> 2)
    counts = {}
    for caller in rule["in"]:
        address, size = int(registry[caller]["address"], 0), int(registry[caller]["size"], 0)
        offset = file_offset(metadata, address, size)
        count = 0
        for i in range(offset, offset + size, 4):
            if struct.unpack_from("<I", image, i)[0] == old:
                struct.pack_into("<I", image, i, new)
                count += 1
        if not count:
            raise ValueError(f"{caller} does not call {rule['from']}")
        counts[caller] = count
    return counts


DLC_BASE = 0x2000000
DLC_SCRIPT = """\
SECTIONS {
 /DISCARD/ : { *(.mwcats*) *(.comment) *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) *(.rel.dyn) }
 .dlc 0x%(base)x : {
  KEEP(*header.o(.data .data.* .rodata .rodata.*))
%(live)s
  *(.text .text.*)
  *(.rodata .rodata.* .rdata .lit4 .lit8)
  *(.data .data.* .sdata .sdata.* .sdata2 .sdata2.*)
  . = ALIGN(16);
  _dlc_file_end = .;
 }
 .dlcbss (NOLOAD) : {
  *(.bss .bss.* .sbss .sbss.* COMMON)
  . = ALIGN(16);
  _dlc_mem_end = .;
 }
}
"""
# mod.json "dlc": {"live": "<bytes>"}: the tables written from cards.json, effects.json,
# bosses.json and rarity.json come first, in a region of that fixed size, the fixed-size tables (.rodata) before
# their text (.data). A change to those files then changes only bytes inside the region, which
# tools/launcher.py writes into the running game; everything after it stays where it was.
DLC_LIVE = """\
  . = ALIGN(16);
  _dlc_live = .;
  *cards_gen.o(.rodata .rodata.* .rdata)
  . = ALIGN(16);
  *effects_gen.o(.rodata .rodata.* .rdata)
  . = ALIGN(16);
  *bosses_gen.o(.rodata .rodata.* .rdata)
  . = ALIGN(16);
  *rarity_gen.o(.rodata .rodata.* .rdata)
  . = ALIGN(16);
  *achievements_gen.o(.rodata .rodata.* .rdata)
  . = ALIGN(16);
  *framemap_gen.o(.rodata .rodata.* .rdata)
  . = ALIGN(16);
  *cards_gen.o(.data .data.* .sdata .sdata.*)
  *achievements_gen.o(.data .data.* .sdata .sdata.*)
  *effects_gen.o(.data .data.* .sdata .sdata.*)
  *bosses_gen.o(.data .data.* .sdata .sdata.*)
  . = ALIGN(16);
  ASSERT(. <= _dlc_live + %(size)#x, "DLC: the card, effect and boss tables outgrew mod.json dlc.live");
  . = _dlc_live + %(size)#x;
  _dlc_live_end = .;"""


def build_dlc(mod, settings, config, lock, output, registry, image, image_metadata):
    """Compiles mods/<name>/dlc/*.c and links them at 0x2000000; returns the bytes of DLC.BIN.

    header.c (the header the loader checks) comes first. The game's functions, and the data
    symbols named in the decompiled functions' configurations, are known to the link by name (a
    name two configurations give different addresses is left out); mod.json "dlc": {"globals"}
    adds or overrides symbols.

    "dlc": {"port": {"<function>": {"globals": {...}, "hook": true}}} compiles the decompiled
    source of a game function into the DLC unchanged, renamed DlcPort_<function>, with its symbols
    as in its configuration except the ones given (a table moved into the DLC, say), and by
    default hooks the game's function to it. "dlc": {"hooks": {"<function>": "<DLC function>"}}
    hooks a game function to a function of the DLC's own sources. tools/mod.py writes the hook
    table (dlc_hooks) the loader installs.

    mods/<name>/cards.json, when present, is turned into C by tools/dlc_cards.py (new cards)."""
    dlc_settings = settings.get("dlc", {})
    sources = sorted((mod / "dlc").glob("*.c"), key=lambda p: (p.name != "header.c", p.name))
    if not sources or sources[0].name != "header.c":
        raise ValueError("a DLC needs dlc/header.c")
    # A developer's own DLC folder (tools/dlc_cards.py MY_DLC): its dlc/*.c are linked in too.
    mine = sorted((mod / "my-dlc" / "dlc").glob("*.c"))
    work = output / "dlc"
    work.mkdir(parents=True, exist_ok=True)
    for stale in work.glob("*.o"):
        stale.unlink()
    generated = work / "generated"
    generated.mkdir(exist_ok=True)
    compiler, _ = match.checked_tool(lock, config["compiler"])
    launcher = [] if match.WINDOWS else [str(match.checked_tool(lock, "wibo")[0])]
    linker, _ = match.checked_tool(lock, "mips-binutils")
    env = dict(os.environ, MWCIncludes=str(ROOT / "include"))
    env["LD_LIBRARY_PATH"] = str(match.TOOLS / "mips-binutils/usr/lib/x86_64-linux-gnu")
    includes = ["-Iinclude", "-I" + str((mod / "dlc").relative_to(ROOT))]
    if mine:
        includes.append("-I" + str((mod / "my-dlc" / "dlc").relative_to(ROOT)))

    # Symbols: game functions, then the data symbols of every function configuration.
    symbols = {f["name"]: f["address"] for f in registry.values()
               if re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", f["name"])}
    data_symbols, ambiguous = {}, set()
    for f in registry.values():
        for n, a in f.get("globals", {}).items():
            if n in symbols:
                continue
            if n in data_symbols and int(data_symbols[n], 0) != int(a, 0):
                ambiguous.add(n)
            data_symbols[n] = a
    symbols.update({n: a for n, a in data_symbols.items() if n not in ambiguous})

    hooks = dict(dlc_settings.get("hooks", {}))
    jobs = [(source, source.stem, []) for source in sources]
    jobs += [(source, "my_" + source.stem, []) for source in mine]
    # my-dlc/hooks.json: {"<game function>": "<function of my-dlc/dlc>"}, as mod.json "dlc" "hooks".
    my_hooks = mod / "my-dlc" / "hooks.json"
    if my_hooks.is_file():
        for name, target in json.loads(my_hooks.read_text(encoding="utf-8")).items():
            if name in hooks:
                raise ValueError(f"{my_hooks}: {name} is already hooked by the project ({hooks[name]})")
            hooks[name] = target
    for name, port in dlc_settings.get("port", {}).items():
        function = registry.get(name)
        if function is None or function.get("match_kind", "c") != "c" or function.get("placements") \
                or function.get("unit_sources"):
            raise ValueError(f"cannot port {name}: not a single decompiled C function")
        symbols.update(port.get("globals", {}))
        for n, a in function["globals"].items():
            if n not in port.get("globals", {}) and n in symbols and int(symbols[n], 0) != int(a, 0) \
                    and n not in ambiguous:
                raise ValueError(f"port {name}: {n} has two addresses")
            symbols.setdefault(n, a)
        source = ROOT / function["source"]
        if port.get("replace"):
            # Text changes to the decompiled source, each [old, new] replacing every occurrence of
            # old as whole words; dlc_port.h (in the mod's dlc/) comes first for the helpers.
            text = source.read_text()
            for old, new in port["replace"]:
                pattern = (r"\b" if re.match(r"\w", old) else "") + re.escape(old) + (r"\b" if re.search(r"\w$", old) else "")
                text, count = re.subn(pattern, lambda _m, new=new: new, text)
                if count == 0:
                    raise ValueError(f"port {name}: {old!r} is not in {function['source']}")
            source = generated / f"port_{name}.c"
            source.write_text(f'#include "dlc_port.h"\n#line 1 "{function["source"]}"\n' + text, newline="\n")
        jobs.append((source, "port_" + name, [f"-D{name}=DlcPort_{name}"]))
        if port.get("hook", True):
            hooks[name] = "DlcPort_" + name
    # Wraps: the DLC's function runs in place of a game function and calls the original through
    # DlcTramp_<function>: the original's first two instructions (overwritten by the hook) followed
    # by a jump to its third. Only instructions that work anywhere may be moved: no branch, jump or
    # PC-relative use.
    trampolines = []
    for name, target in dlc_settings.get("wrap", {}).items():
        if name not in registry:
            raise ValueError(f"cannot wrap {name}: not a function of the game")
        address = int(registry[name]["address"], 0)
        at = file_offset(image_metadata, address, 8)
        first = struct.unpack_from("<2I", image, at)
        for word in first:
            op, funct, rs = word >> 26, word & 0x3f, (word >> 21) & 0x1f
            if op in (1, 2, 3, 4, 5, 6, 7, 20, 21, 22, 23) or (op == 0 and funct in (8, 9)) \
                    or (op in (16, 17, 18) and rs == 8):
                raise ValueError(f"cannot wrap {name}: its first instructions include a branch or jump")
        trampolines.append(f"unsigned int DlcTramp_{name}[4] = {{{first[0]:#010x}, {first[1]:#010x}, "
                           f"{0x08000000 | ((address + 8) >> 2) & 0x3ffffff:#010x}, 0}};\n")
        hooks[name] = target
    for name in hooks:
        if name not in registry:
            raise ValueError(f"cannot hook {name}: not a function of the game")
    hook_source = generated / "hooks.c"
    hook_source.write_text(
        "/* Written by tools/mod.py from mod.json. */\n"
        "struct DlcHook { unsigned int address; void *target; };\n"
        + "".join(trampolines)
        + "".join(f"extern char {target}[];\n" for target in sorted(set(hooks.values())))
        + "struct DlcHook dlc_hooks[] = {\n"
        + "".join(f"    {{{int(registry[n]['address'], 0):#x}, {t}}},  /* {n} */\n" for n, t in sorted(hooks.items()))
        + "    {0, 0},\n};\n", newline="\n")
    jobs.append((hook_source, "hooks", []))
    if (mod / "cards.json").is_file():
        import dlc_cards
        cards_source = generated / "cards_gen.c"
        dlc_cards.generate(mod / "cards.json", cards_source)
        jobs.append((cards_source, "cards_gen", []))
    if (mod / "dlc" / "bosses.c").is_file():
        import dlc_bosses
        bosses_source = generated / "bosses_gen.c"
        if (mod / "bosses.json").is_file():
            dlc_bosses.generate(mod / "bosses.json", bosses_source)
        else:
            bosses_source.write_text('#include "dlc_bosses.h"\nconst struct DlcBoss dlc_bosses[] = {{0}};\n'
                                     'const int dlc_boss_count = 0;\n'
                                     'const short dlc_boss_decks[32][41] = {{0}};\nconst int dlc_boss_deck_count = 0;\n'
                                     'const signed char dlc_boss_deck_ai[32] = {0};\nconst int dlc_random_deck_ai = 25;\n',
                                     newline="\n")
        jobs.append((bosses_source, "bosses_gen", []))
    if (mod / "dlc" / "effects.c").is_file():
        # Card effects and leader abilities of one's own (effects.json, tools/dlc_effects.py).
        import dlc_effects
        effects_source = generated / "effects_gen.c"
        original, _ = verified_elf()
        dlc_effects.generate(mod / "effects.json", mod / "cards.json", effects_source, original)
        jobs.append((effects_source, "effects_gen", []))
    if (mod / "dlc" / "buttons.c").is_file():
        # The Xbox button pictures (MOD OPTIONS' BUTTONS row), from the disc's window texture and
        # the icon sheet in art/ (tools/dlc_buttons.py).
        import dlc_buttons
        buttons_source = generated / "buttons_gen.c"
        dlc_buttons.generate(mod / "art" / "xbox_buttons.png", buttons_source)
        jobs.append((buttons_source, "buttons_gen", []))
    if (mod / "dlc" / "sleeves.c").is_file():
        # The card sleeves (MOD OPTIONS' SLEEVE row): the shield texture from the disc and the
        # pictures in sleeves/ (tools/dlc_sleeves.py).
        import dlc_sleeves
        sleeves_source = generated / "sleeves_gen.c"
        dlc_sleeves.generate(mod, sleeves_source)
        jobs.append((sleeves_source, "sleeves_gen", []))
    if (mod / "dlc" / "shop.c").is_file():
        # The card shop: every card's rarity (rarity.json, in the live region) and the shop's
        # pictures from shop/ and sleeves/ (tools/dlc_shop.py).
        import dlc_shop
        rarity_source = generated / "rarity_gen.c"
        import dlc_cards
        dlc_shop.generate_rarity(mod, rarity_source, dlc_cards.game_exe(mod=mod))
        jobs.append((rarity_source, "rarity_gen", []))
        if (mod / "dlc" / "achievements.c").is_file():
            # The mod's achievements (achievements.json, tools/dlc_achievements.py; live region).
            import dlc_achievements
            achievements_source = generated / "achievements_gen.c"
            dlc_achievements.generate(mod, achievements_source)
            jobs.append((achievements_source, "achievements_gen", []))
        shop_source = generated / "shop_gen.c"
        dlc_shop.generate_pictures(mod, shop_source)
        jobs.append((shop_source, "shop_gen", []))
    if (mod / "dlc" / "frames.c").is_file():
        # Monster card frames in more colours (cards.json "frame", tools/dlc_frames.py): the
        # palettes, and each card's frame (live region).
        import dlc_frames
        frames_source = generated / "frames_gen.c"
        framemap_source = generated / "framemap_gen.c"
        dlc_frames.generate(mod, frames_source, framemap_source)
        jobs.append((frames_source, "frames_gen", []))
        jobs.append((framemap_source, "framemap_gen", []))
    if (mod / "dlc" / "modoptions.c").is_file():
        # Community patches for the title screen's MOD OPTIONS (community.json): their executable
        # bytes as runs the DLC writes into memory (tools/dlc_community.py).
        import dlc_community
        community_source = generated / "community_gen.c"
        if (mod / "community.json").is_file():
            original, baseline = verified_elf()
            dlc_community.generate(mod, community_source, bytes(image), original, image_metadata,
                                   baseline["iso_extent"]["offset"])
        else:
            community_source.write_text('#include "dlc_mods.h"\nconst unsigned char dlc_mod_bytes[1] = {0};\n'
                                        'const struct DlcModRun dlc_mod_runs[1] = {{0, 0, 0}};\n'
                                        'const struct DlcModPatch dlc_mod_patches[1] = {{"", 0, 0, 0}};\n'
                                        'const int dlc_mod_patch_count = 0;\n', newline="\n")
        jobs.append((community_source, "community_gen", []))

    objects = []
    for source, stem, extra in jobs:
        obj = work / (stem + ".o")
        with (work / (stem + ".log")).open("w") as log:
            result = subprocess.run([*launcher, str(compiler), "-c", *config["flags"], *extra, *includes,
                                     "-o", str(obj), str(source)], cwd=ROOT, env=env, stdout=log,
                                    stderr=subprocess.STDOUT, timeout=60)
        if result.returncode != 0:
            raise ValueError(f"DLC: {source.name} did not compile:\n" + (work / (stem + ".log")).read_text()[-3000:])
        objects.append(str(obj))
    symbols.update(dlc_settings.get("globals", {}))
    script = work / "dlc.ld"
    live = int(str(dlc_settings["live"]), 0) if "live" in dlc_settings else 0
    script.write_text("".join(f"{n} = {int(a, 0):#x};\n" for n, a in sorted(symbols.items()))
                      + DLC_SCRIPT % dict(base=DLC_BASE, live=DLC_LIVE % dict(size=live) if live else ""),
                      newline="\n")
    elf = work / "dlc.elf"
    with (work / "linker.log").open("w") as log:
        result = subprocess.run([str(linker), "-T", str(script), "-e", "Dlc_Init", "--orphan-handling=error",
                                 "-Map", str(work / "dlc.map"), "-o", str(elf), *objects], cwd=ROOT, env=env,
                                stdout=log, stderr=subprocess.STDOUT, timeout=60)
    if result.returncode != 0:
        raise ValueError("DLC link failed:\n" + (work / "linker.log").read_text()[-3000:])
    linked = elf.read_bytes()
    metadata = inventory(linked)
    text = [s for s in metadata["sections"] if s["name"] == ".dlc"]
    if len(text) != 1 or text[0]["address"] != DLC_BASE or not text[0]["size"]:
        raise ValueError("DLC link: expected one .dlc section at 0x2000000")
    data = linked[file_offset(metadata, DLC_BASE, text[0]["size"]):][:text[0]["size"]]
    if struct.unpack_from("<I", data, 0)[0] != 0x31434C44:
        raise ValueError("DLC link: the header is not at the start")
    (output / "DLC.BIN").write_bytes(data)
    return data


def dev_files(mod, dlc, image, info, dev, iso, output, result):
    """--dev DIR: the files tools/launcher.py puts on its working copy of the disc, in place of a
    new disc image. Nothing here writes to a disc (PCSX2 keeps its disc locked while it runs).

    DIR/SLUS_205.15 (the executable, DLC_INFO left for the launcher, which knows where DLC.BIN
    lands) and DIR/DLC.BIN (the code only). DIR/disc/ holds the other files the disc needs, made
    from the clean disc and written there only when they changed (DIR/disc/staged.json): the new
    cards' pictures, the second world's map and the new cards' models (as DLCMDL.BIN, which the
    DLC finds through dlc_header.models_sector). build.json says which of them changed and where
    the live region (mod.json dlc.live) is."""
    dev = Path(dev)
    dev = (dev if dev.is_absolute() else ROOT / dev).resolve()
    dev.mkdir(parents=True, exist_ok=True)
    clean = iso_files.Image(project.find_iso(iso), read_only=True)
    assets = {}
    models = b""
    try:
        def whole(path):
            _, _, entry = clean.lookup(path)
            start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
            return clean.read(start, (size + 2047) // 2048)[:size]

        if (mod / "cards.json").is_file():
            import dlc_cards
            _, _, entry = clean.lookup("/DATA/MONSTER.MRG")
            monsters = struct.unpack_from("<I", entry, 2)[0]
            cache = dev / "cache"
            slots, reports = dlc_cards.models(mod / "cards.json", lambda card: clean.read(monsters + card * 512, 512),
                                              cache=cache)
            if slots:
                models = b"".join(slots)
                result["models"] = reports
            # The pictures depend only on each card's donor and art (and the wallpaper).
            listed = dlc_cards.read_entries(mod / "cards.json")
            key = dlc_cards.digest([[c.get("number"), c.get("donor"), c.get("art"), c.get("placeholders")] for c in listed],
                                   *(mod / c["art"] for c in listed if c.get("art")),
                                   dlc_cards.wallpaper_path(mod / "cards.json") or "-")
            assets["/DATA/PICTURE.MRG"] = dlc_cards.cached(
                cache, "pictures", key, lambda: dlc_cards.pictures(mod / "cards.json", whole("/DATA/PICTURE.MRG")))
            assets["/DATA/PICMINI.MRG"] = dlc_cards.cached(
                cache, "minis", key, lambda: dlc_cards.minis(mod / "cards.json", whole("/DATA/PICMINI.MRG")))
            # The game's cards given art of their own also have it in their pack slots.
            assets["/DATA/PICPACK.MRG"] = dlc_cards.cached(
                cache, "picpack", key, lambda: dlc_cards.picpack(mod / "cards.json", whole("/DATA/PICPACK.MRG"),
                                                                 whole("/DATA/PICTURE.MRG")))
        if (mod / "title" / "expanded.png").is_file():
            # The title picture with the mod's logo over the trademark sign (tools/dlc_title.py).
            import dlc_cards
            import dlc_title
            logo = mod / "title" / "expanded.png"
            assets["/DATA/TEXETC.MRG"] = dlc_cards.cached(
                dev / "cache", "title", dlc_cards.digest(logo, tools=("dlc_title.py",)),
                lambda: dlc_title.compose(whole("/DATA/TEXETC.MRG"), logo))
        if (mod / "dlc" / "world.c").is_file():
            import dlc_world
            assets["/DATA/MODEL.MRG"], packs = dlc_world.extend(whole("/DATA/MODEL.MRG"), bytes(image))
            result["world"] = dict(packs=packs)
        if models:
            assets["/DLCMDL.BIN"] = models
    finally:
        clean.close()
    staging = dev / "disc"
    staging.mkdir(exist_ok=True)
    state_path = staging / "staged.json"
    state = json.loads(state_path.read_text()) if state_path.is_file() else {}
    changed = []
    for path, data in assets.items():
        digest = hashlib.sha256(data).hexdigest()
        name = path.strip("/").replace("/", "_")
        if state.get(path, {}).get("sha256") != digest or not (staging / name).is_file():
            (staging / name).write_bytes(data)
            state[path] = dict(sha256=digest, file=name, bytes=len(data))
            changed.append(path)
    for path in [p for p in state if p not in assets]:
        (staging / state.pop(path)["file"]).unlink(missing_ok=True)
        changed.append(path)
    state_path.write_text(json.dumps(state, indent=2) + "\n")
    code = bytearray(dlc)
    if models:
        # Any value but 0 says there are models; dlc_header.models_sector (written by the
        # launcher) says where.
        struct.pack_into("<I", code, 0x44, 0x800)
    image[info:info + 12] = bytes(12)
    (dev / "DLC.BIN").write_bytes(bytes(code))
    (dev / "SLUS_205.15").write_bytes(bytes(image))
    live, symbols = {}, {}
    for line in (output / "dlc" / "dlc.map").read_text().splitlines():
        m = re.match(r"\s+0x([0-9a-f]+)\s+(_dlc_live|_dlc_live_end) = ", line)
        if m:
            live[m.group(2)] = int(m.group(1), 16)
        # Tables the player's setup tool rewrites in DLC.BIN (tools/release.py): their offsets.
        m = re.match(r"\s+0x([0-9a-f]+)\s+(dlc_card_status|dlc_give_all)\s*$", line)
        if m:
            symbols[m.group(2)] = int(m.group(1), 16) - DLC_BASE
    result["dev"] = dict(folder=str(dev), disc_changed=changed, dlc_bytes=len(code),
                         # Where DLC_INFO is in SLUS_205.15, and where SLUS_205.15 is in the disc image.
                         info_offset=info, iso_extent=verified_elf()[1]["iso_extent"]["offset"],
                         live=[live.get("_dlc_live", 0) - DLC_BASE, live.get("_dlc_live_end", 0) - DLC_BASE],
                         symbols=symbols)
    (dev / "build.json").write_text(json.dumps(result["dev"], indent=2) + "\n")


def build(name, iso=None, want_iso=False, defines=(), variant=None, dev=None):
    mod = ROOT / "mods" / name
    if not (mod / "src").is_dir() and not (mod / "asm").is_dir():
        raise ValueError(f"no mod sources under {mod}")
    settings = json.loads((mod / "mod.json").read_text()) if (mod / "mod.json").is_file() else {}
    config = json.loads((ROOT / "config/ntsc-u/function-builds.json").read_text())
    config = {**config, "flags": [*config["flags"], *("-D" + d for d in defines)]}
    lock = json.loads((ROOT / "config/tooling-lock.json").read_text())
    original, baseline = verified_elf()
    metadata = inventory(original)
    registry = {f["name"]: f for f in config["functions"]}
    selected = []
    for function in config["functions"]:
        assembly = str(Path("asm", *Path(function["source"]).parts[1:]).with_suffix(".s").as_posix())
        if (mod / function["source"]).is_file():
            selected.append(function)
        elif function.get("match_kind", "c") == "c" and (mod / assembly).is_file():
            # A C function may be replaced by whole-function assembly at the same path under asm/.
            selected.append({**function, "source": assembly, "match_kind": "asm", "original_source": function["source"]})
    known = {(mod / f["source"]).resolve() for f in selected}
    strays = [str(p.relative_to(mod)) for d in ("src", "asm") for p in (mod / d).rglob("*")
              if p.is_file() and p.suffix in (".c", ".s") and p.resolve() not in known]
    if strays:
        raise ValueError("mod sources that replace no function: " + ", ".join(strays))
    if not selected:
        raise ValueError("the mod replaces no function")
    starts = boundaries(config["functions"])
    output = ROOT / "build/ntsc-u/mods" / (name + ("-" + variant if variant else ""))
    image = bytearray(original)
    changes, defined = [], {}
    for function in selected:
        if function.get("placements") or function.get("unit_sources"):
            raise ValueError(f"{function['name']}: functions built as a comparison unit cannot be replaced yet")
        extra = settings.get("globals", {}).get(function["name"], {})
        function = {**function, "globals": {**function["globals"], **extra}}
        if function["name"] in settings.get("drop_tables", []):
            # The replacement uses none of the original's read-only tables (a switch's jump
            # table, say); they stay in the image where they were, unread.
            function = {**function, "read_only_data": []}
        report = match.build_function(function, config, original, metadata, baseline, lock,
                                      overlay=mod, output_root=output / "functions")
        if any(not r["matching"] for r in report["read_only_data"]):
            raise ValueError(f"{function['name']}: its read-only table changed; tables cannot move yet")
        address, size = int(function["address"], 0), int(function["size"], 0)
        rebuilt, symbols = linked_text(output / "functions" / function["name"] / "function.elf", address)
        defined.update(symbols)
        room = next(s for s in starts if s > address) - address
        offset = file_offset(metadata, address, room)
        if any(original[offset + size:offset + room]):
            raise ValueError(f"{function['name']}: the bytes after it are not padding")
        if len(rebuilt) > room:
            raise ValueError(f"{function['name']}: {len(rebuilt)} bytes do not fit in {room} "
                             f"(original {size} + {room - size} padding)")
        image[offset:offset + room] = rebuilt + bytes(room - len(rebuilt))
        changes.append(dict(name=function["name"], address=f"0x{address:x}", original_size=size,
                            new_size=len(rebuilt), room=room, source=function["source"],
                            defines={n: f"0x{a:x}" for n, a in sorted(symbols.items(), key=lambda kv: kv[1])},
                            changed_bytes=sum(a != b for a, b in zip(image[offset:offset + room],
                                                                     original[offset:offset + room]))))
    # "patch": [{"address": "0x...", "old": "0x<word>", "new": "0x<word>"}]: single instruction
    # words changed in place (for functions that cannot be rebuilt or ported); the old word must be
    # what the executable has there.
    for patch in settings.get("patch", []):
        address = int(patch["address"], 0)
        at = file_offset(metadata, address, 4)
        have = struct.unpack_from("<I", image, at)[0]
        if have != int(patch["old"], 0):
            raise ValueError(f"patch at {patch['address']}: found {have:#010x}, expected {patch['old']}")
        struct.pack_into("<I", image, at, int(patch["new"], 0))
    redirects = [dict(rule, calls=redirect(image, metadata, rule, registry, defined))
                 for rule in settings.get("redirect", [])]
    output.mkdir(parents=True, exist_ok=True)
    dlc = None
    if (mod / "dlc").is_dir():
        if "info" not in settings.get("dlc", {}):
            raise ValueError('a mod with dlc/ needs "dlc": {"info": "<address>"} in mod.json')
        dlc = build_dlc(mod, settings, config, lock, output, registry, image, metadata)
        info = file_offset(metadata, int(settings["dlc"]["info"], 0), 12)
        # Sector and size of DLC.BIN, and the loaded flag; zero (nothing to load) unless an image
        # with DLC.BIN on it is made below.
        image[info:info + 12] = bytes(12)
    result = dict(mod=name, variant=variant, defines=list(defines), functions=changes,
                  redirects=[dict(source=r["from"], target=r["to"], callers=len(r["calls"]),
                                  calls=sum(r["calls"].values())) for r in redirects],
                  changed_bytes=sum(a != b for a, b in zip(image, original)),
                  sha256=hashlib.sha256(image).hexdigest(), original_sha256=baseline["sha256"])
    if dev:
        if dlc is None:
            raise ValueError("--dev needs a mod with a DLC")
        dev_files(mod, dlc, image, info, dev, iso, output, result)
    elif want_iso:
        source = project.find_iso(iso)
        project.verify(source, project.load_config())
        target = output / f"dotr-{output.name}.iso"
        shutil.copyfile(source, target)
        if dlc is not None:
            disc = iso_files.Image(target)
            try:
                contents = dlc
                if (mod / "cards.json").is_file():
                    import dlc_cards
                    _, _, entry = disc.lookup("/DATA/MONSTER.MRG")
                    monsters = struct.unpack_from("<I", entry, 2)[0]

                    def read_slot(card):
                        return disc.read(monsters + card * 512, 512)

                    slots, reports = dlc_cards.models(mod / "cards.json", read_slot)
                    if slots:
                        # The new cards' model slots follow the code in DLC.BIN, from a whole
                        # sector; the header's "models" word (+0x44) says where. The loader still
                        # reads only the code (DLC_INFO keeps its size).
                        start = (len(dlc) + 2047) & ~2047
                        code = bytearray(dlc)
                        struct.pack_into("<I", code, 0x44, start)
                        contents = bytes(code) + bytes(start - len(dlc)) + b"".join(slots)
                        result["models"] = reports
                sector = disc.put("/DLC.BIN", contents)
                if (mod / "cards.json").is_file():
                    import dlc_cards
                    _, _, entry = disc.lookup("/DATA/PICTURE.MRG")
                    start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
                    clean_pictures = disc.read(start, (size + 2047) // 2048)[:size]
                    _, _, entry = disc.lookup("/DATA/PICPACK.MRG")
                    start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
                    pack = dlc_cards.picpack(mod / "cards.json", disc.read(start, (size + 2047) // 2048)[:size],
                                             clean_pictures)
                    result["picpack"] = dict(sector=disc.put("/DATA/PICPACK.MRG", pack), bytes=len(pack))
                    pictures = dlc_cards.pictures(mod / "cards.json", clean_pictures)
                    result["pictures"] = dict(sector=disc.put("/DATA/PICTURE.MRG", pictures), bytes=len(pictures))
                    _, _, entry = disc.lookup("/DATA/PICMINI.MRG")
                    start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
                    mini = dlc_cards.minis(mod / "cards.json", disc.read(start, (size + 2047) // 2048)[:size])
                    result["minis"] = dict(sector=disc.put("/DATA/PICMINI.MRG", mini), bytes=len(mini))
                if (mod / "title" / "expanded.png").is_file():
                    import dlc_title
                    _, _, entry = disc.lookup("/DATA/TEXETC.MRG")
                    start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
                    title = dlc_title.compose(disc.read(start, (size + 2047) // 2048)[:size], mod / "title" / "expanded.png")
                    result["title"] = dict(sector=disc.put("/DATA/TEXETC.MRG", title), bytes=len(title))
                if (mod / "dlc" / "world.c").is_file():
                    # The second world's map: recoloured copies of the campaign map packs.
                    import dlc_world
                    _, _, entry = disc.lookup("/DATA/MODEL.MRG")
                    start, size = struct.unpack_from("<I", entry, 2)[0], struct.unpack_from("<I", entry, 10)[0]
                    models, world = dlc_world.extend(disc.read(start, (size + 2047) // 2048)[:size], bytes(image))
                    result["world"] = dict(sector=disc.put("/DATA/MODEL.MRG", models), packs=world)
            finally:
                disc.close()
            image[info:info + 12] = struct.pack("<III", sector, len(dlc), 0)
            result["dlc"] = dict(sector=sector, bytes=len(dlc))
        with target.open("r+b") as stream:
            stream.seek(baseline["iso_extent"]["offset"])
            stream.write(image)
        result["iso"] = str(target.relative_to(ROOT))
    elif dlc is not None:
        result["dlc"] = dict(bytes=len(dlc))
    result["sha256"] = hashlib.sha256(image).hexdigest()
    if not dev:
        (output / "SLUS_205.15").write_bytes(image)
    (output / "report.json").write_text(json.dumps(result, indent=2) + "\n")
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("name", help="the mod's directory name under mods/")
    parser.add_argument("--make-iso", action="store_true", help="also write a patched copy of the disc image")
    parser.add_argument("--iso", help="disc image to copy; otherwise DOTR_ISO or baserom/*.iso")
    parser.add_argument("--define", action="append", default=[], metavar="NAME=VALUE",
                        help="preprocessor definition for the mod's C sources; repeat for several")
    parser.add_argument("--variant", help="suffix for the output directory, to keep builds with different definitions apart")
    parser.add_argument("--dev", metavar="DIR", help="write the launcher's files to DIR (tools/launcher.py) instead of a disc image")
    args = parser.parse_args(argv)
    try:
        print(json.dumps(build(args.name, args.iso, args.make_iso, args.define, args.variant, args.dev), indent=2))
        return 0
    except (OSError, ValueError, KeyError, StopIteration, subprocess.SubprocessError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
