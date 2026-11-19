# Setup and commands

## Requirements

- Linux x86-64 with glibc 2.35 or newer (the pinned tools are Linux builds), Python 3.10+
  (standard library only), `dpkg-deb`, and GNU Make for the short commands.
- CMake 3.16+ and a C11 compiler for `make host-check`.
- Your own clean USA disc image for anything that compares bytes.

`make test` and `make host-check` need no disc and no PS2 tools. The decompiled C keeps the original's
quirks: 32-bit words for addresses (a 64-bit host compiler warns at every conversion between a word
and a pointer), locals that exist only to reproduce the original code, and the like. `make
host-check` mutes the warnings that only flag these; `make host-check WARNINGS=1` shows them (about a
hundred with GCC).

## Tools

`make tools` downloads three pinned tools into `.tooling/`, checks each against its SHA-256 in
`config/tooling-lock.json` and installs it there; nothing is installed system-wide.
`make tools-check` reports whether they are present. Details: [TOOLING.md](TOOLING.md).

## Your disc

Put exactly one `.iso` directly in `baserom/`, or set `DOTR_ISO=/path/to/image.iso` (when you
run `tools/project.py` or `tools/research.py` directly, `--iso PATH` also works). The supported
image is 2,112,782,336 bytes with MD5 `ca838dae26ff750936c0814d1c2d33f1`; other revisions need
their own profile and address map. Do not edit the hash to make an image pass.

- `make verify` checks size and MD5 and prints a SHA-256.
- `make prepare` verifies the image again and writes to `out/ntsc-u/`: `SLUS_205.15` (the
  executable), `SYSTEM.CNF`, `disc.json` and `elf.json` (fingerprints, program headers).
  The image itself is only read. Its ISO9660 reader handles cooked 2,048-byte-sector images.

## Comparing

- `make match` compiles every function in `config/ntsc-u/function-builds.json` and compares it
  with your executable; per-function reports go to `out/research/ntsc-u/matches/<name>/`. It
  exits non-zero if any function differs.
- `make rebuild` runs `make match`, then assembles the whole executable from the rebuilt
  functions, their tables and generated assembly for the library block and startup code, and
  compares it with the original (`build/ntsc-u/rebuilt/`, report in `report.json`). The data,
  headers and VU1 microcode are still taken from your executable: this checks the code at fixed
  addresses, not a linked build.
- `make progress` prints matching function and byte counts, C and assembly apart.
- `make inventory` and `make disc-inventory` write executable and disc metadata (sections,
  IOP modules, overlay archives) to `out/research/ntsc-u/`.

## Troubleshooting

| Problem | What to do |
| --- | --- |
| No image found, or several | Set `DOTR_ISO`, or pass `--iso PATH` to the Python tool |
| Size or hash differs | The dump is not clean or is another revision |
| `make match` cannot find a tool | Run `make tools`, then `make tools-check` |
| Many warnings with `make host-check WARNINGS=1` | Expected (see above); the build passes if every test passes |
| Host compiler errors | The C targets a 32-bit PS2; host builds rely on the guards in the headers. Report the compiler and version |

Never attach your disc image or anything from `out/` to an issue.
