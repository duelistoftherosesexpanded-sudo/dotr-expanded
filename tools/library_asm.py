#!/usr/bin/env python3
"""Generate plain assembly for the library block from the prepared executable and prove it reassembles.

The library block (Sony and C-library code, outside the decompilation's scope) enters full builds as
assembly only (docs/DECOMPILATION.md, "Scope inside the executable"); `make rebuild` uses the same
routine for the startup code. The assembly is generated from the user's own executable into the
ignored build directory, never tracked. Words the pinned assembler does not reproduce from the
disassembly are kept as `.word`, so the result is byte-identical by construction and every such
word is counted.
"""

import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys

from dotr.elf import file_offset, inventory
from match import checked_tool
from research import verified_elf
from runtime import ROOT, TOOLS

START, END = 0x17B48C, 0x1A6C80
OUT = ROOT / "build/ntsc-u/library"
BRANCH = re.compile(r"^(b\w*|j|jal|bal)$")
GPR = re.compile(r"(?<![\w$.])(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)(?![\w])")
LINE = re.compile(r"^\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(\S+)\s*(.*)$")


def tools():
    lock = json.loads((ROOT / "config/tooling-lock.json").read_text())
    assembler, _ = checked_tool(lock, "mips-binutils", "assembler")
    linker, _ = checked_tool(lock, "mips-binutils")
    objdump = assembler.with_name(assembler.name.replace("-as", "-objdump"))
    env = dict(os.environ, LD_LIBRARY_PATH=str(TOOLS / "mips-binutils/usr/lib/x86_64-linux-gnu"))
    return assembler, linker, objdump, env


def disassemble(objdump, env, start, end):
    text = subprocess.run([str(objdump), "-d", "-z", "-m", "mips:5900", "-M", "no-aliases",
                           f"--start-address=0x{start:x}", f"--stop-address=0x{end:x}",
                           str(ROOT / "out/ntsc-u/SLUS_205.15")],
                          env=env, capture_output=True, text=True, check=True).stdout
    rows = {}
    for line in text.splitlines():
        m = LINE.match(line)
        if m:
            rows[int(m.group(1), 16)] = (int(m.group(2), 16), m.group(3), m.group(4).split("<")[0].strip())
    return rows


def render(rows, start, end, raw):
    targets = set()
    for address, (_, op, args) in rows.items():
        if BRANCH.match(op) and address not in raw:
            target = int(args.split(",")[-1], 16)
            if op not in ("j", "jal") and start <= target < end:
                targets.add(target)
    # .text input sections are 16-byte aligned: start at the boundary below and pad to the block's start
    lines = [".set noreorder", ".set noat", ".set nomacro", ".text", f".space {start - (start & ~15)}",
             ".globl library_block", "library_block:"]
    where = {}
    for address in range(start, end, 4):
        word, op, args = rows[address]
        if address in targets:
            lines.append(f"L_{address:08x}:")
        where[len(lines) + 1] = address
        if address in raw:
            lines.append(f"    .word 0x{word:08x}")
            continue
        if BRANCH.match(op):
            parts = args.split(",")
            target = int(parts[-1], 16)
            if op in ("j", "jal"):
                parts[-1] = f"0x{target:x}"
            elif start <= target < end:
                parts[-1] = f"L_{target:08x}"
            else:
                lines.append(f"    .word 0x{word:08x}")
                continue
            args = ",".join(parts)
        operands = GPR.sub(r"$\1", args)
        lines.append(f"    {op} {operands}".rstrip())
    return "\n".join(lines) + "\n", where


def assemble(source, assembler, linker, env, start, end):
    obj, elf, script = OUT / "library.o", OUT / "library.elf", OUT / "library.ld"
    subprocess.run([str(assembler), "-EL", "-march=r5900", "-mabi=eabi", "-o", str(obj), str(source)],
                   env=env, capture_output=True, text=True, check=True)
    script.write_text(f"SECTIONS {{\n .text 0x{start & ~15:x} : {{ *(.text) }}\n"
                      " /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n",
                      newline="\n")
    subprocess.run([str(linker), "-T", str(script), "-e", "library_block", "-o", str(elf), str(obj)],
                   env=env, capture_output=True, text=True, check=True)
    data = elf.read_bytes()
    return data[file_offset(inventory(data), start, end - start):][:end - start]


def build(start=START, end=END):
    original, _ = verified_elf()
    want = original[file_offset(inventory(original), start, end - start):][:end - start]
    assembler, linker, objdump, env = tools()
    rows = disassemble(objdump, env, start, end)
    if set(rows) != set(range(start, end, 4)):
        raise ValueError("disassembly does not cover the block word by word")
    OUT.mkdir(parents=True, exist_ok=True)
    raw = set()
    for round_ in range(40):
        source = OUT / "library.s"
        text, where = render(rows, start, end, raw)
        source.write_text(text, newline="\n")
        try:
            got = assemble(source, assembler, linker, env, start, end)
        except subprocess.CalledProcessError as error:
            rejected = {where[int(n)] for n in re.findall(r":(\d+): Error", error.stderr or "") if int(n) in where}
            if not rejected or rejected <= raw:
                raise ValueError("assembler rejected the generated source:\n" + (error.stderr or "")[:2000])
            raw |= rejected
            print(f'round {round_}: {len(rejected)} rejected by the assembler', file=sys.stderr)
            continue
        diff = {start + i for i in range(0, end - start, 4) if got[i:i + 4] != want[i:i + 4]}
        if not diff:
            (OUT / "library.bin").write_bytes(got)
            return dict(start=f"0x{start:x}", end=f"0x{end:x}", words=(end - start) // 4,
                        raw_words=len(raw), identical=True)
        print(f'round {round_}: {len(diff)} words differ', file=sys.stderr)
        raw |= diff
    raise ValueError("generated assembly does not converge to the original bytes")


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.parse_args()
    report = build()
    (OUT / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))


if __name__ == "__main__":
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    main()
