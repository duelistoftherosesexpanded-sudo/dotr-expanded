# Disc and executable identity

What `make prepare` checks and reports about the supported disc.

| Observation | Result | Evidence level |
| --- | --- | --- |
| Image size | 2,112,782,336 bytes | Verified locally |
| Image MD5 | `ca838dae26ff750936c0814d1c2d33f1` | Verified locally |
| Image SHA-256 | `a33e3d59691505fd9ced5bdf01fc9a3ed8579df7c8145e19de4d744eaaed23e0` | Verified locally |
| SYSTEM.CNF boot target | `cdrom0:\SLUS_205.15;1` | Verified locally |
| ELF format | Little-endian ELF32, executable, MIPS machine ID 8 | Verified locally |
| Entry | `0x100008` | Verified locally |
| Program-header count | 835 | Verified locally |
| File-backed PT_LOAD segments | One | Verified locally |
| Main PT_LOAD | File offset `0x6900`, vaddr `0x100000`, file size `0x20FF00`, memory size `0x251300` | Verified locally |
| Executable SHA-256 | `1df7759b68e6aee73d8deab2efd2e556ec553d952af00fe8bf0f499a57298719` | Verified locally |
| Other revision | Another `SLUS_205.15` exists (sha256 `3b9b73bbfd921e3436203fe3631ec42d1f58a9a62e0f1311905cf7b16a06b075`, sha1 `077ac97a878d1bc3b7eb1c3e09bc7fef7becdf68`), reported elsewhere (see [CREDITS.md](CREDITS.md)): there the tutorial hooks are patched in after the build (jumps to `0x27f890` etc. at `0x166ee0`, `0x16c8f0`, `0x1aaaf0`, `0x14fd40`, `0x2514f0`; stubs at `0x27f5b0`-`0x27fda0`) and some constants differ (`Ta_DelUnit` argument 0 there, -4 here); in this executable all of those are ordinary compiled functions | Reported, not verified here (no copy) |

Reproduce with `make prepare`, then look at `out/ntsc-u/disc.json` and `elf.json`. The
[ELF map](ELF_MAP.md) describes the executable's sections and layout, the
[module inventory](MODULE_INVENTORY.md) the disc's files, overlay archives and IOP modules, and
[topics/overlay-loader.md](topics/overlay-loader.md) how overlays are loaded and dispatched.
