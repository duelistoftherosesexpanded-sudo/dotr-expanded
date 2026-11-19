# Licensing and provenance

The project's own work (C sources, headers, tools, tests and documents) is released under
**CC0 1.0** ([LICENSE](../LICENSE)). No licence is given or implied for the game itself: this
repository contains no game data, and you need your own disc to build or compare anything. This is
an unofficial fan project, not affiliated with, endorsed by or sponsored by Konami.

## Third-party material

| Material | Origin | Licence |
| --- | --- | --- |
| C for many game functions, and the warm-compile and original-file-unit techniques | [kira-502/dotr-decomp](https://github.com/kira-502/dotr-decomp); the functions are listed in [CREDITS.md](CREDITS.md) | CC0-1.0 |
| Data labels, structure and enum definitions, parameter names, comments; the name `useSoftFocus` | batzpup's Ghidra project for `SLUS_205.15` (2019) | used with the author's permission; names and readings only, no code |
| Original function and data names | the game's Japanese release (its own symbol table) | facts about the game, recovered by comparing code |
| Pinned build tools (Metrowerks compiler, wibo, binutils) | downloaded by `make tools` from the URLs in `config/tooling-lock.json` | their own licences; not redistributed here |

Nothing from [batzpup/DotrModdingToolRedux](https://github.com/batzpup/DotrModdingToolRedux)
(GPL-3.0) is copied. kira-502/dotr-decomp's C for the Sony library block (MIT/BSD per its
`THIRD_PARTY_NOTICES.md`) is not included.

## Keeping it clean

Disc images, extracted executables, archives, disassembly of the game's library code and any
generated comparison output stay in ignored `baserom/`, `out/` and `build/`. Ignore rules are a
convenience, not an audit: review the file list before publishing anything.
