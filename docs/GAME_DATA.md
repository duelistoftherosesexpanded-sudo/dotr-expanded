# Game data reference

What is known about the game's tables and run-time layouts, in one place. Addresses are EE
RAM addresses of `SLUS_205.15` (NTSC-U) unless stated. Names in `code font` are original names from the
Japanese build ([topics/jp-build-symbols.md](topics/jp-build-symbols.md)) where one exists.

Status words: **matched code** = read from a function this project compiles to the original bytes;
**observed** = read from memory of the running game (PCSX2 save states) or from the disc;
**sample** = seen in one or a few cases, not checked across the range; **other project** = taken
from another reverse-engineering project's notes and not rechecked here; **modder** = from a
modder's hand-made Ghidra project and not rechecked here; **guess** = inference.
Nothing here is a claim about the original source's declarations: field offsets are facts, field
meanings are readings.

## Identifiers

| Thing | Range | Notes | Status |
|---|---|---|---|
| Card number | 0-853 | 0-682 are monsters, 683-853 the other cards | matched code (limits 683 and 854 in many functions) |
| "No card" | 999 | fills empty deck and hand slots | matched code, observed |
| Card 671 | | treated specially: several record lookups turn it into 999 first | matched code (`SzData_GetRank`, `SzData_SetRank`) |
| Opponent / duel number | 0-21 named | selects AI, name, face; deck and terrain by the samples below. 0 = Simon McMooran (tutorial host), 1 Seto ... 21 Manawyddan fab Llyr | matched code + sample |
| Controller number | byte | 100, 101 = pad 1, pad 2; below 100 = index into the AI table | matched code (`lSzDuel_GetControl`), observed |
| Terrain | 0-9 | 0 forest, 1 wasteland, 2 mountain, 3 meadow, 4 sea, 5 dark, 6 toon, 7 normal, 8 labyrinth, 9 crush | observed |
| Deck leader rank | 0-12 | from experience through `gSzData_awExpTable` | matched code |

The same limits appear as literals in code: 127 instructions in 74 functions hold 683, 854 or a value
derived from them.

## Enumerations

Value names for fields and arguments the code tests, from a modder's hand-made Ghidra project
([CREDITS.md](CREDITS.md)). Status: **modder** (not rechecked here; where they disagree with the
tables above, those win).

- **Card attribute**: 0 Light, 1 Dark, 2 Fire, 3 Earth, 4 Water, 5 Wind.
- **Card type** (byte of the card record): -1 Invalid, 0 Monster, 1 Magic, 2 Power Up, 3 Trap Limited, 4 Trap Full, 5 Ritual.
- **Monster type**: 0 DRAGON, 1 SPELLCASTER, 2 ZOMBIE, 3 WARRIOR, 4 BEAST WARRIOR, 5 BEAST, 6 WINGED BEAST, 7 FIEND, 8 FAIRY, 9 INSECT, 10 DINOSAUR, 11 REPTILE, 12 FISH, 13 SEA SERPENT, 14 MACHINE, 15 THUNDER, 16 AQUA, 17 PYRO, 18 ROCK, 19 PLANT, 20 IMMORTAL.
- **Deck leader abilities**: 0 EXTRA SLOTS, 1 HIDDEN CARD, 2 EXODIA?, 3 DESTINY DRAW, 4 INCREASED MOVEMENT, 5 LP RECOVERY, 6 DIRECT DAMAGE HALVED, 7 TERRAIN CHANGE, 8 LEVEL COST REDUCTION FOR SUMMONING SAME TYPE, 9 EXTENDED SUPPORT RANGE, 10 INCREASED STRENGTH FOR SAME TYPE FRENDLIES, 11 IMPROVED RESISTANCE FOR SAME TYPE FRENDLIES, 12 MOVEMENT BOOST FOR SAME TYPE FRENDLIES, 13 OPEN OPPONENTS CARD, 14 WEAKEN SPECIFIC ENEMY TYPE, 15 SPELLBIND SPECIFIC ENEMY TYPE, 16 DESTROY SPECIFIC ENEMY TYPE.
- **Movement direction**: 0 RIGHT, 1 LEFT, 2 DOWN, 3 UP, 4 DOWN RIGHT, 5 UP RIGHT, 6 DOWN LEFT, 7 UP LEFT.
- **Deck leader rank** (0-12, as in the identifiers table): 0 NCO, 1 LT2, 2 LT1, 3 CPT, 4 MAJ, 5 LTC, 6 COL, 7 BG, 8 RADM, 9 VADM, 10 ADM, 11 SADM, 12 SD.
- **AI unit mission**: 2 Attack mission, 3 Move to Location, 4 Run Mission, 5 Move Mission, 6 Unknown Mission (uses location), 9 Enchant Mission, 10 Summon Mission, 11 Trap Mission, 12 Revive Mission, 13 Revolt Mission, 14 Do None Mission.
- **AI task type**: 2 Attack, 3 ConfirmUnit, 5 Run, 6 MoveToLocation, 9 Enchant, 10 Summon, 11 Trap, 12 Revive, 13 Revolt, 14 Do nothing.
- **Equip (enchant) kind**: 0 Normal Equip, 1 GiveMonsterEffect, 2 Cursebreaker, 3 Paralysing potion, 4 Fusion Equips.
- **Unit status bits** (bit numbers): 0 hasMoved, 1 isRevolt, 2 isShowingFaceup, 3 isShowingDefense, 4 isFaceUp, 5 isInDefense, 6 IsCurrentlySelected.
- **Combat effect flags** (bit masks): 1 AttackerPreventsCombatDamage, 2 DefenderPreventsCombatDamage, 4 PreventCombatRevival, 8 DefenderCancelsPowerups, 16 AttackerCancelsPowerups, 32 AttackerResultFlag, 64 DefenderResultFlag, 128 UnknownFlag.
- **Arithmetic operation** (for example SzData_CalcExp): 0 Add, 1 Minus, 2 Multiply, 3 Divide, 4 Set.
- **Campaign locations**: 0 Brest, 1 Milford Haven, 2 Chester, 3 Lancashire, 4 Newcastle, 5 Towton, 6 St Albans, 7 Bosworth, 8 Tewkesbury, 9 Exeter, 10 Isle of Man, 11 Stonehenge, 12 Windsor, 13 London, 14 Canterbury, 15 Dover, 16 Straight of Dover, 17 Boulogne, 18 Amiens, 19 Paris, 20 Le Mans, 21 Rennes.
- **Duelist numbers**: 0 Simon, 1 Seto, 2 Weevil, 3 Rex, 4 Keith, 5 Ishtar, 6 Necromancer, 7 Darkness Ruler, 8 Labyrinth Ruler, 9 Pegasus, 10 Richard, 11 Tea, 12 Tristan, 13 Mai, 14 Mako, 15 Joey, 16 Shadi, 17 Jasper Grandpa, 18 Bakura, 19 Yugi, 20 MFL SK, 21 MFL C, 22 DM K, 23 DM I, 24 DM T, 25 DM S, 26 Tut1, 27 Tut2, 28 Tut3, 29 Tut4, 30 Tut5, 100 Player 1, 101 Player 2.
- **AI search types**: 0 DRAGON, 1 SPELLCASTER, 2 ZOMBIE, 3 WARRIOR, 4 BEAST WARRIOR, 5 BEAST, 6 WINGED BEAST, 7 FIEND, 8 FAIRY, 9 INSECT, 10 DINOSAUR, 11 REPTILE, 12 FISH, 13 SEA-SERPENT, 14 MACHINE, 15 THUNDER, 16 AQUA, 17 PYRO, 18 ROCK, 19 PLANT, 20 LIGHT, 21 DARK, 22 FIRE, 23 EARTH, 24 WATER, 25 WIND, 26 CARD THAT ATTACK ME?, 28 CARD NUMBER, 29 MONSTER, 30 NON MONSTER, 31 ALL CARDS, 32 CARD WITH WEAKEST STATS, 33 CARD WITH BEST STATS, 34 LEADER, 36 CARDS ATK GREATER THAN OR EQUAL EXTRA DATA, 37 CARDS ATK LESS THAN OR EQUAL EXTRA DATA, 45 ALL CARDS1, 46 monster on field of SearchMode, 47 Graveyard, 48 random but not self, 49 FEMALE, 52 NON LEADER, 53 ATTACK POSITION CARDS, 54 FACE UP TRAPS, 55 DRAGONS WITH ATK LESS THAN OR EQUAL EXTRA DATA, 56 DRAGONS WITH ATK GREATER THAN OR EQUAL EXTRA DATA, 57 MONSTERS WITH DP GREATER THAN OR EQUAL EXTRA DATA, 58 MONSTERS WITH DP LESS THAN OR EQUAL EXTRA DATA, 59 IMMORTAL, 60 FACE-DOWN CARDS, 61 ALL CARDS2.

## The duel block (`gSzDuel_dl`, 0x317490)

One block holds a duel in progress. `SzDuel_Act` (0x1cee60) keeps its address in a register and reaches
about 1,500 of its data accesses through it.

| Offset | Address | Size | Content | Status |
|---|---|---|---|---|
| 0 + 848 * side | 0x317490, 0x3177e0 | 848 each | per-side record, sides 0 and 1 | observed |
| side + 0x1e0 | | 40 x u16, then a count | the side's deck list (999 = empty) | observed |
| side + 418 | 0x317632 (side 0) | u16 | step counter of a tutorial script | observed |
| side + 0x33a | 0x3177ca, 0x317b1a | u16 | life points | observed |
| side + 0x33c | | u8 | controller number | matched code, observed |
| 3384 | 0x3181c8 | u8 | step of the 3D battle sequence (third switch of `SzDuel_Act`) | observed |
| 3385 | 0x3181c9 | u8 | battle step, 0-18 (second switch) | observed |
| 3415 | 0x3181e7 | u8 | duel phase: 0-18 set-up, 19-98 main (first switch index = phase - 19), 99 map edit | observed; 99 from `SzDuel_InitMapEdit` |
| | 0x318e60 | 49 x 28 | the 7 x 7 board (`gSzDuel_aaMapInfo`), row by row; terrain is the byte at +0x16 (`field`) of a cell | observed |

Related: the frame counter is the word at 0x317254 (inside `gSzGame_gl`, 0x317250); it does
not advance on the title screen.

Phase meanings observed in the running game (not read from the code): 0-18 set-up, 20-23 standby,
24 cursor, 29-31 decide, 55 move, 63-70 hand, 35-38 surrender, 39-46 options and graveyard, 84-98
endings (93-97 the leader's rank going up).

## Controllers and the AI table (`g_aLogFunc`)

`lSzDuel_GetControl` indexes a table of function pointers at 0x2baeb0 with the controller number.

| Controller | Entry | Status |
|---|---|---|
| 1-21 | `L001`-`L021`: one routine per named opponent; that the order is that of the name strings is a sample (controller 11 in a duel against Tea) | observed (table contents) |
| 0, 22-31 | all `L022`, a generic routine | observed |
| 32-36 | `L032`-`L036`: the tutorial lessons' scripts | observed; 35 and 36 not seen running |
| 25 | what Custom Duel gives a computer side | observed |

Writing a controller byte during a duel hands that side to the pad or to an AI at once; with both sides on
AIs a duel plays to its end without input (observed in more than 88 games).

## Cards

| Table | Address | Layout | Status |
|---|---|---|---|
| Card data | 0x2bf080 | 854 records of 20 bytes | observed; the fields are not written up here |
| Fusion data | `FusionDat` 0x29e830 | 0x19eb4 bytes; `Fusion` searches its first 26,540 words, each holding the two input cards in bits 0-9 (the larger number) and 10-19 and the result in bits 20-29 (999 when the pair is absent) | name and size from the Japanese symbol table; layout matched code (`Fusion`) |
| Special cards | `aSpecialCard` 0x2d0870 (0x60), `aTreasureData` 0x2d08d0 (0x58) | | name and size from the Japanese symbol table; read by matched code (`SzData_RollSpecial`, `SzData_ExchSpecial`, `SzData_SearchTreasure`), fields not written up |

## Built-in decks (`gSzData_ConstDeck`, 0x2d0970)

51 decks of 41 u16 (4,182 bytes). Word 0 = leader card in the low 12 bits, leader rank in the high 4; words
1-40 the cards. Readers: `SzData_GetConstDeck`, `SzData_SetupFromConstDeck` (both matched; limits 51 and 41).

| Decks | Use | Status |
|---|---|---|
| 0-16 | starter decks (the set-up code treats deck < 17 differently) | matched code |
| 17-26 | five tutorial lessons, a pair each (17/18 ... 25/26) | matched code (`Taku_Camp_Go_Rensyu_Duel`, `TkCamp_Term`) |
| 27-50 | opponents: deck = 26 + opponent number | matched code (`Taku_Camp_TEKI_WINDOW` sets up deck 26 + duel number for its panel); sample (opponent 11 used deck 37) |

## Terrain layouts (0x2cee5c)

46 maps of 49 bytes, `[y][x]`, one terrain code per square. `SzDuel_SetupMap` (matched) reads them for map
numbers 0-45; numbers 46-50 and 51-55 come from five layouts stored in each player's save data. A campaign
duel used the map with its duel number (sample: 11). Maps 22-30 are a single terrain each. Other readers:
`lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe`, `Taku_Camp_TIKEI_Map_Hyouji`.

## Text

| Thing | Where | Status |
|---|---|---|
| String offsets | 0x2d19d0: 3,073 s32, in 16-bit words into the blob that follows (`gSzMsg_Data`) | other project; used here to decode names |
| String word | bits 0-12 character code, bit 13 half width, bit 14 back-reference (copy from another string), bit 15 end | other project |
| Letters | `A` = 0x56, `a` = 0x73, `0` = 0x4c, space 0x70 | observed (decoded names) |
| Font | 0x2f8dec: 369 glyphs of 22 x 22, 4 bits a pixel | other project |
| Opponent names | string 196 + opponent number (196 Simon McMooran ... 207 Tea ... 216, 217 Manawyddan fab Llyr) | observed; matched code |
| Place names | string 218 + duel number (Stonehenge, Chester, Tewkesbury ...) | observed; matched code |

`Taku_Camp_TEKI_WINDOW` picks both strings per campaign location with a switch (location 11 also
by side and progress; locations 1, 15 and 17 have a place string, 240-242, but keep the default
name 196) and stores name - 196 as the duel number.

## Pictures

| Thing | Where | Status |
|---|---|---|
| Opponent faces | `DATA/TEXANM.MRG`: 22 sheets of 256 x 256 (face plus blink and mouth frames); sheet = opponent number + 1, sheets 0 and 1 both Simon | observed (sheets extracted and looked at); the face request in `Taku_Camp_TEKI_WINDOW` uses duel number + 1 (the number itself for name 217) |
| Story portraits | `DATA/TEXEVE.MRG` sheets 17-23 | other project |
| Title pictures | `DATA/TEXETC.MRG` sheets 0, 1 | other project |

## Campaign

| Thing | Where | Layout | Status |
|---|---|---|---|
| Locations | `model_data` 0x298ba0 | 22 records of 52 bytes: x, height, z as floats; two bytes; seven shorts, [4] and [5] the events after a win and a loss. All 22 used; 0 and 11 always shown, 1-10 for one side, 12-21 for the other | matched code (`TkCamp_Act2`, `Taku_Camp_What_KATIMAKE`), observed |
| Routes | `root_no` 0x299020 | 32 records of 76 bytes: 16 points (x, z shorts), then point count, a flag (1 on sea routes), from, to, from, to. 28 used, 28-31 empty | observed |
| Progress ("board") | `save_data` 0x299e18 (the structure used by code starts at 0x299e10) | side; `seen[]` per location; win counters per duel number (2 x 32 shorts); +0xc4 duel number; +0xc6 current location; +0xc9 a switch special to location 11 | matched code (several `Taku_` functions); field meanings are readings |
| Starting a duel | `Taku_Camp_Set_Go_Duel` | `SzGame_SetMenuNum(1, duel number)`, `(2, side)`, `(3 / 4, 1100 for the human, duel number + 1000 for the AI)` | matched code |
| Duel number | board + 0xc4 | written by `Taku_Camp_TEKI_WINDOW`, the opponent panel of the campaign map, as name string - 196 | matched code |
| Tutorial | `Taku_Camp_Go_Rensyu_Duel`, `TkCamp_Term` | lesson 0-4 -> controllers 32-36 and deck pairs 17/18 ... 25/26; the lesson counter wraps after the third | matched code |

## Save data

| Thing | Where | Status |
|---|---|---|
| Player blocks | `gSzData_All` 0x32f790: two blocks of 65,440 bytes | observed |
| Which block is "mine" | `gSzData_bRoseFlag` 0x34f6d0 selects the order | matched code (the side macro) |
| Card records | start of a block: 683 records of 76 bytes; +0x48 rank in the low 14 bits, +0x4a experience (u16) | matched code (`SzData_GetRank`, `SzData_SetRank`, `SzData_SetRankDirect`) |
| Experience table | `gSzData_awExpTable` 0x2d0850: 0, 500, 1000, 2000, 4000, 7000, 11000, 16000, 22000, 30000, 40000, 52000, 65535 | observed |
| Deck A | block + 64916: 40 x u16; the leader u16 follows at +80 | observed |
| Memory-card write | one write of 0x100f0 bytes in `TakuSave_card_file_kakikomi` | matched code |

Check: setting a leader's experience one below the next rank in a save state and winning the duel ran the
rank-up phases 93-97.

## Disc files

`mapFile` (0x293090) is 97 records of four 64-bit words (name pointer, first sector, last sector, sector
count) plus a terminator. The sector numbers stored in the executable are stale (the first 30 are 1,861
sectors off). `RhFile_Init` (matched) looks each name up in the ISO 9660 directory at start and overwrites
first sector and count; count = bytes >> 11, rounded down, so a file's tail beyond a whole sector is not read.

Check: a copy of the disc with `DATA/TEXETC.MRG` moved to the end and its old sectors zeroed booted, and the
table in memory showed the new sector.

## Not yet described

Card record fields; which routes are open at the start of a campaign; the event (dialogue) data; the
overlays' data. How the opponent panel chooses names, faces and which locations it offers is in
`Taku_Camp_TEKI_WINDOW` ([modules/campaign.md](modules/campaign.md)).
