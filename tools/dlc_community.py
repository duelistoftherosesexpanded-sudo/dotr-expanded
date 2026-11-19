"""Community patches for the title screen's MOD OPTIONS (mods/<mod>/community.json, dlc/modoptions.c).

Each patch is a .ppf made against the original NTSC-U disc. Only its bytes inside the executable
are used: they become runs of (memory address, original bytes, patched bytes) that the DLC writes
into memory while the patch is chosen and writes back when it is not. A main patch is taken
against the game; an extra against the game ("for": "original") or against a main patch (its id).

The executable the runs are taken against is the mod's own (functions rebuilt, words patched): a
patch byte the mod has changed is an error, since one of the two would be lost."""

import json
import struct
from pathlib import Path


def ppf_bytes(path):
    """{disc offset: byte} of a PPF 3.0 (or 2.0) patch."""
    d = Path(path).read_bytes()
    out = {}
    if d[:5] == b"PPF30":
        blockcheck, undo = d[57], d[58]
        pos = 60 + (1024 if blockcheck else 0)
        end = d.rfind(b"@BEGIN_FILE_ID.DIZ")
        end = len(d) if end < 0 else end
        while pos + 9 <= end:
            off = struct.unpack_from("<Q", d, pos)[0]
            n = d[pos + 8]
            for i in range(n):
                out[off + i] = d[pos + 9 + i]
            pos += 9 + n + (n if undo else 0)
    elif d[:5] == b"PPF20":
        pos = 56 + 4 + 1024
        while pos + 5 <= len(d):
            off = struct.unpack_from("<I", d, pos)[0]
            n = d[pos + 4]
            for i in range(n):
                out[off + i] = d[pos + 5 + i]
            pos += 5 + n
    else:
        raise ValueError(f"{path}: not a PPF 2.0/3.0 patch")
    return out


def _address(metadata, offset):
    for s in metadata["program_headers"]:
        if s["type"] == 1 and s["offset"] <= offset < s["offset"] + s["filesz"]:
            return s["vaddr"] + offset - s["offset"]
    return None


def _runs(before, after, metadata):
    """Runs of changed bytes between two executables: [(address, old, new)], gaps under 8 merged."""
    changed = [i for i in range(len(before)) if before[i] != after[i]]
    runs = []
    for i in changed:
        if runs and i - runs[-1][1] < 8 and _address(metadata, i) is not None \
                and _address(metadata, i) - _address(metadata, runs[-1][0]) == i - runs[-1][0]:
            runs[-1][1] = i + 1
        else:
            runs.append([i, i + 1])
    out = []
    for a, b in runs:
        address = _address(metadata, a)
        if address is None:
            raise ValueError(f"community patch: changes at executable offset {a:#x}, outside the loaded program")
        out.append((address, bytes(before[a:b]), bytes(after[a:b])))
    return out


def generate(mod, out_c, image, original, metadata, iso_offset):
    settings = json.loads((mod / "community.json").read_text())
    mains = settings.get("main", [])
    ids = {m["id"]: k + 1 for k, m in enumerate(mains)}
    ours = {i for i in range(len(image)) if image[i] != original[i]}

    def patched(base, path):
        out = bytearray(base)
        for off, v in ppf_bytes(mod / path).items():
            i = off - iso_offset
            if 0 <= i < len(out):
                out[i] = v
        return out

    def check(path):
        hits = sorted(i for i in (off - iso_offset for off in ppf_bytes(mod / path)) if i in ours)
        if hits:
            raise ValueError(f"community patch {path}: changes {len(hits)} bytes the mod also changes "
                             f"(first at executable offset {hits[0]:#x})")

    patches = []   # (name, for, runs)
    main_images = {}
    for m in mains:
        check(m["ppf"])
        after = patched(image, m["ppf"])
        main_images[m["id"]] = after
        patches.append((m["name"], 0, _runs(image, after, metadata), m.get("note", "")))
    for e in settings.get("extras", []):
        check(e["ppf"])
        base = image if e["for"] == "original" else main_images[e["for"]]
        after = patched(base, e["ppf"])
        runs = _runs(base, after, metadata)
        patches.append((e["name"], -1 if e["for"] == "original" else -1 - ids[e["for"]], runs, e.get("note", "")))

    data = bytearray()
    run_rows, patch_rows = [], []
    for name, kind, runs, _note in patches:
        first = len(run_rows)
        for address, old, new in runs:
            run_rows.append((address, len(old), len(data)))
            data += old + new
        patch_rows.append((name, kind, first, len(runs), sum(len(o) for _, o, _ in runs)))

    lines = ["/* Written by tools/dlc_community.py from community.json. */",
             '#include "dlc_mods.h"', "",
             f"const unsigned char dlc_mod_bytes[{max(len(data), 1)}] = {{"]
    for i in range(0, len(data), 24):
        lines.append("    " + ",".join(f"{b}" for b in data[i:i + 24]) + ",")
    if not data:
        lines.append("    0,")
    lines += ["};", "", f"const struct DlcModRun dlc_mod_runs[{max(len(run_rows), 1)}] = {{"]
    lines += [f"    {{{a:#x}, {n}, {at}}}," for a, n, at in run_rows] or ["    {0, 0, 0},"]
    lines += ["};", "", "/* kind: 0 a main patch (main option k + 1 in order); -1 an extra for the game;",
              " * -1 - k an extra for main option k. */",
              f"const struct DlcModPatch dlc_mod_patches[{max(len(patch_rows), 1)}] = {{"]
    for name, kind, first, count, size in patch_rows:
        lines.append(f'    {{"{name}", {kind}, {first}, {count}}},  /* {size} bytes */')
    if not patch_rows:
        lines.append('    {"", 0, 0, 0},')
    lines += ["};", f"const int dlc_mod_patch_count = {len(patch_rows)};", ""]
    Path(out_c).write_text("\n".join(lines), newline="\n")
    return [dict(name=n, kind=k, runs=c, bytes=s) for n, k, _, c, s in patch_rows]
