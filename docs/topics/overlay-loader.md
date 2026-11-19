# EE overlay loading and dispatch

How the main executable loads the overlay modules of `DATA/*MODULE.MRG` and calls into them.
Executable: NTSC-U `SLUS_205.15`, SHA-256
`1df7759b68e6aee73d8deab2efd2e556ec553d952af00fe8bf0f499a57298719`, on the supported USA disc.
Addresses belong to **EE main** unless described as the shared overlay address space. Everything
here is read from the original instructions and archive bytes; no overlay was run in an
emulator.

## The loader

`SzEffect_LoadModule` (`0x1f3910`, [note](../modules/effects.md)) selects archive letters C, B
or M for group indices 0, 1 or 2 (`0x1f3afc..0x1f3b3c`). Its format string at `0x2cfbb0`
constructs an archive path. It requests `0x8000` bytes at an offset
computed as a slot index shifted left 15 (`0x1f3bb0..0x1f3bf8`). Its 32-entry
cache uses eight-byte records at `0x31bb50`, with corresponding buffers starting
at `0x01ee0000`, spaced `0x8000` bytes (`0x1f3ad0..0x1f3aec`). The asynchronous
completion and the cache state machine around it, and the meaning of every record bit, are not
described here.

Two dispatch paths, in `SzEffect_Draw` (`0x1f40d0`) and `SzEffect_EntryCallback` (`0x1f4900`), copy a
cached slot to **`0x01e00000`** when the requested group/index differs from the
currently active group/index. The copy call sites are `0x1f4308` and `0x1f4ac4`.
Their third argument is `0x800`, but `RhMem_CopyMemory16` (`0x20b380`) counts **16-byte
units**: each iteration performs one `lq`/`sq` and advances both pointers by 16, so these calls
copy **32 KiB**, not 2 KiB. A currently active overlay can be called again
without repeating the copy and initialization.

After a cache-maintenance syscall, both paths read the header's word at `+0x1c`
and call `mwOverlayInit(base, word_1c - base)` (`0x17b410`). That function:

1. Reads the zero-fill byte count from `base + 0x14`.
2. Performs another cache-maintenance call.
3. If the count is nonzero, calls `memset(base + supplied_size, 0, count)` (`0x198948`, in the
   library block).
4. Passes words `+0x18` and `+0x1c` to `__initialize_cpp_rts` (`0x286910`).

At `0x286924..0x28694c`, `__initialize_cpp_rts` iterates the half-open pointer range
`[word_18, word_1c)` in four-byte steps, loads each pointer and calls it with
`jalr`. These words are therefore **initializer-list bounds**, not overlay entry points; the
Japanese name of the function points to C++ static constructors. Both bounds are equal in every
present overlay, so no initializer is called for any of them.

## Header fields, checked in all 762 slots

| Header offset | Observed meaning or relationship |
| --- | --- |
| `0x00` | `MWo3` signature |
| `0x04` | Correlates with the corresponding main-ELF load-header index |
| `0x08` | Shared load base, `0x01e00000` |
| `0x0c` | Size of first initialized partition; every dispatch target is inside it |
| `0x10` | Size of second initialized partition |
| `0x14` | Zero-fill byte count read by `mwOverlayInit` |
| `0x18` | Initializer-list start pointer read by `__initialize_cpp_rts` |
| `0x1c` | Initializer-list end pointer; also passed as the zero-fill start via subtraction |
| `0x20..0x3f` | Zero-terminated embedded filename |

For every present overlay:

```text
initialized_size = 0x40 + word_0c + word_10
word_1c          = base + initialized_size
zero_fill        = [word_1c, word_1c + word_14)
ELF memory size  = initialized_size + word_14
```

There are 30 nonempty zero-fill ranges: 29 are 128 bytes and one is 256 bytes.
The other 732 need no zero-fill. The largest initialized and memory sizes are both `0x7f80`
bytes, inside a `0x8000` slot. The zero-fill explains the 30 overlays whose `word_1c - base` is
smaller than their ELF memory size.

The first partition is code-bearing and the second commonly holds initialized
data. Which linker sections end up in `0x0c` and `0x10` is not proven, so the partitions keep
neutral names. The copy, initialization and dispatch path has no relocation table or relocation
loop; that does not rule out other loading paths.

## Logical indices, physical slots and entry points

The three records starting at `0x292ff0` point to zero-terminated dispatch tables.
`SzEffect_Init` (`0x1f3700`) computes their counts. Dispatch instructions
`0x1f437c..0x1f43a8` and `0x1f4b3c..0x1f4b64` load the selected table pointer,
then the logical-index entry, and call that address. The original table names
and the entry functions' signatures are unknown.

| Group | Archive | Dispatch table | Nonzero entries | Physical archive slots | Present MWo3 headers |
| --- | --- | --- | --- | --- | --- |
| 0 | CMODULE | `0x2b9190` | 1,000 | 1,000 | 78 |
| 1 | BMODULE | `0x2ba140` | 854 | 683 | 683 |
| 2 | MMODULE | `0x2baea0` | 1 | 683 | 1 |

CMODULE uses the logical index directly as the physical slot. The numeric part
of every present `cNNN.bin` name equals its archive offset divided by `0x8000`.
The distance from one `MWo3` signature to the next includes slots without a header; it is not
the size of the preceding overlay. For example `c000.bin` occupies a 32 KiB slot,
with only 256 initialized bytes, while the next signature is six slots later.

BMODULE and MMODULE pass the logical index through `SzDuel_Xax2New` (`0x1de130`), which reads
the 854-entry halfword table at `0x2cd8a0`. That table is a permutation of `0..853`.
Every present `bNNN.bin` independently maps to its observed physical slot through
this table. The function special-cases mapped value 671 to the sentinel 999, which
`SzEffect_LoadModule` rejects through its `<683` check. The present header at this
slot is `b737.bin`, so a request for it is refused. Why is not known; 671 is also treated
specially by card-record lookups ([GAME_DATA.md](../GAME_DATA.md#identifiers)).

For every present overlay, its logical index selects a dispatch
target inside `[base + 0x40, base + 0x40 + word_0c)`. This establishes concrete
entry addresses without treating the initializer-list words as entries. Examples:

| Module identity | Physical slot | ELF header index | EE dispatch target |
| --- | --- | --- | --- |
| `DATA/CMODULE.MRG`, `c006.bin` | 6 | 2 | `0x01e00b80` |
| `DATA/BMODULE.MRG`, `b000.bin` | 0 | 79 | `0x01e05670` |
| `DATA/MMODULE.MRG`, `m000.bin` | 0 | 833 | `0x01e00080` |

All overlays share one address space. The same address in different slots does not identify
the same function; overlay symbols need the archive and slot as part of their identity. The
32 KiB slot is the transfer unit; it says nothing about function or linker-section boundaries
inside it.

## Filename table

The exact archive string starts have raw little-endian pointers at `0x2932f0`,
`0x293310` and `0x293330`. They are part of a 97-record, `0x20`-stride filename
table starting at `0x293090`. Instructions `0x1fefb8..0x1ff040` compute each
record's address, inspect its filename, and advance 32 bytes, so the strings have no direct
references; this is `mapFile`, which `RhFile_Init` updates from the disc's directory
([GAME_DATA.md](../GAME_DATA.md#disc-files)). The path format string in `SzEffect_LoadModule` is
a separate route to the archives.

Every complete 32 KiB slot was checked against its load header and dispatch table. A test link with
the Metrowerks linker reproduced overlapping segments at the group address but no MWo3 header, so
the game's overlay packaging and linker script remain unknown.

## Overlays examined on their own

The initialized bytes of `c006.bin` and `b725.bin` were loaded in a disassembler as separate EE
programs at `0x01e00000`, with dispatch entries at `0x01e00b80` and `0x01e00080`, and the B725
program with its 128-byte zero-fill tail; both were checked against the original initialized
bytes, zero-fill region and entry. Both entries call back into the main executable: the C006
entry begins with a call to `ExGetMyOT` (`0x207500`) and calls its internal helper at
`0x01e00080` on one branch; the B725 entry also calls `ExGetMyOT`. This shows only that the
overlays can be read at their shared address, not what they do in the game.

## Open

- The asynchronous completion callback and cache state machine of the loader.
- Whether the game's linker command file or a post-link packager creates the `MWo3` header, and
  how it assigns the two initialized partitions.
- Rebuilding an individual slot and loading it through the normal path; no overlay source exists
  in this project (see [ROADMAP.md](../ROADMAP.md)).
