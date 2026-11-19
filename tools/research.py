#!/usr/bin/env python3
"""Executable and disc inventories and checked byte-range extraction, written to the ignored out/research/."""

import argparse
import json
from pathlib import Path
import sys
import hashlib
import subprocess

from dotr.elf import inventory, file_offset, symbol_table
from dotr.disc import disc_files, overlay_candidates
from dotr.binary import read_at
import project
from runtime import ROOT


def verified_elf():
    data = (ROOT / "out/ntsc-u/SLUS_205.15").read_bytes()
    baseline = json.loads((ROOT / "out/ntsc-u/elf.json").read_text())
    if hashlib.sha256(data).hexdigest() != baseline["sha256"]:
        raise ValueError("prepared executable no longer matches its verification record")
    return data, baseline


def inventory_disc(path, output):
    identity = project.verify(path, project.load_config())
    with path.open("rb") as stream:
        files = disc_files(stream)
        boot = files[project.load_config()["boot_path"]]
        main = inventory(read_at(stream, boot["offset"], boot["size"]))
        modules = []
        archives = []
        for name, extent in sorted(files.items()):
            if name.endswith(".IRX"):
                if extent["size"] > 8 * 1024 * 1024:
                    raise ValueError("implausible IOP module size")
                data = read_at(stream, extent["offset"], extent["size"])
                metadata = inventory(data, allowed_types=(1, 2, 0xff80))
                metadata.update(path=name, iso_extent=extent, address_space="iop-module-relative")
                destination = output / "iop" / name
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(data)
                records = symbol_table(data, metadata)
                metadata["symbol_record_count"] = len(records)
                metadata["debug_metadata"] = [s["name"] for s in metadata["sections"]
                                              if s["name"] in {".mdebug", ".debug_info", ".stab"} and s["size"]]
                for section in metadata["sections"]:
                    if section["name"] == ".iopmod":
                        header = data[section["offset"]:section["offset"] + section["size"]]
                        if len(header) < 27 or b"\0" not in header[26:]:
                            raise ValueError("invalid IOP module identity header")
                        metadata["iopmod_name"] = header[26:header.index(b"\0", 26)].decode("ascii")
                        metadata["iopmod_version_raw"] = int.from_bytes(header[24:26], "little")
                destination.with_suffix(".symbols.json").write_text(json.dumps(records, indent=2) + "\n")
                modules.append(metadata)
            if name in {"DATA/BMODULE.MRG", "DATA/CMODULE.MRG", "DATA/MMODULE.MRG"}:
                if extent["size"] > 512 * 1024 * 1024:
                    raise ValueError("implausible overlay archive size")
                data = read_at(stream, extent["offset"], extent["size"])
                entries = overlay_candidates(data)
                for entry in entries:
                    entry["iso_offset"] = extent["offset"] + entry["offset"]
                    start, size = entry["offset"], entry["candidate_extent_size"]
                    entry["candidate_sha256"] = hashlib.sha256(data[start:start + size]).hexdigest()
                    if size >= 64:
                        raw = data[start:start + 64]
                        entry["header_words_00_1c"] = [int.from_bytes(raw[i:i + 4], "little")
                                                       for i in range(0, 32, 4)]
                        entry["header_name_raw"] = raw[32:64].split(b"\0")[0].decode("ascii")
                        words = entry["header_words_00_1c"]
                        header_id = words[1]
                        headers = main["program_headers"]
                        entry["header_id"] = header_id
                        entry["expected_memory_size_from_header"] = 64 + sum(words[3:6])
                        entry["main_program_header_correlated"] = (0 < header_id < len(headers)
                            and headers[header_id]["vaddr"] == words[2]
                            and headers[header_id]["memsz"] == entry["expected_memory_size_from_header"])
                archives.append(dict(path=name, iso_extent=extent, sha256=hashlib.sha256(data).hexdigest(),
                                     format="MWo3 signature candidates; payload/relocations unverified", entries=entries))
    ids = {entry["header_id"] for archive in archives for entry in archive["entries"] if "header_id" in entry}
    missing = [dict(index=i, **header) for i, header in enumerate(main["program_headers"])
               if i and header["vaddr"] == 0x1e00000 and i not in ids]
    result = dict(disc=identity, files=files, iop_modules=modules, overlay_archives=archives,
                  main_executable_sha256=main["sha256"], overlay_headers_without_archive_candidate=missing)
    (output / "disc-inventory.json").write_text(json.dumps(result, indent=2) + "\n")
    print(f"Inventoried {len(files)} files and {len(modules)} IOP modules")
    for archive in archives:
        print(f"{archive['path']}: {len(archive['entries'])} aligned MWo3 candidates")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("inventory", "disc", "range"))
    parser.add_argument("--iso", help="disc image for the disc inventory; otherwise DOTR_ISO or baserom/*.iso")
    parser.add_argument("--address", type=lambda value: int(value, 0),
                        help="range: start address in the EE address space")
    parser.add_argument("--size", type=lambda value: int(value, 0), help="range: size in bytes")
    args = parser.parse_args()
    try:
        output = ROOT / "out/research/ntsc-u"
        output.mkdir(parents=True, exist_ok=True)
        if args.command == "disc":
            inventory_disc(project.find_iso(args.iso), output)
            return 0
        data, baseline = verified_elf()
        metadata = inventory(data)
        if args.command == "inventory":
            metadata["iso_extent"] = baseline["iso_extent"]
            (output / "inventory.json").write_text(json.dumps(metadata, indent=2) + "\n")
            print(f"Inventoried {len(metadata['sections'])} sections and {len(metadata['program_headers'])} program headers")
            print("Compiler comments:", metadata["comments"])
        else:
            if args.address is None or args.size is None:
                raise ValueError("range requires --address and --size")
            offset = file_offset(metadata, args.address, args.size)
            path = output / f"range_{args.address:08x}_{args.size:x}.bin"
            path.write_bytes(data[offset:offset + args.size])
            print(json.dumps({"path": str(path), "elf_offset": offset,
                              "iso_offset": baseline["iso_extent"]["offset"] + offset,
                              "sha256": hashlib.sha256(path.read_bytes()).hexdigest()}, indent=2))
    except (OSError, ValueError, KeyError, UnicodeError, subprocess.SubprocessError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
