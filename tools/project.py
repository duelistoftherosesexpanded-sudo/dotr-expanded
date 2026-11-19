#!/usr/bin/env python3
"""Workspace commands; Python 3.10+, standard library only."""

import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import re
import sys

from dotr.binary import elf_metadata, read_at, root_files

ROOT = Path(os.environ["DOTR_ROOT"]) if os.environ.get("DOTR_ROOT") else Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config/ntsc-u"
STATUSES = {"identified", "in-progress", "nonmatching", "matching"}
FIELDS = ["module", "address", "size", "name", "kind", "status", "source", "evidence", "match_kind"]


def load_config():
    config = json.loads((CONFIG / "disc.json").read_text())
    if config["schema_version"] != 1 or config["target"] != "ntsc-u":
        raise ValueError("unsupported target configuration")
    if config["sector_size"] != 2048 or config["serial"] != config["boot_path"]:
        raise ValueError("invalid disc configuration")
    if not re.fullmatch(r"[0-9a-f]{32}", config["image_md5"]):
        raise ValueError("invalid image MD5")
    return config


def symbols():
    rows = []
    seen_names, seen_addresses = set(), set()
    with (CONFIG / "symbols.csv").open(newline="") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != FIELDS:
            raise ValueError("invalid symbol registry columns")
        for line, row in enumerate(reader, 2):
            if None in row or any(not row.get(key) for key in FIELDS):
                raise ValueError(f"symbols.csv:{line}: incomplete row")
            address, size = int(row["address"], 0), int(row["size"], 0)
            if not 0 <= address <= 0xFFFFFFFF or not 0 <= size <= 0x100000000 - address:
                raise ValueError(f"symbols.csv:{line}: invalid address range")
            if row["kind"] not in {"function", "data"} or row["status"] not in STATUSES:
                raise ValueError(f"symbols.csv:{line}: invalid kind/status")
            allowed = {"c", "asm"} if row["kind"] == "function" else {"none"}
            if row["match_kind"] not in allowed:
                raise ValueError(f"symbols.csv:{line}: invalid match kind")
            identity = (row["module"], row["name"])
            location = (row["module"], address)
            if identity in seen_names or location in seen_addresses:
                raise ValueError(f"symbols.csv:{line}: duplicate symbol")
            seen_names.add(identity)
            seen_addresses.add(location)
            source = (ROOT / row["source"]).resolve()
            if not source.is_relative_to(ROOT) or not source.is_file():
                raise ValueError(f"symbols.csv:{line}: source must be a workspace file")
            evidence = (ROOT / row["evidence"].split("#", 1)[0]).resolve()
            if not evidence.is_relative_to(ROOT) or not evidence.is_file():
                raise ValueError(f"symbols.csv:{line}: evidence must name a workspace file")
            if row["status"] == "matching" and size == 0:
                raise ValueError(f"symbols.csv:{line}: matching requires a known size")
            if row["kind"] == "function" and row["status"] == "matching":
                suffix = ".s" if row["match_kind"] == "asm" else ".c"
                if source.suffix.lower() != suffix:
                    raise ValueError(f"symbols.csv:{line}: source differs from match kind")
            rows.append(row)
    return rows


def progress(rows):
    functions = [row for row in rows if row["kind"] == "function"]
    matched = [row for row in functions if row["status"] == "matching"]
    result = {"registered_functions": len(functions), "matching_functions": len(matched),
              "registered_bytes": sum(int(row["size"], 0) for row in functions),
              "matching_bytes": sum(int(row["size"], 0) for row in matched)}
    for kind in ("c", "asm"):
        group = [row for row in matched if row["match_kind"] == kind]
        result[f"matching_{kind}_functions"] = len(group)
        result[f"matching_{kind}_bytes"] = sum(int(row["size"], 0) for row in group)
    result["scope"] = "game code of the main executable; overlay modules and IOP modules not included"
    return result


def find_iso(explicit):
    value = explicit or os.environ.get("DOTR_ISO")
    if value:
        path = Path(value).expanduser().resolve()
        if not path.is_file():
            raise ValueError(f"ISO does not exist: {path}")
        return path
    candidates = sorted(p for p in (ROOT / "baserom").iterdir() if p.suffix.lower() == ".iso")
    if len(candidates) != 1:
        raise ValueError("supply --iso PATH or DOTR_ISO, or place exactly one ISO in baserom/")
    return candidates[0]


def verify(path, config):
    if path.stat().st_size != config["image_size"]:
        raise ValueError("disc size differs from the supported clean NTSC-U image")
    md5, sha256 = hashlib.md5(), hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(4 * 1024 * 1024), b""):
            md5.update(chunk)
            sha256.update(chunk)
    if md5.hexdigest() != config["image_md5"]:
        raise ValueError("disc MD5 differs; modified/other-revision images are unsupported")
    return {"target": config["target"], "size": path.stat().st_size,
            "md5": md5.hexdigest(), "sha256": sha256.hexdigest()}


def prepare(path, config):
    identity = verify(path, config)
    with path.open("rb") as stream:
        files = root_files(stream)
        boot = files[config["boot_path"]]
        cnf = files["SYSTEM.CNF"]
        if boot["size"] > 32 * 1024 * 1024 or cnf["size"] > 65536:
            raise ValueError("implausible boot file size")
        system = read_at(stream, cnf["offset"], cnf["size"])
        expected = "cdrom0:\\" + config["boot_path"] + ";1"
        if not re.search(r"^BOOT2\s*=\s*" + re.escape(expected) + r"\s*$", system.decode("ascii"), re.M):
            raise ValueError("SYSTEM.CNF does not boot the expected executable")
        executable = read_at(stream, boot["offset"], boot["size"])
    metadata = elf_metadata(executable)
    if metadata["machine"] != config["elf_machine"] or metadata["entry"] != config["elf_entry"]:
        raise ValueError("unexpected executable identity")
    metadata["iso_extent"] = boot
    metadata["sha256"] = hashlib.sha256(executable).hexdigest()
    output = ROOT / "out/ntsc-u"
    output.mkdir(parents=True, exist_ok=True)
    (output / config["boot_path"]).write_bytes(executable)
    (output / "SYSTEM.CNF").write_bytes(system)
    for name, value in (("disc.json", identity), ("elf.json", metadata)):
        (output / name).write_text(json.dumps(value, indent=2) + "\n")
    print(f"Prepared verified boot files and metadata in {output}")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("check", "verify", "prepare", "progress"))
    parser.add_argument("--iso", help="disc image; otherwise DOTR_ISO or baserom/*.iso")
    args = parser.parse_args(argv)
    try:
        config = load_config()
        if args.command == "check":
            print(f"Configuration and {len(symbols())} symbol rows validated")
        elif args.command == "progress":
            print(json.dumps(progress(symbols()), indent=2))
        elif args.command == "verify":
            print(json.dumps(verify(find_iso(args.iso), config), indent=2))
        else:
            prepare(find_iso(args.iso), config)
    except (OSError, ValueError, KeyError, UnicodeError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
