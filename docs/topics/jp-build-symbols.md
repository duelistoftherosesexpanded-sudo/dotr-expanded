# Japanese build symbols and how this project uses them

The Japanese release (SLPM-65050, `VER = 2.00`) shipped with its executable's symbol table and
some debug information. The comparison below was made against a Japanese disc in a separate
project; neither that disc nor anything extracted from it is part of this repository.

## Use in this project

- `config/ntsc-u/jp-names.csv` lists every US address that received a Japanese function, found
  by comparing function bodies between the two executables or, for `gap-order` rows, placed by
  position (below). Each row gives the Japanese name, address and size, the match level and how
  the hit was resolved; `c_name` is the identifier this project uses.
- Every function with a usable name carries it in sources, headers, tests and the build
  configuration. Source file names keep the address (`src/ee/<group>/func_XXXXXXXX.c`) until
  sources are regrouped by original file.
- A name is evidence of identity, not of bytes: `make match` compares every registered function
  over its full range regardless of its name.
- Names not used as identifiers: `ambiguous` (two Japanese functions at one address),
  `collision` (a C library name, or already used here), `library` (see below). A name shared by
  file-local functions of several source files carries `__<address>`.
- Level 2 hits on very small functions can be coincidences. The table keeps the level so that a
  reader can discount them.

## How names were placed

- The 287 SECTION symbols in the Japanese `main` do not give source-file boundaries for game
  code. They delimit the library objects; the game code before and after the libraries is one
  linked block each. For game code the source file is known from the debug information (87
  files) and is otherwise inferred from the name prefix (`lSzGame_X`, `SzGame_Y`) of neighbours
  in link order.
- The library block of the US executable is `0x17b48c-0x1a6c80`: functions placed from the
  Japanese library range (`0x17b400-0x1a4fb0`, kernel stubs and 169 objects marked
  `gcc2_compiled.`) reach up to `0x1a6c80` (`strcat` at `0x19b9d0`, `strlen` at `0x19bef0`), and
  the first game function after them, `SzGame_Init`, is at `0x1a6c80`.
  [DECOMPILATION.md](../DECOMPILATION.md) keeps this block out of scope.
- Names placed by position: of 2,578 US function starts in the two game-code ranges, 21 had no
  name after body matching. Seven of them sit at the position of an unplaced Japanese function
  of the same size (between the same two matched neighbours) and take its name with match level
  3 (`gap-order`): `PAINT_PIRM_G4` 0x108f90 and `PAINT_PIRM_GT4` 0x108ff0 (96 bytes each, the two
  functions of the gap before `PAINT_PIRM_LG2`), `SzGame_Init` 0x1a6c80 (US 876 bytes, Japanese 872),
  `SzGame_DrawMenu` 0x1a6ff0 (904) and `SzGame_ActMenu` 0x1a7380 (3,932) in that order,
  `Ta_Eff_070` 0x275740 (652) and `Ta_Eff_071` 0x2759d0 (816) between `Ta_Eff_BTL_002` and
  `TaLogic_InitLogicData`. The ambiguous 0x1efd30 (identical bodies `SzModel_ShotTextureWithFunc`
  and `ExSortSoftFocus` in the Japanese build) is `SzModel_ShotTextureWithFunc`: it lies among the
  SzModel functions, and the US build has no function between `ExSortDispBlur_Pipe` and
  `ExSortSoftFocus_Pipe` where `ExSortSoftFocus` would be. Of the other 13 unnamed starts: 0x100610
  has no Japanese counterpart (US-only code; named `useSoftFocus` from a third-party project, see
  [REFERENCES.md](../REFERENCES.md)); 0x1000d0 `main` and 0x242580 `movie` keep their
  collision notes; 0x1e0c78 is 8 bytes of padding; nine at 0x28c2b0-0x28d5b8 are kernel/library
  code beyond the Japanese list (0x28c698 is `iSetAlarm`). Without the padding and those nine,
  2,568 functions remain: the registered game code. Function starts were taken from `jal`
  targets, the registry and a third-party Ghidra project's function list (boundary evidence only).
- Function starts: an address where a Japanese function was placed is a function start even
  when nothing calls it with `jal`. This splits ranges that a heuristic based on `jal` targets
  takes for one large function (for example `0x1039a0`: 140 bytes, not 18,056).
- Functions that changed for the US release are named by position (`match_level` 3,
  `gap-order`): where two neighbouring placed functions enclose the same number of functions
  in both builds, they are paired in order. `jp-names.csv` has 355 such rows: 284 in the game
  code, all used as identifiers (among them the seven above, the AI entries below and the largest
  function of the game code, `SzDuel_Act` at `0x1cee60`, 36,528 bytes here and 37,000 in the
  Japanese build), and 71 in the library block, recorded but not used. Only the name is carried
  over for these.
- Starts reached only through data pointers: the table of opponent AI entry functions at
  `0x2baeb4` (22 entries, `L001`..`L022`, then filler) and similar pointers expose 12 functions
  that no `jal` reaches (`L002`, `L007`, `L009`, `L013`, `L014`, `L017`, `L019` and the tutorial
  routines `L032`..`L036`); they are named by position like the others.
- Outside check: the community function map
  ([GenericMadScientist/DotR-Documentation](https://github.com/GenericMadScientist/DotR-Documentation),
  no licence stated, commit `cb5bf0b`) lists 61 NTSC-U functions; the 53 that are in
  `jp-names.csv` have the same name at the same address, the other 8 are C library functions.
  The opponent each AI entry belongs to (`L001` Seto ... `L022` Deck Master K) is reported by
  [batzpup/DotrModdingToolRedux](https://github.com/batzpup/DotrModdingToolRedux) (GPL-3.0,
  `GameData/Ai.cs`; addresses and names read as facts, no code used) and is not verified here.
- Data names: for every function found in both builds the two bodies differ only
  in addresses, so each `lui` + load/store/`addiu` pair names one Japanese data address and the
  US address that corresponds to it. A Japanese object symbol containing such an address gives
  its name to the US address at the same offset. `config/ntsc-u/jp-data-names.csv`: 430 objects;
  257 placed with every pairing agreeing (`pairs`), 165 by position between two neighbours that
  moved by the same amount (evidence only, not used as identifiers), 8 disputed. 206 of the
  `pairs` objects carry their Japanese name as identifier (`c_name`). Outside check:
  `EnchMatch` `0x29d580`, `gSzModel_ahModel` `0x31abb0` and the save data at `0x32f790` equal
  the community RAM map.
- Not used: overlay function names (the overlays are not decompiled here); names of
  function-static objects (`name$123`), whose objects keep their address names. Japanese
  functions in gaps whose function counts differ between the builds have no US address.

---

## The Japanese executable compared with the US one

A read-only comparison of the Japanese disc ("Yu-Gi-Oh! Shin Duel Monsters II - Keishou Sareshi
Kioku", SLPM-65050) with the US disc (SLUS-20515); nothing extracted from either disc is part
of this repository. Nothing here was run in an emulator.

**Summary.** The Japanese main executable is **not stripped**. It keeps a
full ELF symbol table (26,433 entries, 12,088 function symbols, 7,980 distinct
function names), named sections for every code overlay, relocation tables, and a
Metrowerks DWARF 1 debug section for part of the game code (87 source files with
`C:\dm2\Prog\...` paths, 420 functions with parameters and locals, struct
layouts). The US executable has none of this. The code is close enough that most
names transfer to the US build mechanically.

## Identity

| Item | Japan | USA |
| --- | --- | --- |
| Image size (bytes) | 2,259,222,528 | 2,112,782,336 |
| Image MD5 | `6b4d054b20516d4ac041c849cef2d5c7` | `ca838dae26ff750936c0814d1c2d33f1` |
| ISO volume creation date | 2001-10-02 12:13:28 | 2002-11-20 15:34:02 |
| `SYSTEM.CNF` | `BOOT2 = cdrom0:\SLPM_650.50;1`, `VER = 2.00`, `VMODE = NTSC` | `BOOT2 = cdrom0:\SLUS_205.15;1`, `VER = 1.00`, `VMODE = NTSC` |
| Executable | `SLPM_650.50`, 6,474,548 bytes, dir date 2001-10-02 12:06:39 | `SLUS_205.15`, 2,223,064 bytes, dir date 2002-11-12 19:33:46 |
| Executable MD5 | `afd56d732602da8e3d8fec8fc6e753b1` | `238a9eb09ee00a4215474a68e1cbb3a6` |
| Executable SHA-256 | `87aefc3d74696de3dac340fc5086349e42d26030233cd39108850432bb82868d` | `1df7759b68e6aee73d8deab2efd2e556ec553d952af00fe8bf0f499a57298719` |
| Files on disc | 98 | 97 |

The disc identifies itself as **VER 2.00**. This image is therefore the v2.00
revision mentioned by outside sources; a v1.03 disc was not available and
whether it has the same symbols is unknown.

## Findings

Status: **verified** = checked on the images here; **reported** = from an
outside source; **guess** = inference not checked.

| Finding | Status | How checked |
| --- | --- | --- |
| JP image is 2,259,222,528 bytes, MD5 `6b4d054b...` | verified | `md5sum` on the image |
| JP disc says `VER = 2.00`, boots `SLPM_650.50` | verified | `SYSTEM.CNF` extracted with the ISO reader |
| Japanese revisions v1.03 and v2.00 exist | reported | outside sources; only the v2.00 disc was examined |
| JP ELF has `.symtab` with 26,433 entries and a 402,435-byte `.strtab` | verified | `readelf -S`, `readelf -s` |
| 12,088 FUNC symbols: 3,417 in `main`, 8,671 in overlay sections; 7,980 distinct function names | verified | a separate ELF parser |
| JP ELF has 2,438 section headers against 841 in the US ELF; both have 835 program headers and entry `0x100008`, flags `0x20924000` | verified | `readelf -h` on both |
| The 833 overlay segments are named in JP (`c000`..., `b001`...`b853`, `m000`, plus `heap` at `0x1fe0000`); the same 833 sections are unnamed in the US ELF | verified | section tables of both ELFs |
| JP ELF has `.debug` (764,505 bytes) and `.line` (148,338 bytes) in DWARF 1 format; no `.mdebug`, no STABS, no DWARF 2 | verified | section table; whole `.debug` parsed without error as DWARF 1 |
| DWARF covers `0x244460..0x284a28` (260,884 bytes), 87 source files, 420 `global_subroutine` entries, 1,242 parameters, 1,219 locals, 1,801 structure types with 12,084 members | verified | DWARF 1 dump |
| Original source tree was `C:\dm2\Prog\etc\...`; the slot-machine library came from `C:\project\slotLIB\src\`; one file from `Y:\Effect\SRC\Tasaki\` | verified | `AT_name` of DWARF compile units |
| JP ELF has 761 relocation sections (`.relmain` with 42,557 entries, one `.rel<overlay>` each for 760 overlays), 235,569 entries in all | verified | section table; reloc types in `.relmain` are R_MIPS_26/HI16/LO16/32 |
| JP ELF has 834 Metrowerks `.mwcats` sections (90,216 bytes) | verified | section table; contents not decoded |
| Both executables carry the same `.comment`: `MW MIPS C Compiler (2.3.1.01)` / `PlayStation2`; DWARF `AT_program` is `MW MIPS C Compiler` | verified | `readelf -p .comment`; DWARF dump |
| 169 objects in JP `main` are GCC-built (`gcc2_compiled.` markers, `0x17bc90..0x1a4e90`), i.e. the Sony libraries; game code sits before (`0x1000c0..0x17b400`) and after (`0x1a4fb0..0x284a84`) | verified for the markers; "Sony libraries" is a guess | `gcc2_compiled.` symbols; library names such as `sceDeci2Open`, `sceTtyWrite` |
| US ELF is stripped (`.symtab`/`.strtab` present but empty, no debug sections) | verified | `readelf -S` on `SLUS_205.15`; agrees with [ELF_MAP.md](../ELF_MAP.md) |
| Overlay archives have the same shape in both regions: `CMODULE.MRG` 32,768,000 bytes / 78 `MWo3` slots, `BMODULE.MRG` 22,380,544 / 683, `MMODULE.MRG` 22,380,544 / 1; 762 in all, same slot numbers | verified | archive headers of both discs |
| Overlay files carry no names in either region (no symbols, no extra strings) | verified | whole-image string scan, no hits inside `*MODULE.MRG` |
| 739 of 762 overlays have identical header size fields in both regions; 811 of 835 program-header memory sizes are equal; only 2 overlays are byte-identical (addresses into `main` differ) | verified | archive headers and program headers of both |
| The overlay header word at `+0x04` is the program-header index, and the JP section of that index gives the overlay's name | verified | 762/762 headers resolve to a named JP section; for the C archive the name equals letter + slot number (`c006` = C slot 6); B-archive names (`b357` = B slot 1) are not slot numbers |
| Of 8,600 JP function symbols in overlays present on disc, 8,445 match the US overlay at the same offset, 113 match at a moved offset, 42 do not match | verified | per-overlay comparison of function bodies |
| Of 3,417 JP function symbols in `main`, 2,721 are located in the US executable (2,281 unique pattern hits + 440 resolved by link order); 547 not found, 149 ambiguous | verified | comparison of function bodies (see below) |
| Of the 1,839 functions this project had registered at the time of the comparison, 1,783 (97%) have a pattern hit in JP; 1,296 hits are unique and all 1,296 land exactly on a JP symbol of identical size; 1,676 (91%) receive a JP name through the placement of `main` functions above | verified | comparison of function bodies |
| The one name known beforehand is confirmed: US `0x1a82e0` = JP `0x1a6610` `SzGame_GetMenuNum`, size 0x38 | verified | comparison of function bodies |
| Function order is preserved between regions (6 inversions among 2,721 placed functions; 97 distinct address shifts) | verified | comparison of function bodies |
| Unmatched JP functions are mostly Sony library code (408 of 845 library functions unplaced, library versions differ) and text/menu/memory-card drawing code (`*_Pipe`, `Taku_Camp_*`, `TkMc_*`) | verified for the counts; that localisation is the cause is a guess | comparison of function bodies |
| Sony library tags: JP `PsIIlibkernl2140`, `libgraph2100`, `libdma 2100`, `libipu 2100`, `libsdr 2100`, `libcdvd 2130`, `libmc 2140`, `libpad 2130`; US `2430/2400/2400/2400/2400/2420/2430/2410` | verified | strings of the loaded segments |
| IRX modules expose the same things in both regions: only `KCEJEAST.IRX` has `.mdebug` and symbols (JP 397 symbol records / 187 functions, US 398 / 188; the only extra name is US `SD_vTransferBankBody_Ex`); the six Sony modules have just the null symbol | verified | `readelf -s`, CCC `stdump identify/files` |
| IRX import tables hold library names and ordinals only (no function names) in both regions | verified | import-stub tables parsed (`0x41e00000` records) |
| KCEJEAST source paths differ: JP `/u/home/user/usr03333/sdps2/sdiop/*.c`, US `/u/home/user/usr04623/sdIop/*.c`; 53 file entries each. The sound driver was compiled from a Unix home directory, under a different account for each release (the game code itself was built on Windows: `C:\dm2\Prog\...`, below) | verified | CCC `stdump files` |
| No `.MAP`, `.SYM`, `.LCF`, `.ELF`, `.BAK` or other build files on either disc; file sets are equal except the movie names and executable | verified | directory walk of both discs |
| No data hidden in unused space: no nonzero slack after any file, no gaps between files; only the system area before the first file and a 14-byte trailer are outside files | verified | scan of the ISO 9660 extents of both discs |
| `MOVIE/DUMMY.PSS` is byte-identical in both regions (MD5 `4cfa6c8c...`) and is a real MPEG program stream | verified | MD5 and pack-start code |
| The loaded segment of the JP executable adds almost no name-like strings over the US one (no source paths, no assert strings in either) | verified | string comparison; 17 name-like JP-only strings, e.g. `Apply_N_Cmd 0`, `sceSifWriteBackDCache addr= 0x%08x size= %d` |
| The larger JP loaded segment (`0x27db80` vs `0x20ff00` file bytes) is data, not code: JP code ends near `0x284b00`; US game code ends at `0x286964`, followed by VU1 microcode to `0x290000` | guess | from symbol addresses (JP) and this project's layout (US, [ELF_MAP.md](../ELF_MAP.md)); the JP data was not classified |
| A debug menu module survives as names in JP: `SzDebug_Init`, `SzDebug_Act`, `SzDebug_GetActiveMenu`, `SzDebug_OutputFrameBuffer`, `lSzGame_TEXTURE_TEST`, `SzDuel_ActCardTest` | verified | symbol table; the bodies are also found in the US executable (`SzDebug_Init` at US `0x203cd0`, `SzDebug_Act` `0x203e20`, `lSzGame_TEXTURE_TEST` `0x1a9f50`, level 1-2 unique hits); whether the code is reachable was not checked |

## Side by side: ELF headers and sections

| | `SLPM_650.50` (JP) | `SLUS_205.15` (US) |
| --- | --- | --- |
| Type / flags / entry | EXEC, `0x20924000`, `0x100008` | same |
| Program headers | 835 | 835 |
| Main segment | file `0x27db80`, memory `0x2bf500` at `0x100000` | file `0x20ff00`, memory `0x251300` |
| Section headers | 2,438 | 841 |
| `.shstrtab` | 17,754 bytes | 45 bytes |
| `.strtab` / `.symtab` | 402,435 / 422,928 bytes | 0 / 0 |
| Main section | named `main`, with `.relmain` | unnamed |
| Overlay sections | 833 named, 760 with `.rel*` | 833 unnamed |
| `.mwcats` | 834 | none |
| `.debug` / `.line` | 764,505 / 148,338 bytes | none |
| `.comment` | `MW MIPS C Compiler (2.3.1.01)`, `PlayStation2` | same |
| `.reginfo` | present | present |

## The JP symbol table

| Kind | Count |
| --- | --- |
| FUNC in `main` | 3,417 (2,515 global, 902 local) |
| FUNC in overlays | 8,671 (7,946 in `b*`, 724 in `c*`, 1 in `m000`) |
| OBJECT in `main` | 1,020 (739 with real names, 689 distinct; rest compiler-made `@123`, `@stringBase0`) |
| OBJECT in overlays | 1,340 (157 with real names) |
| NOTYPE global | 10,873, almost all linker symbols per overlay (`_b853_text_start`, `_b853_data_size`, ...) |
| SECTION | 287 in `main`: 183 `.text`, 54 `.rodata`, 29 `.data`, 19 `.bss`, `.vutext`, `.vubss` (object-file boundaries) |
| FILE | 0 |

Roughly 2,572 `main` functions are game code and 845 are runtime/Sony library.
Naming is C with a module prefix per programmer or subsystem; `l` marks
file-local functions, `g`/`s_` globals and statics. Largest families in game
code: `Ta*` (1,011; duel AI `TaLogicNNN_*`, tutorial `TaTuto_*`), `Sz*` and
`lSz*` (871; `SzGame_`, `SzDuel_`, `SzMsg_`, `SzModel_`, `SzData_`, `SzDebug_`),
`Rh*`/`lRh*` (163; engine: `RhFile_`, `RhBase_`, `RhMath_`), `Taku*`/`Tk*`
(campaign map, memory card), `Sh*` (`ShFx_`, `ShSlot_`), `Ex*`, `Lk*`, `SD_*`
(sound). Overlay functions are effect routines: `C006_OBJ_MOVE_2_`,
`C017PAINT_RAIN3`, `ShFx_MvLight`, `LkPrim_SortSpot`, `IWMagicSmoke3`,
`POLPAN_UVIN_FT4`; 4,736 distinct names over the present overlays. Examples of
data names: `FusionDat`, `DeckDat`, `Destiny_Tab`, `EnchData`, `EnchMatch`,
`gMain_ModelData`, `gSzGame_abMenu`.

## DWARF 1 coverage

Only the last part of the game code was compiled with debug information: the
duel rules and effect sequencer in `C:\dm2\Prog\etc\` (`driver.c`, `util.c`,
`hand.c`, `Enchant.c`, `Leader.c`, `PermEfc.c`, `Effect.c`, `Trap.c`,
`TrapB.c`, `Magic.c`, `fusion.c`, `tuto.c`, `Heap.c`, `SetMoveArea.c`,
`EndPhase.c`, `EndBattle.c`, `TurnStanby.c`, `AtkEfc.c`, `ActEfc.c`,
`finishmove.c`, `talk.c`), the per-effect files `C:\dm2\Prog\etc\Effect\SeqNNN.c`,
`SeqUtil.c`, `SeqBtl.c`, `SeqHand.c`, the slot library `ShSlot.c`,
`ShSlot_Flear.c`, and `Tasaki_Eff.c`. For these 420 functions the debug data
gives return type, parameter and local names with locations, line tables, and
the struct layouts in use (mostly anonymous typedef'd structs, 336 distinct
member names; named tags: `cardcor`, `TreeTag`, `PieceTreetag`). The other
functions have names and sizes only. A Metrowerks vendor entry (tag `0x4080`,
833 times) repeats the overlay name and address range.

## Transfer to the US decompilation

Method: instruction words compared at
three levels: 0 exact bytes; 1 with `j`/`jal` targets, `lui` immediates and
`gp`-relative offsets masked; 2 additionally with all immediates on non-`sp`
base registers masked. A function is "placed" when its whole body is found at
exactly one aligned address, or at several but only one lies between the
neighbouring unique hits.

| Set | Total | Exact | Level 1 | Level 2 | Not placed |
| --- | --- | --- | --- | --- | --- |
| JP `main` functions placed in US | 3,417 | 473 | 1,320 | 928 | 696 (547 no hit, 149 ambiguous) |
| of which game code | 2,572 | | | | 288 |
| of which library code | 845 | | | | 408 |
| JP overlay functions in US overlays | 8,600 | 698 | 7,706 | 154 | 42 |
| registered US functions hit in JP (at the time) | 1,839 | 162 | 1,002 | 619 | 56 |

Random sample of 200 of those registered functions: 190 hit (139 uniquely), 174 receive a
JP name. Hit rate by size class over all 1,839: 100% up to 0x20 bytes (but
often ambiguous), 99.7% up to 0x80, 95% up to 0x200, 94% above.

The resulting US names are in `config/ntsc-u/jp-names.csv` (with the additions described under
[How names were placed](#how-names-were-placed)); the per-overlay results are not part of this
repository.

Limits. Level 2 hits on very small functions can be coincidental; the 1,296
unique hits of registered functions all fell on a JP symbol of identical size,
which supports the method but does not prove each small match. Data symbols were
mapped later through the masked `lui`/`addiu` pairs of matched functions
(`config/ntsc-u/jp-data-names.csv`, see above). The 71 JP overlay sections without a slot on
disc (`b071`, `b365`, `b720`, `b775`..`b842`) have names but no code to compare. Functions that
changed for the US release are named by position only; their bodies differ.

## Other aids for decompilation

- **Relocations** (`.relmain`, `.rel<overlay>`): they mark every address
  reference in JP code and data, separating pointers from constants.
- **Object boundaries and link order**: 287 SECTION symbols and 169
  `gcc2_compiled.` markers in `main`; local symbols are grouped per object in
  the symbol table.
- **Per-overlay linker symbols**: `_bNNN_text_start/_text_size/_data_start/
  _data_size/_bss_size/_static_init`, giving the text/data split of each overlay.
- **Compiler**: both regions report MW MIPS C Compiler 2.3.1.01; game code is C
  (DWARF language code 2), with `__initialize_cpp_rts` linked at the end.
- **Sony libraries**: JP uses the 2.1.x SDK generation, US 2.4.x, so library
  functions should be identified from the matching SDK release, not by
  cross-region comparison.
- **IOP**: `KCEJEAST.IRX` (`KCEJ_EAST_IOP_Driver1.5`, `PsIIlibspu2 1600` in
  both) already has `.mdebug` in the US release; the JP one adds nothing.
  `IOPRP.IMG` is 140,945 bytes in JP and 253,201 in US (contents not unpacked).

## Staff credits and dates

What the staff roll and build dates can and cannot say about who wrote what.

| Claim | Status | How checked |
|---|---|---|
| The executables are dated 2001-10-02 12:06:39 (`SLPM_650.50`) and 2002-11-12 19:33:46 (`SLUS_205.15`), both UTC+9 | verified | ISO 9660 directory records of the two images |
| Neither executable contains a build date string, and the DWARF 1 data carries no dates | verified for strings of 8+ characters in the Japanese ELF; DWARF by its unit attributes | `strings`, DWARF 1 compile units |
| So a function cannot be dated. A body equal in both builds (match level 0-2) existed by 2001-10-02; nothing more follows | reasoning | |
| The staff roll is not text on the disc: it is drawn in the ending movie `MOVIE/EDMV_A5.PSS` (120 s, MPEG-2) | verified | no hit for staff names in the image or the string table; frames read from the movie |
| Programming credits in that movie: Program Direction JUN; Campaign Part Program Takuya HASHIMOTO; Thought Logic Program TRUTH; VFX Program UKIN, Toshiya SHIMIZU, Shin'ichi TASAKI, SHOPPE; Sound Program Masayuki TAKAHASHI, Kazuhiko INABA; Card Rule Design TRUTH, Morikuni KUBO, JUN; "Created by Konami JPN EAST" | verified | frames of the movie, one every 4 s |
| `Taku...`/`Tk...` functions (campaign, memory-card screens) are Takuya Hashimoto's | likely | the credit names exactly that part; "Taku" only ever occurs as a prefix |
| `TaLogic...` (the 22 AI routines) are TRUTH's | likely | "thought logic" is the AI |
| The `Ta_Lk...` functions and `TaLib_c615` are Shin'ichi Tasaki's (VFX Program), and probably the `Lk...` helpers too | likely | their source file is `Y:\Effect\SRC\Tasaki\Tasaki_Eff.c`; the `Lk...` helpers share the `Lk` stem and coordinate and flag style |
| The other `Ta...` code (the effect sequences `Seq000.c` ... `SeqBtl.c` under `C:\dm2\Prog\etc\Effect`, and the rules in `Magic.c`, `Trap.c`, `EndPhase.c` and others) | guess | "Ta" may abbreviate Tasaki (his own file uses it) or stand for TRUTH (first under Card Rule Design). Against Tasaki for the sequences: their work blocks embed a member the Japanese debug data names `nTasakiEfc` ("Tasaki's effect"), run through `TaLib_c615`, as another author would name a colleague's part. Neither reading is established |
| `Sh...` code (the slot machine `ShSlot...` from a separate `C:\project\slotLIB` project, the effect helpers `ShFx...`, the face parts `ShFace...`) is one VFX programmer's: Toshiya Shimizu or SHOPPE | guess | the credits list both under VFX Program; only the prefix points at them |
| The `G...` routines and the all-caps helpers (`PAINT_...`, `OBJ_...`, `MATRIX_...`, `XYZ_...`) form one small library by one author | guess | they call only each other and share a naming style found nowhere else; no credit points to an author |
| Authors of `Sz...`, `Rh...`, `Ex...` | not identified | no credit covers one of these families alone; `Rh` (rendering) and `Ex` (draw sorting) are called from every other family, as shared engine libraries would be |
| The two sound-driver build accounts (`usr03333` for the Japanese release, `usr04623` for the US one) belong to the two credited sound programmers, one each | guess | only that the credits name two sound programmers and the builds use two accounts; `KCEJEAST.IRX` is Konami's in-house sound driver, which someone not credited on this game may have built |
| 417 functions have an original source file name (`config/ntsc-u/jp-source-files.csv`): 395 under `C:\dm2\Prog\etc\`, 14 under `C:\project\slotLIB\src\`, 8 under `Y:\Effect\SRC\Tasaki\` | verified | DWARF 1 compile units of the Japanese build |

## Not checked

- No other Japanese revision (v1.03) or PAL disc was available.
- `.mwcats` and `.line` were not decoded beyond their sizes.
- Of the 547 unmatched and 149 ambiguous `main` functions, those in the game code were placed
  by position where possible (above); none was examined by hand, and no fuzzy matching was
  attempted.
- Whether the debug-menu code (`SzDebug_*`), whose bodies exist in both
  builds, can be reached at run time was not determined.
- `IOPRP.IMG` contents were only string-scanned.
- The image MD5 was not compared with a public database.
