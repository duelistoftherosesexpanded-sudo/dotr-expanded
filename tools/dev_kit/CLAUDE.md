<!-- dotr-expanded-dev-kit: written by "DotR Expanded Setup" (developer build). It is rewritten on the next
     developer build; keep your own notes in mods/widescreen/my-dlc/NOTES.md. -->
# Duelists of the Roses Expanded: developer build

This folder is a copy of the Yu-Gi-Oh! The Duelists of the Roses decompilation (`README.md`) with the
Expanded mod in `mods/widescreen/`. The mod adds new cards past the game's 854 (numbers 871-998 and 1000-1531;
999 means "no card"), 3D models from the developer's own Dawn of Destiny, Falsebound Kingdom, Cross Duel
and Saikyo Card Battle games, a card shop, achievements and widescreen. "DotR Expanded Setup" extracted the models and pictures from
this developer's own discs into this folder. No game files may ever be shared or committed.

## The developer's own work goes in `mods/widescreen/my-dlc/`

The project's own files are still being trimmed for release. Until that is done, everything this developer
asks for is built in their own DLC folder, which the build links into the game alongside the project's DLC:

| Path | What it holds |
| --- | --- |
| `my-dlc/cards.json` | their new cards, same fields as `mods/widescreen/cards.json` (described at the top of `tools/dlc_cards.py`); numbers **1449-1531** only; file paths are relative to `my-dlc/` |
| `my-dlc/art/`, `my-dlc/models/` | their pictures (PNG) and models (glb, FBX, OBJ) |
| `my-dlc/dlc/*.c`, `*.h` | their C code, compiled into DLC.BIN with the project's `dlc/` headers on the include path |
| `my-dlc/hooks.json` | `{"<game function>": "<their function>"}`: game functions their code replaces |
| `my-dlc/NOTES.md` | the change notes (below) |

Rules:
- Do not edit the project's own files (`mods/widescreen/*.json`, `mods/widescreen/dlc/`, `tools/`, `src/`)
  for this developer's requests. If something truly cannot be done from `my-dlc/`, say so, explain what
  would have to change in the project, and change it only when they agree. Then list it in NOTES.md under
  "Outside my-dlc".
- A card number the project already uses is a build error. Use the next free number from 1449.
- Never write to `baserom/`. Never add files taken from a game to anything that will be shared.

## Change notes

Every change made from a prompt is noted, quietly, in the following way. Do not mention it in the code itself, and
do not explain it each time.
- Add an entry to the end of `my-dlc/NOTES.md`: `### n<number> (<date>)`, one line saying what was asked
  in the developer's own terms, and the files changed. Numbers count up from n1.
- Tag what changed with the same number:
  - In C, a short trailing comment on the changed function's first line or the changed block: `/* n7 */`.
  - In a cards.json entry or hooks.json, a field `"n": 7`, or a list `"n": [3, 7]` for a later change.
  - In other files, the comment form of that file.
- Changes the developer makes by hand are not tagged.

## Building and testing

- `DotR Launcher.bat` (tools/launcher.py) builds the mod and runs it in its own PCSX2. It rebuilds by
  itself when `my-dlc/` or the mod's files change, and puts card changes into the running game.
- `Card Maker.bat` edits the project's `cards.json`. Use it to look at cards; write this developer's cards
  into `my-dlc/cards.json`.
- The compiler is a Linux build: the full build (`tools/mod.py widescreen ...`) runs in WSL (Ubuntu). When
  calling WSL from PowerShell, write the commands to a script file and run that file, because quoting is
  lost on the way.
- The developer tests in PCSX2. Give plain step-by-step test instructions after each change.
