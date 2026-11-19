#!/usr/bin/env python3
"""Compile and compare individual EE functions; not a complete PS2 build."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys

from dotr.compare import compare_bytes
from dotr.elf import inventory, file_offset, rename_function_sections, symbol_table
from research import verified_elf
from runtime import ROOT, TOOLS, WINDOWS, platform_artifacts


def checked_tool(lock, name, role="executable"):
    artifact = next(a for a in platform_artifacts(lock) if a["tool"] == name)
    executable = ROOT / artifact[role]
    if hashlib.sha256(executable.read_bytes()).hexdigest() != artifact[role + "_sha256"]:
        raise ValueError(f"{role} fingerprint differs: {name}")
    return executable, artifact


def read_only_placements(function):
    """Only explicit whole read-only sections; no offsets, fragments or masks."""
    result = []
    occupied = [(int(function["address"], 0), int(function["size"], 0))]
    names = set()
    entries = function.get("read_only_data", [])
    if not isinstance(entries, list):
        raise ValueError("read-only placements must be a list")
    for entry in entries:
        if not isinstance(entry, dict) or set(entry) != {"section", "address", "size"}:
            raise ValueError("read-only placement requires section, address and size only")
        if not all(isinstance(value, str) for value in entry.values()):
            raise ValueError("read-only placement fields must be strings")
        name = entry["section"]
        if not isinstance(name, str) or not re.fullmatch(r"\.(?:rodata|rdata)(?:\.[A-Za-z_0-9]+)*", name):
            raise ValueError("invalid read-only section name")
        address, size = int(entry["address"], 0), int(entry["size"], 0)
        if name in names or size <= 0 or not 0 <= address < address + size <= 0x100000000:
            raise ValueError("invalid or duplicate read-only placement")
        if any(address < a + n and a < address + size for a, n in occupied):
            raise ValueError("overlapping read-only placement")
        names.add(name)
        occupied.append((address, size))
        result.append(dict(section=name, address=address, size=size))
    return result


def validate_read_only_object(metadata, placements):
    for entry in placements:
        # A compiler may emit one section per table under the same name (one per `switch`).
        # They are accepted as one placement only as a whole: every such section complete,
        # allocated and non-writable, laid out in object order with its own alignment, and
        # together exactly the size of the placement. The linked bytes are still compared
        # one by one against the original range by compare_read_only.
        sections = [s for s in metadata["sections"] if s["name"] == entry["section"]]
        if not sections:
            raise ValueError("read-only input section is missing")
        offset = 0
        for section in sections:
            if section["type"] != 1 or section["flags"] != 2 or section["size"] <= 0:
                raise ValueError("read-only input must be a complete allocated non-writable data section")
            alignment = section["alignment"] or 1
            if alignment & (alignment - 1):
                raise ValueError("read-only placement violates section alignment")
            offset += -offset % alignment
            if (entry["address"] + offset) % alignment:
                raise ValueError("read-only placement violates section alignment")
            offset += section["size"]
        if offset != entry["size"]:
            raise ValueError("read-only input must be a complete allocated non-writable data section")
    if placements:
        names = {e["section"] for e in placements}
        extras = [s for s in metadata["sections"] if s["size"] and s["flags"] & 2
                  and not s["flags"] & 4 and s["name"] not in names
                  and s["name"] not in {".reginfo", ".MIPS.abiflags"}]
        if extras:
            raise ValueError("comparison with read-only data emits unplaced allocated data")


def compare_read_only(original, metadata, rebuilt, rebuilt_metadata, placements, output):
    reports = []
    for index, entry in enumerate(placements):
        # The retail ELF combines code/data in a flattened RWX section. Its flags
        # cannot prove original const qualifiers; require a unique physical range.
        original_sections = [s for s in metadata["sections"] if s["type"] == 1
                             and s["address"] <= entry["address"]
                             and entry["address"] + entry["size"] <= s["address"] + s["size"]]
        if len(original_sections) != 1:
            raise ValueError("original table must lie in one file-backed section")
        sections = [s for s in rebuilt_metadata["sections"] if s["name"] == entry["section"]]
        if len(sections) != 1:
            raise ValueError("linked read-only section must be unique")
        section = sections[0]
        if (section["address"] != entry["address"] or section["size"] != entry["size"]
                or section["type"] != 1 or section["flags"] != 2):
            raise ValueError("linked read-only section has unexpected address, size or flags")
        a = file_offset(metadata, entry["address"], entry["size"])
        b = file_offset(rebuilt_metadata, entry["address"], entry["size"])
        before = original[a:a + entry["size"]]
        after = rebuilt[b:b + entry["size"]]
        report = compare_bytes(before, after)
        report.update(entry, elf_offset=a)
        (output / f"read-only-{index}.original.bin").write_bytes(before)
        (output / f"read-only-{index}.rebuilt.bin").write_bytes(after)
        reports.append(report)
    return reports


def validate_registry_builds(functions, rows):
    registry = {(r["module"], r["name"]): r for r in rows if r["kind"] == "function"}
    for function in functions:
        row = registry.get((function["module"], function["name"]))
        # A comparison unit may include the function's own source file (the registry's `source`).
        sources = [function["source"], *function.get("unit_sources", [])]
        if (row is None or row["status"] != "matching" or row["source"] not in sources
                or int(row["address"], 0) != int(function["address"], 0)
                or int(row["size"], 0) != int(function["size"], 0)
                or row["match_kind"] != function.get("match_kind", "c")):
            raise ValueError("function build differs from matching registry entry")


WARM_SOURCE = "double dotr_warm(double a, double b)\n{\n    return a * b;\n}\n"


def build_function(function, config, original, metadata, baseline, lock, overlay=None, output_root=None):
    """Build one function and compare it. `overlay` (tools/mod.py) is a directory whose file at the
    function's source path replaces the source; `output_root` then keeps the results apart."""
    name = function["name"]
    if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", name) or function["module"] != "ee-main":
        raise ValueError("only ordinary identifiers in the main EE module are supported")
    base = overlay if overlay is not None and (overlay / function["source"]).is_file() else ROOT
    source = (base / function["source"]).resolve()
    match_kind = function.get("match_kind", "c")
    source_root = base / ("asm/ee" if match_kind == "asm" else "src/ee")
    if match_kind not in {"c", "asm"} or not source.is_relative_to(source_root) or not source.is_file():
        raise ValueError("C source must be under src/ee; ASM source must be under asm/ee")
    if source.suffix.lower() != (".s" if match_kind == "asm" else ".c"):
        raise ValueError("function source differs from match kind")
    read_only = read_only_placements(function)
    headers = {}
    for filename in function.get("headers", [] if match_kind == "asm" else ["include/dotr/ee/menu.h"]):
        header = (ROOT / filename).resolve()
        if not header.is_relative_to(ROOT / "include") or not header.is_file():
            raise ValueError("function header must be under include")
        headers[filename] = hashlib.sha256(header.read_bytes()).hexdigest()
    # Optional comparison units: other sources included by the compiled source, and
    # other functions it defines that must sit at their observed addresses.
    unit_sources = {}
    for filename in function.get("unit_sources", []):
        included = (ROOT / filename).resolve()
        if not included.is_relative_to(ROOT / "src/ee") or not included.is_file():
            raise ValueError("unit source must be under src/ee")
        unit_sources[filename] = hashlib.sha256(included.read_bytes()).hexdigest()
    placements = {}
    for symbol, value in function.get("placements", {}).items():
        if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", symbol) or symbol == name:
            raise ValueError("invalid placement identifier")
        placements[symbol] = int(value, 0)
        if not 0 <= placements[symbol] <= 0xffffffff:
            raise ValueError("invalid placement address")
    compiler, artifact = checked_tool(lock, config["compiler"])
    # The compiler is a Windows program: Windows runs it directly, elsewhere wibo does.
    launcher, wibo_artifact = [], {"version": None}
    if not WINDOWS:
        wibo, wibo_artifact = checked_tool(lock, "wibo")
        launcher = [str(wibo)]
    linker, linker_artifact = checked_tool(lock, "mips-binutils")
    address, size = int(function["address"], 0), int(function["size"], 0)
    offset = file_offset(metadata, address, size)
    output = (output_root or ROOT / "out/research/ntsc-u/matches") / name
    output.mkdir(parents=True, exist_ok=True)
    for generated in ("comparison.json", "original.bin", "rebuilt.bin"):
        (output / generated).unlink(missing_ok=True)
    for generated in output.glob("read-only-*.bin"):
        generated.unlink()
    obj, linked, script = output / "function.o", output / "function.elf", output / "function.ld"
    assignments = []
    for symbol, value in function["globals"].items():
        if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", symbol):
            raise ValueError("invalid global identifier")
        value = int(value, 0)
        if not 0 <= value <= 0xffffffff:
            raise ValueError("invalid global address")
        assignments.append(f"{symbol} = 0x{value:x};")
    data_sections = "".join(f" {e['section']} 0x{e['address']:x} : {{ *({e['section']}) }}\n" for e in read_only)
    discard = data_sections + " /DISCARD/ : { *(.mwcats) *(.comment) *(.reginfo) *(.MIPS.abiflags) }\n}\n"
    link_object = obj
    if placements:
        # Every placed function gets its own output section; unplaced code is an error.
        link_object = output / "function.placed.o"
        placed = sorted({name: address, **placements}.items(), key=lambda item: item[1])
        script.write_text("\n".join(assignments) + "\nSECTIONS {\n" + "".join(
            f" .text.{symbol} 0x{value:x} : {{ *(.text.{symbol}) }}\n" for symbol, value in placed)
            + " .unplaced 0 : { *(.text) }\n" + discard, newline="\n")
    else:
        script.write_text("\n".join(assignments) + "\nSECTIONS {\n"
                          f" .text 0x{address:x} : {{ *(.text) }}\n" + discard, newline="\n")
    compile_command = [*launcher, str(compiler), "-c", *config["flags"], "-Iinclude",
                       "-o", str(obj), str(source)]
    warm = bool(function.get("warm", False))
    if warm and match_kind != "c":
        raise ValueError("only C sources can be compiled warm")
    if warm:
        # The original build compiled many files per compiler run. Once the compiler has called a
        # runtime helper with arguments in a0/a1 (double maths), later float-to-int casts are
        # allocated differently for the rest of the run. A warm function is compiled in one run
        # right after this warm-up file; only its own object is kept.
        warm_dir = output / "warm"
        warm_dir.mkdir(exist_ok=True)
        for stale in warm_dir.glob("*.o"):
            stale.unlink()
        warm_source = warm_dir / "dotr_warm.c"
        warm_source.write_text(WARM_SOURCE, newline="\n")
        compile_command = [*launcher, str(compiler), "-c", *config["flags"], "-Iinclude",
                           "-o", str(warm_dir), str(warm_source), str(source)]
    assembler_artifact = None
    if match_kind == "asm":
        assembler, assembler_artifact = checked_tool(lock, "mips-binutils", "assembler")
        compile_command = [str(assembler), "-EL", "-march=r5900", "-mabi=eabi",
                           "-o", str(obj), str(source)]
    link_command = [str(linker), "-T", str(script), "-e", name, "-o", str(linked), str(link_object)]
    env = dict(os.environ, MWCIncludes=str(ROOT / "include"))
    env["LD_LIBRARY_PATH"] = str(TOOLS / "mips-binutils/usr/lib/x86_64-linux-gnu")
    for command, log in ((compile_command, "compiler.log"), (link_command, "linker.log")):
        with (output / log).open("w") as stream:
            subprocess.run(command, cwd=ROOT, env=env, stdout=stream, stderr=subprocess.STDOUT,
                           check=True, timeout=60)
        if warm and log == "compiler.log":
            (warm_dir / (source.stem + ".o")).replace(obj)
        if placements and log == "compiler.log":
            link_object.write_bytes(rename_function_sections(obj.read_bytes(), [name, *placements]))
        if log == "compiler.log" and read_only:
            validate_read_only_object(inventory(obj.read_bytes(), allowed_types=(1,)), read_only)
    rebuilt = linked.read_bytes()
    rebuilt_metadata = inventory(rebuilt)
    if any(s["name"] == ".unplaced" and s["size"] for s in rebuilt_metadata["sections"]):
        raise ValueError("comparison unit emits code without a placement")
    candidates = [s for s in symbol_table(rebuilt, rebuilt_metadata) if s["name"] == name and s["type"] == 2]
    if len(candidates) != 1 or candidates[0]["value"] != address:
        raise ValueError("linked function symbol is missing or has an unexpected address")
    actual_size = candidates[0]["size"]
    rebuilt_offset = file_offset(rebuilt_metadata, address, actual_size)
    report = compare_bytes(original[offset:offset + size], rebuilt[rebuilt_offset:rebuilt_offset + actual_size])
    data_reports = compare_read_only(original, metadata, rebuilt, rebuilt_metadata, read_only, output)
    report["function_matching"] = report["matching"]
    report["matching"] = report["matching"] and all(r["matching"] for r in data_reports)
    (output / "original.bin").write_bytes(original[offset:offset + size])
    (output / "rebuilt.bin").write_bytes(rebuilt[rebuilt_offset:rebuilt_offset + actual_size])
    report.update(name=name, module="ee-main", address=address, elf_offset=offset,
                  iso_offset=baseline["iso_extent"]["offset"] + offset,
                  source=function["source"], original_executable_sha256=metadata["sha256"],
                  compiler_version=artifact["version"], compiler_sha256=artifact["executable_sha256"],
                  wibo_version=wibo_artifact["version"], linker_version=linker_artifact.get("version", "2.38"),
                  linker_sha256=linker_artifact["executable_sha256"], flags=config["flags"],
                  globals=function["globals"], compile_command=compile_command, link_command=link_command,
                  source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                  headers_sha256=headers, unit_sources_sha256=unit_sources,
                  placements={symbol: f"0x{value:x}" for symbol, value in placements.items()},
                  match_kind=match_kind, read_only_data=data_reports, warm=warm,
                  scope="function bytes and explicitly placed read-only data; full module/layout unverified")
    if assembler_artifact:
        report.update(compiler_version=None, compiler_sha256=None, wibo_version=None, flags=[],
                      assembler_version=assembler_artifact["version"],
                      assembler_sha256=assembler_artifact["assembler_sha256"])
    (output / "comparison.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"{name}: {'MATCH' if report['matching'] else 'NONMATCH'} "
          f"({report['original_size']}/{report['rebuilt_size']} bytes; {report['different_bytes']} differences)"
          + (f"; read-only data: {sum(r['different_bytes'] for r in data_reports)} differences" if data_reports else ""))
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--function", help="compare only this configured function")
    args = parser.parse_args()
    try:
        config = json.loads((ROOT / "config/ntsc-u/function-builds.json").read_text())
        lock = json.loads((ROOT / "config/tooling-lock.json").read_text())
        if config["schema_version"] != 1:
            raise ValueError("unsupported function build configuration")
        original, baseline = verified_elf()
        metadata = inventory(original)
        if metadata["sha256"] != config["original_sha256"]:
            raise ValueError("executable differs from the one recorded in function-builds.json")
        functions = [f for f in config["functions"] if not args.function or f["name"] == args.function]
        if not functions:
            raise ValueError("no configured functions selected")
        from project import symbols
        validate_registry_builds(functions, symbols())
        reports = [build_function(f, config, original, metadata, baseline, lock) for f in functions]
        return 0 if all(r["matching"] for r in reports) else 1
    except (OSError, ValueError, KeyError, StopIteration, subprocess.SubprocessError) as error:
        print(f"error: {error}; inspect out/research/ntsc-u/matches/*/*.log", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
