# Duelists of the Roses Expanded

**Duelists of the Roses Expanded** (v0.55 beta) is a fan mod of *Yu-Gi-Oh! The Duelists of the
Roses* (PS2, USA) for PCSX2. 
> ⚠️ **v0.55 is a beta and a proof of concept.** The new cards have **no balancing** yet: their
> stats, effects, costs and rarities are placeholders. The mod isn't meant to be played through
> yet, and saves from this beta may not carry over to later versions.

**Pick your era of nostalgia.** Whether you grew up with Battle City, ZEXAL or today's game, you
can play Duelists of the Roses with exactly the cards you remember. Choose your era in MOD
OPTIONS: OG keeps the classic Duel Monsters card pool, and every era after it adds its cards on
top, through GX, 5D's, ZEXAL, ARC-V and VRAINS up to Modern. Everyone gets the game they want.

## What it adds

### 576 new cards with 3D models

- New cards are numbered past the game's old limit (up to 1448). Each one has its own card art, a
  password, and a 3D monster model in duels. The models come from other Yu-Gi-Oh! games:

  | Model source | Cards |
  | --- | --- |
  | Saikyo Card Battle (3DS) | 248 |
  | Cross Duel (mobile) | 156 |
  | The Dawn of Destiny (Xbox) | 68 |
  | The Falsebound Kingdom (GameCube) | 3 |
  | "Picture frame" models showing the card's art, for cards no game has a model for | 100 |

- New cards can have effects of their own, and a monster's effect slot can hold several effects at
  once (**stacked effects**).
- Card pictures load as they're needed, so the deck builder can show every card, old and new.

### Card eras

Every card belongs to one of seven eras by its first TCG release: **OG** (before 2004, always on),
**GX** (2004-08), **5D's** (2008-11), **ZEXAL** (2011-14), **ARC-V** (2014-17), **VRAINS** (2017-20)
and **Modern** (2020 on). Pick the newest era you want in **MOD OPTIONS** on the title screen; every
era before it comes with it. Cards from later eras can't be obtained and stay out of your chest.

### Native widescreen

- The game runs in true **16:9**. The 3D view is drawn wider instead of a 4:3 picture being
  stretched.
- The whole 2D interface (menus, panels, the hand, the map's windows) is redrawn at its proper width,
  so nothing looks stretched. Each piece stays pinned to its screen edge or to the centre.
- The highlight around cards in the hand and on the field lines up with the wider 3D view.

### Card shop

- A **Card Shop** stands on the campaign map. Pick its podium to open it.
- The shop shows **25 cards drawn by rarity**: Common, Rare, Super Rare, Ultra Rare, Secret Rare
  and Ultra Secret. The stock **restocks after every campaign duel**.
- Pick a card to buy it; it goes straight to your chest. Cards are free for now.
- The shop is also where you choose your **card sleeve**, the back of your face-down cards.
- It has its own shop keeper and background.

### Second campaign: a whole new world

- A **gate** on the campaign map leads to a **second world**: a new map with green land and blue
  sea, with its own podiums in place of the game's locations.
- **20 new duels** against the story's duelists, from Yugi and Seto to Pegasus and Manawyddan fab
  Llyr. Press SQUARE at the gate to travel between the two worlds.
- **Every boss has a pool of new decks** and draws one at random, a new one after every duel.
- Your world is saved with the game.

### Deck Masters on the map

- Four **Deck Masters** (T, K, I and S) stand on new podiums on the original campaign map, from the
  start of the campaign, on both the Lancaster and the York side.
- Beating them counts toward the NEW CHALLENGER and CONQUEROR achievements.

### Fields made for the deck

In duels against the new bosses and the Deck Masters, the 7x7 board is **generated at the start of
the duel from the opponent's deck**: patches of the terrains its monsters like best, different every
duel. The leaders' squares are never crush terrain, and there is no labyrinth.

### Achievements and rewards

- **12 achievements**, from FIRST VICTORY to CARD MASTER: win duels, buy cards in the shop, beat the
  new podiums and grow your collection.
- Each one gives a reward: a card or a new card sleeve. An "ACHIEVEMENT UNLOCKED" pop-up shows on
  the map and in the shop, and a trophy icon on the map.

### Card frames and card sleeves

- Monster cards can have **frames in new colours**, set per card in the Card Maker. Any monster can
  also use the normal (yellow) or effect (orange) frame, whatever its type.
- The frame colour shows everywhere the card does: on the field, in the hand, in the chest and on
  the card detail panel.
- **New card sleeves** change the back of your face-down cards. Some are achievement rewards.

### Card quality of life

- **Three copies of a card at most.** Deck tabs share one collection: change tabs and every card is
  back in the chest, so three copies cover all your decks. Older saves are trimmed to three copies
  when they load.
- **Any monster can lead a deck**, the new cards included. Experience is never taken away, so a
  leader never loses a rank it has reached.
- **Simple passwords.** Every card's password is its card number with eight digits (Blue-Eyes White
  Dragon is 00000000, card 132 is 00000132), so any card can be typed in at the deck builder.
- New cards sort by name with the game's own cards in the card lists.

### Options and convenience

- **AI SPEED** in the duel settings: FAST plays the computer's turn at double speed.
- **Xbox button pictures** as an option in MOD OPTIONS, with the A, B, X and Y buttons.
- **SPEX built in.** The game always plays with the DOTR SPEX community patch.
## Tools

- **DotR Expanded Builder** (`DotR Expanded Setup.exe` from the
  [Releases](../../releases) page, or `tools/setup_tool.py`): drag your games onto it and it builds
  a playable disc, set up to play in your PCSX2 with the mod's graphics settings. It can also make a
  developer copy of the project.
- **Card Maker** (`Card Maker.bat`, `tools/card_maker.py`): add or change cards. Set the art, model,
  frame colour and era, and choose whether a test build hands out every card.
- **Launcher** (`DotR Launcher.bat`): runs the mod in PCSX2 and rebuilds when files change.

The mod's code is in [mods/widescreen/](mods/widescreen/) (the C in `dlc/` is loaded by the game as
DLC.BIN) and its tools are in [tools/](tools/).

## The games you need

This repository and its releases have no disc images, models, textures, sounds or card art. The
**DotR Expanded Builder** (`DotR Expanded Setup.exe`) makes everything on your computer from your
own, legally acquired copies of the games:

| Game | Needed? | What it gives |
| --- | --- | --- |
| **Yu-Gi-Oh! The Duelists of the Roses** (PlayStation 2, USA, SLUS-20515) | **Required** | the game itself |
| Yu-Gi-Oh! Duel Monsters: Saikyo Card Battle! (Nintendo 3DS, Japan) | optional | 248 monster models, and card pictures |
| Yu-Gi-Oh! Cross Duel (Android, the .apk) | optional | 156 monster models, and card pictures |
| Yu-Gi-Oh! The Dawn of Destiny (Xbox, USA) | optional | 68 monster models |
| Yu-Gi-Oh! The Falsebound Kingdom (GameCube, USA) | optional | 3 monster models |

**Only Duelists of the Roses is required.** The other four add models. Without one of them, the
cards whose models come from that game can't be collected: they stay out of your chest, and the
computer's decks play them with a stand-in model. Everything else in the mod works. You can run the
builder again later with more games to unlock those cards.

The builder takes the files as you have them: disc images (.iso; Falsebound also as .rvz), the
Cross Duel .apk, and Saikyo Card Battle as a decrypted .cci/.3ds or its eShop .cia. Card art comes
from the games given, or is downloaded from the YGOPRODeck card database.

## Model tools

Expanded comes with tools that read the monster models of other Yu-Gi-Oh! games and turn them
into standard **glTF (.glb)** files with their textures, a preview picture of each and a gallery
page:

| Game | Tool | Reads |
| --- | --- | --- |
| The Dawn of Destiny | `tools/dod_export.py`, `tools/models_dump.py --dod` | the Xbox disc's IGB models |
| The Falsebound Kingdom | `tools/fbk_models.py`, `tools/models_dump.py --falsebound` | the GameCube disc's HSD models (MONSTER.MRG) |
| Cross Duel | `tools/crossduel_export.py`, `tools/cross_dump.py` | the app's Unity bundles, with animations |
| Saikyo Card Battle | `tools/bch_export.py`, `tools/saikyo_dump.py` | the 3DS game's H3D (BCH) models |

The same code puts the models into Duelists of the Roses, shrunk to fit the game's model slots.
As with the mod itself, you need your own copies of the games.

## Credits

- **zusk**, . Expanded
  could not exist without it: every new feature is written against its C code and names.
- **Starlight Phoenix Project**, for [DOTR SPEX](mods/widescreen/community/spex/LICENSE.txt). The
  game Expanded plays is always SPEX 1.0, and the SPEX extra patches are offered too (CC BY 4.0).
- **batzpup**, for [DotrModdingToolRedux](https://github.com/batzpup/DotrModdingToolRedux), whose
  card and image layouts guided our card editing tools (no code copied).
- The wider DotR community  draws on, listed in
  [docs/REFERENCES.md](docs/REFERENCES.md): ,
  GenericMadScientist's DotR documentation, batzpup's Ghidra project and DOTR-Rebuilt.
- Card art comes from [YGOPRODeck](https://ygoprodeck.com/)'s card database, downloaded by the
  setup tool.
- **Zacksly**, for the "Xbox Series Button Icons" (CC BY) used for the Xbox button pictures.

### Open-source software

Expanded is built, tested and packaged with these projects. Thank you to everyone behind them.

- [**PCSX2**](https://pcsx2.net/), the PlayStation 2 emulator Expanded is made for and tested in
  (GPL-3.0). The launcher talks to it over its PINE interface for live edits.
- [**Dolphin**](https://dolphin-emu.org/)'s DolphinTool, which the builder uses to read a
  Falsebound Kingdom .rvz image (GPL-2.0-or-later).
- [**Python**](https://www.python.org/) and Tkinter, which every tool and the builder are written
  in.
- [**PyInstaller**](https://pyinstaller.org/), which packs the builder into one .exe.
- [**Pillow**](https://python-pillow.org/), for every picture and texture.
- [**UnityPy**](https://github.com/K0lb3/UnityPy), with texture2ddecoder, etcpak, astc-encoder and
  fsb5, for reading Cross Duel's Unity bundles.
- **pyfmodex** and fmod_toolkit, for Cross Duel's sound banks.
- **Brotli** and archspec, used by those libraries.


Each project keeps its own licence. None of their code is copied into this repository; the
builder .exe carries Python and the Python libraries above, unchanged, under their licences.

This is an unofficial fan project. It is not affiliated with, endorsed by or sponsored by Konami.
Yu-Gi-Oh! and every game named here are trademarks of their respective owners.

---

# Duelists of the Roses 


Every function of the game's own code in the main executable is written in C that the original
compiler turns into exactly the original bytes: **2,568 functions, 1,405,804 bytes** (2,555 in C,
13 VU0 routines in assembly). Built together with the Sony library and startup code (as assembly
generated from your own executable at build time, not stored here), the whole executable comes
out byte-identical to the original.

Function names are the developers' own, recovered from the Japanese release, which shipped with
its symbol table. What the code does is described module by module in
[docs/modules/](docs/modules/README.md); the game's tables and save layout in
[docs/GAME_DATA.md](docs/GAME_DATA.md).

No game data is included. You need your own copy of the game.

This is an unofficial fan project. It is not affiliated with, endorsed by or sponsored by Konami.
Yu-Gi-Oh! and Yu-Gi-Oh! The Duelists of the Roses are trademarks of their respective owners.

## What you need

- Linux x86-64 (the pinned tools are Linux builds), Python 3.10 or newer, `dpkg-deb`.
- For host tests: CMake 3.16+ and a C11 compiler.
- Your own unmodified USA disc image (2,112,782,336 bytes, MD5
  `ca838dae26ff750936c0814d1c2d33f1`).

## Quick start

```sh
make tools        # download and verify the pinned compiler, wibo and MIPS binutils into .tooling/
make test         # ROM-free checks (registry, tool tests)
make host-check   # compile all the C on this machine and run its tests

cp /path/to/your.iso baserom/
make prepare      # verify the image, extract SLUS_205.15 into out/
make match        # compile every function and compare it with your executable
make rebuild      # build the whole executable and compare it byte for byte
```

`make rebuild` writes the rebuilt executable and a comparison report to `build/ntsc-u/rebuilt/`.
`make help` lists every target. [docs/SETUP.md](docs/SETUP.md) has the details.

## Layout

| Path | Contents |
| --- | --- |
| `src/ee/<group>/func_<address>.c` | one function per file, grouped by subject |
| `include/dotr/ee/` | per-group headers: types, data views, callee declarations |
| `asm/ee/vu0/` | the 13 VU0 routines kept as assembly |
| `config/ntsc-u/` | symbol registry, build entries, Japanese names, disc identity |
| `tests/` | host tests (C) and tool tests (Python) |
| `tools/` | preparation, matching, library assembly, whole-executable rebuild, tool setup |
| `docs/` | rules, data reference, module descriptions |
| `baserom/`, `out/`, `build/`, `.tooling/` | your disc, generated files and tools; never committed |

## Documents

| Document | |
| --- | --- |
|
| [Modules](docs/modules/README.md) | every function, by original module, with a description |
| [Game data](docs/GAME_DATA.md) | identifiers, tables, decks, campaign, save data |
| [Architecture](docs/ARCHITECTURE.md) | how the program is organised |
| [Disc identity](docs/FINDINGS.md), [ELF map](docs/ELF_MAP.md), [module inventory](docs/MODULE_INVENTORY.md) | the disc, the executable, overlay and IOP modules |
| [Roadmap](docs/ROADMAP.md) | what is done and what comes next |
| [Tooling](docs/TOOLING.md), [Setup](docs/SETUP.md) | tools and commands |
| [Credits](docs/CREDITS.md), [References](docs/REFERENCES.md), [Licensing](docs/LICENSING.md) | contributors, sources used, licences |

## Credits


  permission; details in [docs/CREDITS.md](docs/CREDITS.md).
- Other references are listed in [docs/REFERENCES.md](docs/REFERENCES.md).
