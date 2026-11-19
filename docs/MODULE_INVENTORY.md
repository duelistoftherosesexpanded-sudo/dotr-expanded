# Disc modules and overlay archives

`make disc-inventory` re-hashes the image, walks its ISO9660 directories and writes the
metadata to `out/research/ntsc-u/disc-inventory.json`. It finds **97 files**.
Only IOP modules are extracted; the original image is opened read-only.

## IOP modules

All seven files are little-endian MIPS ELF32 with type `0xff80`. Addresses in
these files are **IOP module-relative**, not EE addresses or final relocated
runtime addresses. ELF entry points and `.iopmod` names are retained below;
version fields are shown raw rather than assigning an unverified display format.

| File in `IOP/` | Bytes | Entry | Embedded module name | Version field |
| --- | --- | --- | --- | --- |
| `KCEJEAST.IRX` | 433,459 | `0x0` | `KCEJ_EAST_IOP_Driver1.5` | `0x0105` |
| `LIBSD.IRX` | 25,397 | `0xa0` | `Sound_Device_Library` | `0x0301` |
| `MCMAN.IRX` | 90,805 | `0x168` | `mcman` | `0x0220` |
| `MCSERV.IRX` | 7,353 | `0x40` | `mcserv` | `0x020f` |
| `PADMAN.IRX` | 44,917 | `0x3224` | `padman` | `0x0403` |
| `SDRDRV.IRX` | 6,977 | `0x0` | `sdr_driver` | `0x0401` |
| `SIO2MAN.IRX` | 6,553 | `0x634` | `sio2man` | `0x0205` |

The inventory records each file's complete SHA-256, ISO extent, sections
and load headers. Extracted files and raw ELF symbol records are under
`out/research/ntsc-u/iop/IOP/`. The IOP modules are not decompiled.

KCEJEAST, Konami's sound driver, retains a 310,696-byte `.mdebug` section and 398 ELF symbol
records (188 functions, 53 source files and 164 types by one parser's count); the other six
modules retain no symbols. The IOP is a separate processor with its own ABI and toolchain.

## EE overlay archives

`make disc-inventory` looks for the `MWo3` signature at 2048-byte-aligned positions and reports
each one as an overlay candidate, extending to the next signature or the archive end. That extent
includes empty slots and padding; it is **not** the overlay's size (see below).

| Archive in `DATA/` | Bytes | ISO LBA | Candidates | Header identifiers |
| --- | --- | --- | --- | --- |
| `BMODULE.MRG` | 22,380,544 | 33,216 | 683 | 79 through 832, with gaps |
| `CMODULE.MRG` | 32,768,000 | 17,216 | 78 | 1 through 78 |
| `MMODULE.MRG` | 22,380,544 | 44,144 | 1 | 833 |

All 762 headers have distinct identifiers and distinct embedded names
within each archive. The first 64 bytes contain the magic, seven little-endian
words and a zero-terminated name field at offset `0x20`. Examples are `b000.bin`,
`c000.bin` and `m000.bin`. Archive order is not identifier or filename order.

For **every header**, the word at offset `0x04` selects a main-ELF program header with the same
virtual address as the word at `0x08`, and that header's memory size equals
`0x40 + word(0x0c) + word(0x10) + word(0x14)`. All headers report base `0x01e00000`. This ties the
archives to the ELF's otherwise file-empty headers. The loader's instructions, checked against all
762 headers, show **32 KiB physical slots**, two initialized partitions, a zero-fill size,
initializer-list bounds and entry addresses selected from separate tables.
[topics/overlay-loader.md](topics/overlay-loader.md) has the instructions, mappings, exceptions and
limits.

There are 833 ELF headers at that base. Exactly **71 identifiers** have no overlay in the
archives; each of their ELF memory sizes is `0x100`, and their purpose is unknown. Another
zero-sized header at `0x01fe0000` accounts for the remaining program header.

Words at `0x18` and `0x1c` are the start/end of an initializer pointer list, as the loop that calls
them shows. They are equal in all present overlays, so no initializer function is called for any of
them. Word `0x1c` also equals `base + 0x40 + word(0x0c) + word(0x10)`, the zero-fill start. Thirty
slots have nonempty zero-fill (29 of 128 bytes, one of 256); this explains their larger ELF memory
sizes. Dispatch entry points come from separate tables. MMODULE contains 683 physical slots with one
present MWo3 header; its 22 MB are not one executable payload.

The signature and count convention was first reported by DOTR-Rebuilt; the counts and header
correlations here were checked independently. The loader maps every present named overlay to its
physical slot and dispatch entry; one mapping, physical slot 671 / `b737.bin`, is turned into the
sentinel 999 by `SzDuel_Xax2New` and refused by `SzEffect_LoadModule`, for an unknown reason. The
archive paths sit in a 97-record filename table in the main executable (the same table `RhFile_Init`
fills from the disc's directory). The overlay code itself is not decompiled; see
[ROADMAP.md](ROADMAP.md).
