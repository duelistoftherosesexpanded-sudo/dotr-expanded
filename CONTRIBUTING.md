# Contributing

Every game function already matches, so contributions now are mostly about **making the code
readable and the build more complete** without breaking a single byte:

- names for `data_<address>` objects and `unknown_<offset>` fields, backed by evidence (what the
  code does, the game's strings, the Japanese debug data);
- shared structures in place of per-group "views" of the same storage;
- better descriptions in [docs/modules/](docs/modules/README.md) and [docs/GAME_DATA.md](docs/GAME_DATA.md);
- the linked build and typed data described in [docs/ROADMAP.md](docs/ROADMAP.md).

## Rules

- Read [docs/DECOMPILATION.md](docs/DECOMPILATION.md). A change to a matching source keeps its
  status only if `make match` still reports it exact. Renames normally do not change the code;
  the four original-file units (`SzDuel_Act` and three others) are sensitive to any change in the
  text before them, so check them after every edit there.
- Run `make test` and `make host-check` for every change, `make match` (and ideally
  `make rebuild`) for any change to C, headers or build configuration.
- Never commit the game: no disc image, executable, extracted files, disassembly of game data,
  or anything from `out/`, `build/` or `baserom/`. Tests use synthetic inputs only.
- Distinguish what you checked from what you infer. A name or field meaning taken from another
  project is a reported claim until checked; say where it came from (repository and commit).
  Code taken from elsewhere needs a compatible licence and its exact revision.
- Four spaces in C and Python, two in JSON; `.clang-format` for new C. Constructs needed only to
  make the compiler match stay as they are and are marked `MATCHING DEVICE` where surprising.

A merge request says what changed, how it was checked, and what remains uncertain.
