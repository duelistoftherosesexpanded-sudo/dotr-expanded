#!/usr/bin/env python3
"""Assemble SLUS_205.15 from rebuilt parts and compare it with the original (stage 1 of a full build).

The whole code span is cleared, then filled only with: every registered function's rebuilt bytes and
read-only tables from its latest `make match` comparison, the library block and the startup code as
generated plain assembly (tools/library_asm.py). Everything outside the code span (ELF headers, data,
VU1 microcode) is still the original's bytes; this proves the code, not a linked build.
Run `make match` first: every comparison must be current (its source hash equals the file on disk).
"""

import hashlib
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dotr.elf import file_offset, inventory  # noqa: E402
import library_asm  # noqa: E402
from research import verified_elf  # noqa: E402
from runtime import ROOT  # noqa: E402

CODE_START, CODE_END = 0x100000, 0x286964
STARTUP_END = 0x1000D0
MATCHES = ROOT / "out/research/ntsc-u/matches"
OUT = ROOT / "build/ntsc-u/rebuilt"


def main():
    original, _ = verified_elf()
    metadata = inventory(original)
    image = bytearray(original)
    owner = {}

    def place(address, data, label):
        offset = file_offset(metadata, address, len(data))
        for i in range(0, len(data), 4):
            if address + i in owner:
                raise ValueError(f"0x{address + i:x}: {label} overlaps {owner[address + i]}")
            owner[address + i] = label
        image[offset:offset + len(data)] = data

    start = file_offset(metadata, CODE_START, CODE_END - CODE_START)
    image[start:start + CODE_END - CODE_START] = bytes(CODE_END - CODE_START)

    counts = dict(c=0, asm=0, read_only=0, library=0, startup=0)
    functions = json.loads((ROOT / "config/ntsc-u/function-builds.json").read_text())["functions"]
    for function in functions:
        name = function["name"]
        report = json.loads((MATCHES / name / "comparison.json").read_text())
        current = hashlib.sha256((ROOT / function["source"]).read_bytes()).hexdigest()
        if not report.get("matching") or report["source_sha256"] != current:
            raise ValueError(f"{name}: comparison is not a current match; run make match")
        rebuilt = (MATCHES / name / "rebuilt.bin").read_bytes()
        place(report["address"], rebuilt, name)
        counts[report.get("match_kind", "c")] += len(rebuilt)
        for index, entry in enumerate(report.get("read_only_data", [])):
            table = (MATCHES / name / f"read-only-{index}.rebuilt.bin").read_bytes()
            place(int(str(entry["address"]), 0), table, name + " read-only")
            counts["read_only"] += len(table)

    for lo, hi, key in ((CODE_START, STARTUP_END, "startup"),
                        (library_asm.START, library_asm.END, "library")):
        library_asm.build(lo, hi)
        data = (library_asm.OUT / "library.bin").read_bytes()
        place(lo, data, key)
        counts[key] += len(data)

    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "SLUS_205.15").write_bytes(image)
    same = bytes(image) == original
    differing = [] if same else [hex(a) for a in range(CODE_START, CODE_END, 4)
                                 if image[file_offset(metadata, a, 4):][:4] != original[file_offset(metadata, a, 4):][:4]]
    report = dict(identical=same, sha256=hashlib.sha256(image).hexdigest(),
                  original_sha256=hashlib.sha256(original).hexdigest(), functions=len(functions),
                  bytes=counts, zero_filled=(CODE_END - CODE_START) - 4 * len([a for a in owner if a < CODE_END]),
                  differing_words=len(differing), first_differences=differing[:20],
                  scope="code span 0x100000-0x286964 rebuilt; headers, data and VU1 microcode are original bytes")
    (OUT / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    return 0 if same else 1


if __name__ == "__main__":
    raise SystemExit(main())
