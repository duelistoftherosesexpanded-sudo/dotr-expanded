# Duelists of the Roses Expanded

**Duelists of the Roses Expanded** (v0.55 beta) is a mod built on the decompilation below. It adds
hundreds of new cards with 3D models, card eras you can switch on and off in MOD OPTIONS, a card
shop, achievements, coloured card frames and widescreen. The mod's code is in
[mods/widescreen/](mods/widescreen/) and its tools are in [tools/](tools/).

No game data is in this repository: no disc images, models, textures or card art. The setup tool
(`tools/setup_tool.py`) builds everything from your own copies of the games: Duelists of the Roses, The Dawn of
Destiny, The Falsebound Kingdom, Cross Duel and Saikyo Card Battle.

---

# Duelists of the Roses decompilation

A byte-matching decompilation of **Yu-Gi-Oh! The Duelists of the Roses** for the PlayStation 2,
USA release (`SLUS_205.15`).

Every function of the game's own code in the main executable is written in C that the original
compiler turns into exactly the original bytes: **2,568 functions, 1,405,804 bytes** (2,555 in C,
13 VU0 routines in assembly). Built together with the Sony library and startup code (as assembly
generated from your own executable at build time, not stored here), the whole executable comes
out byte-identical to the original.

Function names are the developers' own, recovered from the Japanese release, which shipped with
its symbol table. What the code does is described module by module in
[docs/modules/](docs/modules/README.md); the game's tables and save layout in
[docs/GAME_DATA.md](docs/GAME_DATA.md).

No game data is included. You need your own copy of the game.

This is an unofficial fan project. It is not affiliated with, endorsed by or sponsored by Konami.
Yu-Gi-Oh! and Yu-Gi-Oh! The Duelists of the Roses are trademarks of their respective owners.

## What you need

- Linux x86-64 (the pinned tools are Linux builds), Python 3.10 or newer, `dpkg-deb`.
- For host tests: CMake 3.16+ and a C11 compiler.
- Your own unmodified USA disc image (2,112,782,336 bytes, MD5
  `ca838dae26ff750936c0814d1c2d33f1`).

## Quick start

```sh
make tools        # download and verify the pinned compiler, wibo and MIPS binutils into .tooling/
make test         # ROM-free checks (registry, tool tests)
make host-check   # compile all the C on this machine and run its tests

cp /path/to/your.iso baserom/
make prepare      # verify the image, extract SLUS_205.15 into out/
make match        # compile every function and compare it with your executable
make rebuild      # build the whole executable and compare it byte for byte
```

`make rebuild` writes the rebuilt executable and a comparison report to `build/ntsc-u/rebuilt/`.
`make help` lists every target. [docs/SETUP.md](docs/SETUP.md) has the details.

## Layout

| Path | Contents |
| --- | --- |
| `src/ee/<group>/func_<address>.c` | one function per file, grouped by subject |
| `include/dotr/ee/` | per-group headers: types, data views, callee declarations |
| `asm/ee/vu0/` | the 13 VU0 routines kept as assembly |
| `config/ntsc-u/` | symbol registry, build entries, Japanese names, disc identity |
| `tests/` | host tests (C) and tool tests (Python) |
| `tools/` | preparation, matching, library assembly, whole-executable rebuild, tool setup |
| `docs/` | rules, data reference, module descriptions |
| `baserom/`, `out/`, `build/`, `.tooling/` | your disc, generated files and tools; never committed |

## Documents

| Document | |
| --- | --- |
| [Matching rules](docs/DECOMPILATION.md) | what counts as a match, compiler and flags, what a source may contain, scope |
| [Modules](docs/modules/README.md) | every function, by original module, with a description |
| [Game data](docs/GAME_DATA.md) | identifiers, tables, decks, campaign, save data |
| [Architecture](docs/ARCHITECTURE.md) | how the program is organised |
| [Disc identity](docs/FINDINGS.md), [ELF map](docs/ELF_MAP.md), [module inventory](docs/MODULE_INVENTORY.md) | the disc, the executable, overlay and IOP modules |
| [Roadmap](docs/ROADMAP.md) | what is done and what comes next |
| [Tooling](docs/TOOLING.md), [Setup](docs/SETUP.md) | tools and commands |
| [Credits](docs/CREDITS.md), [References](docs/REFERENCES.md), [Licensing](docs/LICENSING.md) | contributors, sources used, licences |

## Credits

- Code for about 165 functions (about 395 kB, including the hardest), and the original-file and
  warm-compile techniques that made the last of them match, come from
  [kira-502/dotr-decomp](https://github.com/kira-502/dotr-decomp) (CC0-1.0); the functions are
  listed in [docs/CREDITS.md](docs/CREDITS.md).
- Data labels, structure and enum definitions, parameter names and comments draw on
  reverse-engineering work by **batzpup** (Ghidra project for `SLUS_205.15`, 2019), used with
  permission; details in [docs/CREDITS.md](docs/CREDITS.md).
- Other references are listed in [docs/REFERENCES.md](docs/REFERENCES.md).
