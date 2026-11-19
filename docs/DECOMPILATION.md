# Matching rules

How a function counts as decompiled here, what a source may contain, and how the build checks it.

## What "matching" means

A function is **matching** when its C (or, in the narrow cases below, its assembly) compiles with
the pinned compiler and flags into exactly the original's bytes over the function's complete
physical range: every instruction through the final return's delay slot, plus any compiler-
generated read-only data placed with it (jump tables). `make match` checks this for every entry
in `config/ntsc-u/function-builds.json` against your own `SLUS_205.15`; nothing else counts.
Host tests, behaviour comparisons and similar-looking output are useful evidence, never a match.

Every function of the game code is matching (see [ROADMAP.md](ROADMAP.md)). The rules below
apply to changes: a changed source keeps its status only if `make match` still reports it exact.

## Compiler and flags

Metrowerks CodeWarrior for PS2, `mwccps2` 2.3.3 (build 000906), run through `wibo`, with
`-O4,p -sdatathreshold 0 -relax_pointers`. The original command line is not known; these are the
closest evidenced settings:

- `-sdatathreshold 0`: the game code never addresses data through `gp` (one `gp`-based access
  in about 1.4 MB), which the compiler would do for small globals otherwise.
- `-relax_pointers`: relaxes pointer type checking only; every function compares exact with it.

Declare a global with the type the code shows; do not inflate small objects into arrays or
structures to steer addressing (a few sources still do; replacing them is open work).

## The registry

`config/ntsc-u/symbols.csv` has one row per symbol:

| Field | Meaning |
| --- | --- |
| `module` | `ee-main` for the main executable |
| `address`, `size` | `0x` address in the EE address space; size in bytes (physical range) |
| `name` | the original name from the Japanese build where it has one, else `func_<address>` / `data_<address>` |
| `kind`, `status` | `function` or `data`; `matching`, `nonmatching`, `in-progress` or `identified` |
| `source` | the source file (`src/ee/<group>/func_<address>.c`, or `asm/ee/...` for assembly) |
| `evidence` | the document describing it (`docs/modules/<module>.md`) |
| `match_kind` | `c` or `asm` for functions, `none` for data |

`make check` validates rows, ranges, uniqueness and paths; it does not compile anything.
`python3 tools/project.py progress` counts matching functions and bytes, C and assembly apart.

## Build entries

Each matching function has an entry in `config/ntsc-u/function-builds.json`: its source,
headers, the addresses of every external symbol it links against (`globals`), and optionally:

- `read_only_data`: whole compiler-generated read-only sections (switch tables) placed at their
  original address; every byte is compared, and a table difference makes the function fail.
- `warm: true`: the source is compiled in one compiler run right after a fixed warm-up file
  (`double dotr_warm(double a, double b) { return a * b; }`). The original was built many files
  per run, and once the compiler has emitted a call to a double-arithmetic helper with arguments
  in `a0`/`a1`, later float-to-integer casts are register-allocated differently for the rest of
  the run. Warm functions write float-to-integer conversions as casts.
- `placements` (with `unit_sources` when the source includes others): a comparison unit with
  several functions, each linked at its original address; code without a placement is refused.
  Only the entry's own function is credited by its entry.

### Original-file units

Compiler state also carries from one function to the next inside a source file. Four functions
compare exact only when compiled after the functions that preceded them in the original file:
`SzDuel_Act` (23 predecessors), `SzDuel_SetupCard_Pipe` (28), `lSzBattle_DrawFakeShadow` (17)
and `GT4_BOX_IN__2DMAP` (44). Their sources hold the predecessors' C first, inside
`#ifdef __MWERKS__` (so host builds compile only the function itself), then the function; the
predecessors are placed at their addresses. Predecessors in a separate file of the same compiler
run are not enough, and subsets give chaotic results: the whole prefix is needed.

## What a source may contain

C, written so that the compiler produces the original. In addition:

- **Per-function compiler pragmas** that only select code generation (`optimization_level`,
  `conditional_move`, `opt_propagation`, `opt_strength_reduction`, `opt_lifetimes`,
  `opt_dead_assignments`, `global_optimizer`, `schedule`), set directly before one function and
  reset directly after it, when the original code shows that choice (for example the `ShSlot_*`
  slot-machine code is evidently unoptimised). Two comparison units
  (`src/ee/resource_submit/unit_wait.c`, `src/ee/resource_process/unit_poll.c`) instead set
  `auto_inline on` for the whole unit, because their functions contain a helper expanded inline.
- **Inline assembly blocks** only for instructions with no C spelling in this compiler (VU0
  macro mode/COP2, MMI, COP0, `sync`/`ei`) or operations C cannot express (switching the stack
  pointer to the scratchpad and back). The rest of the function stays C. Ordinary integer or
  float code is never written as assembly. Host builds replace the block with plain C or skip it.
- **Whole functions in assembly** (`match_kind: asm`, `asm/ee/`) only when the original is
  evidently handwritten or architecture-specific: the 13 VU0 matrix routines. They are counted
  separately from C.
- Constructs whose only purpose is to make the compiler match (a variable order, a temporary,
  a form of loop) are allowed and marked `MATCHING DEVICE` where they would surprise a reader.

## Scope inside the executable

| Range | Contents | Status |
| --- | --- | --- |
| `0x100000-0x1000d0` | startup code (Sony `crt0`) | out of scope; assembly in builds |
| `0x1000d0-0x17b48c` | game code | all matching |
| `0x17b48c-0x1a6c80` | Sony SDK and C library, built with GCC | out of scope; assembly in builds |
| `0x1a6c80-0x286964` | game code | all matching |
| `0x286964-0x290000` | VU1 microcode (data) | data |

The library block's end at `0x1a6c80` comes from the Japanese build's symbols, where the C
library and further Sony objects run up to the first game function, `SzGame_Init`. Whole-
executable builds take the startup code and the library block as plain assembly generated from
your executable at build time (`tools/library_asm.py`, into the ignored `build/`, never tracked);
the few words the assembler cannot reproduce stay `.word`.

## Names

The Japanese release's executable keeps its symbol table and some debug information. Functions
carry the original name wherever a Japanese function with the same body exists
(`config/ntsc-u/jp-names.csv`; method and limits in
[topics/jp-build-symbols.md](topics/jp-build-symbols.md)). Parameter, local and structure names
from the Japanese debug data describe the Japanese function and are hypotheses here. Some data
objects carry Japanese names (`config/ntsc-u/jp-data-names.csv`). One function name,
`useSoftFocus`, comes from a modder's hand-labelled Ghidra project, used with permission
([REFERENCES.md](REFERENCES.md)).

## Shared storage

Different functions often view the same storage with different C types. Where the views are
incompatible they use distinct symbols (`data_<address>_<group>`) that link to the same address.
A view describes the fields a function touches, not the original object.

## Tests

Every C source in the registry has a host test under `tests/` (`make host-check` compiles the C
on your machine and runs it): callees are stubbed, data symbols are fixtures, and the test checks
the function's observable effects. Some functions have only a smoke test (one ordinary path);
the byte comparison is their proof.
A host test checks C semantics on a 64-bit machine, not PS2 behaviour. The 13 assembly routines
are not built on the host; their callers' tests stub them.

Test comments name the coverage they aim for: **straight-line** (one call per path through code
without branches), **branch** (each outcome of every condition taken at least once) and **loop**
(each loop run zero times, once and up to its bounds, where it has them); "branch and loop" is both.

## Non-matching functions

None remain. If a change cannot be made to match, register the function `nonmatching` without a
build entry; its note says how far it is from the original. `make rebuild` fills the game code
only from build entries, so the whole executable is then no longer byte-identical.
