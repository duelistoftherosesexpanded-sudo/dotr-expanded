# Architecture

How the game's main executable is organised, how the build works, and which address spaces to
keep apart.

## The program by original module

The Japanese build keeps its function names, and the names carry a prefix per module. The game
code in scope (1,405,804 bytes) divides as follows; sizes are those of the US executable. All of
it byte-matches; the functions of each module, with exact byte counts, are described in
[modules/](modules/README.md).

| Prefix | Size | What it is | Staff-roll credit that covers it |
|---|---|---|---|
| `TaLogic...`, `L001`-`L036` | about 481 kB | Opponent AI: one entry routine per opponent (`L0nn`) and, per opponent, a near-identical family of decision functions (`TaLogic0nn_...`). The largest and most repetitive part of the program | Thought Logic Program: TRUTH |
| `Sz...` (`lSz...` file-local) | about 397 kB | The engine and everything the player sees of a duel. By sub-module: `SzDuel` 224 kB (the duel state machine `SzDuel_Act`, windows, hand, deck building, trade, custom duel), `SzModel` 48 kB, `SzView` 22 kB (camera), `SzGame` 28 kB (modes, menus, memory-card screens), `SzEffect` 15 kB, `SzData` 12 kB (save data, decks, ranks), `SzText`, `SzBattle`, `SzMsg` (text drawing), `SzFile`, `SzPad`, `SzSound`, `SzFade` (47 kB together) | Program Direction: JUN (no credit covers this module alone) |
| `Ta...`, `Chk_...` | about 182 kB | Duel rules: card effects (`Ta_Eff_...`), movement, traps, battle resolution, the tutorial's helpers (`TaTuto...`). The Japanese debug data names the original source file (under `C:\dm2\Prog\etc\`) of 281 of these functions | Card Rule Design: TRUTH, Morikuni KUBO, JUN |
| `Taku...`, `Tk...` | about 166 kB | The campaign: map, locations and routes, events and dialogue, the opponent and terrain windows, memory-card save and load screens | Campaign Part Program: Takuya HASHIMOTO |
| `Rh...` | about 47 kB | Low level: disc files (`RhFile`), models and geometry, memory card | VFX Program (four names; not attributable) |
| `Ex...` | about 14 kB | 3D mathematics and primitive drawing | as above |
| `Main_...` | about 2 kB | Start-up and the main loop | |
| other | about 117 kB | Sound and movie playback helpers and names with no module prefix | |

Outside this: the Sony and C library block `0x17b48c-0x1a6c80` (178,164 bytes, built with GCC, kept as assembly)
and the overlays in `DATA/*MODULE.MRG`. Tables and run-time layouts are in [GAME_DATA.md](GAME_DATA.md);
evidence for names, credits and dates in [topics/jp-build-symbols.md](topics/jp-build-symbols.md).

How a duel runs, as read from the code: `SzGame` holds the current mode and the four "menu numbers" a mode is
started with (for a duel: the duel number, the side, and each side's controller). `SzDuel_Init` sets the
duel block up from them; `SzDuel_Act` is called once a frame and switches on the duel phase, calling into
`Ta...` for rules and into `SzView`, `SzModel`, `SzEffect` and `SzSound` for presentation; a side whose
controller is below 100 is driven by its `L0nn` routine. The campaign (`TkCamp_Act`) sets the menu numbers
and pushes the duel mode.

## Build

1. `make prepare` identifies your disc by size and MD5 and extracts the executable.
2. `make match` compiles each function with Metrowerks 2.3.3, links it with the GNU linker at its
   original address against the original addresses of everything it calls or reads, and
   compares the bytes (and any switch tables).
3. `make rebuild` assembles the whole executable from those rebuilt functions plus generated
   assembly for the startup code and the library block, and compares the whole file.

The GNU linker is a comparison tool here, not evidence of the original linker or layout. A
build in which the linker places everything by name is the next step ([ROADMAP.md](ROADMAP.md)).

## Address spaces

Distinguish ISO byte offsets, LBAs, ELF file offsets, EE virtual addresses, IOP
addresses and overlay-relative addresses. For a file-backed load segment:

`ELF offset = virtual address - segment vaddr + segment offset`

`ISO offset = executable ISO extent + ELF offset`

This is valid only inside that segment's file-backed range. BSS has no original file bytes. Do not
apply a global subtraction constant to other segments or overlays. Derive mappings from
`out/ntsc-u/elf.json` (written by `make prepare`), and record the relevant module.

Overlays live in `DATA/CMODULE.MRG`, `BMODULE.MRG` and `MMODULE.MRG` and all run at the shared
address `0x01e00000`. The loader and the 762 present headers give 32 KiB transfers, initialized
and zero-fill extents, initializer-list bounds and dispatch entries; the sections inside an
overlay, the original packaging and any relocation format are unknown
([topics/overlay-loader.md](topics/overlay-loader.md)). The ELF's many program headers with no
file bytes are not overlay code. IOP modules are separate programs for another processor, with
their own ABI and toolchain.

## Source and types

`include/dotr/types.h` defines storage widths and a 32-bit address type. It does not
establish target pointer behavior or struct layout. Use explicit widths, assert
verified sizes/offsets, and keep unknown fields named by offset. Never use host
`long`, pointers or packing pragmas as substitutes for target ABI evidence.

`make host-check` compiles every source on your machine into per-group test programs with
stubbed callees and fixture data. Host tests check the C's logic; they say nothing about target
instructions, ABI or the running game.
