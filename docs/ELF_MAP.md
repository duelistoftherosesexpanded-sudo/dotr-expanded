# Main executable inventory

The NTSC-U executable, SHA-256
`1df7759b68e6aee73d8deab2efd2e556ec553d952af00fe8bf0f499a57298719`.
`make prepare` then `make inventory` reproduce these observations and write the full metadata to
`out/research/ntsc-u/inventory.json`.

| Address space | Half-open range | Meaning |
| --- | --- | --- |
| Main ELF file | `0x6900..0x216800` | One file-backed load segment |
| EE virtual addresses | `0x100000..0x30ff00` | Initialized bytes in that segment |
| EE virtual addresses | `0x30ff00..0x351300` | Zero-filled segment tail; no corresponding file bytes |
| EE virtual address | `0x100008` | ELF entry point |
| EE virtual address | `0x01e00000` | Address shared by 833 sections/load headers without file bytes |
| EE virtual address | `0x01fe0000` | One additional zero-sized section/load header |

For a range wholly inside the file-backed segment:
`ELF offset = EE address - 0x100000 + 0x6900`.
The executable begins at ISO offset `0xc9800` (LBA 403), so its ISO offset is
`0xc9800 + ELF offset`. `tools/research.py range --address ... --size ...`
checks segment containment and rejects zero-fill or ambiguous ranges.

The ELF contains 835 program headers and 841 section headers. Section 4 is a
single unnamed, writable/executable PROGBITS section holding the entire initialized
segment. Separate `.text`, `.data` and `.bss` boundaries do not survive. Sections
5 through 837 are empty unnamed PROGBITS sections at `0x01e00000`. Their 833
corresponding load headers have nonzero memory sizes despite having no file bytes.
Section 838 and program header 834 are zero-sized at `0x01fe0000`. Their count
alone does not identify archive entries or overlay boundaries. See
[module inventory](MODULE_INVENTORY.md) for header correlations.

`.symtab` and `.strtab` exist but are empty. There is no `.mdebug` section.
The US executable is stripped; the names in this project come from the Japanese release
([topics/jp-build-symbols.md](topics/jp-build-symbols.md)).
`.reginfo` contains a GP value of `0x317ef0`. ELF flags are `0x20924000`;
the generic binutils machine label does not establish an IOP architecture.

`.comment` contains `MW MIPS C Compiler (2.3.1.01)` and `PlayStation2`.
This is a Metrowerks fingerprint in the linked image. The comment names 2.3.1; the release that
reproduces the code is 2.3.3 (build 000906), established by comparing compiled output with the
original. A retained comment can come from one object rather than every object.

## Code layout

| EE range | Contents |
| --- | --- |
| `0x100000-0x1000d0` | startup code (Sony `crt0`) |
| `0x1000d0-0x17b48c` | game code, Metrowerks |
| `0x17b48c-0x1a6c80` | Sony SDK and C library, GCC |
| `0x1a6c80-0x286964` | game code, Metrowerks |
| `0x286964-0x290000` | VU1 microcode (uploaded to the vector unit; data) |
| `0x290000-0x30ff00` | initialised data, including read-only tables |
| `0x30ff00-0x351300` | zero-initialised data |

Every gap between registered functions in the two game-code ranges is zero padding; the
functions themselves are in the registry and described in [modules/](modules/README.md).
Function boundaries were reviewed by hand where automatic analysis disagreed: a function's
physical range runs through the delay slot of its final return, and some functions contain
words automatic analysis left out of their bodies.
