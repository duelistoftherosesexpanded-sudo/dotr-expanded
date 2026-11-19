# Tooling

## Pinned tools

`config/tooling-lock.json` pins three downloads by URL and SHA-256; `make tools` fetches,
verifies and installs them under `.tooling/`.

| Tool | Version | Used for |
| --- | --- | --- |
| Metrowerks CodeWarrior for PS2, `mwccps2` | 2.3.3 (000906) | compiling every function (`make match`) |
| wibo | 1.2.0 | running the Windows compiler on Linux |
| GNU binutils for MIPS (Ubuntu cross package) | 2.38 | linking, assembling, disassembling |

The compiler version was chosen by trying the available Metrowerks releases against the game's
code: 2.3.3 is the one whose output matches. `python3 tools/runtime.py binutils -- objdump ...`
runs the pinned binutils with their libraries.

## Project tools

All are Python 3.10+ standard library.

| Tool | |
| --- | --- |
| `tools/project.py` | `check` (registry), `verify` / `prepare` (your disc), `progress` (counts) |
| `tools/match.py` | compile and compare every function build entry; see [DECOMPILATION.md](DECOMPILATION.md) for warm builds, placements and read-only data |
| `tools/library_asm.py` | disassemble a range of your executable (the library block, the startup code) into plain assembly and prove it reassembles to the same bytes |
| `tools/rebuild_exe.py` | assemble the whole executable from rebuilt parts and compare it (`make rebuild`) |
| `tools/research.py` | executable and disc inventories, byte-range extraction |
| `tools/setup_tooling.py`, `tools/runtime.py` | install and run the pinned tools |
| `tools/dotr/` | ELF, ISO9660, byte-comparison helpers shared by the above |

Generated assembly, rebuilt executables and comparison reports stay in ignored `build/` and
`out/`; none of them may be committed, because they are derived from the game.

## What was used during the project but is not here

Development also used Ghidra (with an Emotion Engine language extension), PCSX2 run headless
for behaviour checks, objdiff, decomp-permuter and m2c. None is needed to build or check the
decompilation.
