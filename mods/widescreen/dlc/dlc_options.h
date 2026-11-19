/* Switches of the DLC. Change a value and rebuild (mods/obelisk/build.sh). */

#ifndef DLC_OPTIONS_H
#define DLC_OPTIONS_H

/* Every player is given at least this many copies of every new card (cards.json): when a save is
 * loaded and when a game starts. For testing; 0 switches it off, and new cards then come only
 * from their passwords in the deck builder (and, later, from duels). Up to DLC_CARD_COPIES (3). */
#define DLC_GIVE_ALL_CARDS 3
/* With DLC_GIVE_ALL_CARDS: 1 gives the game's own cards too (the player's), 0 only the new ones.
 * The Card Maker's "Starting cards" sets both (tools/card_maker.py). */
#define DLC_GIVE_ALL_GAME 0

/* 1: every card's password is 00000 and its card number, written with eight digits (Blue-Eyes
 * White Dragon, card 0, is 00000000; card 132 is 00000132), the game's own passwords replaced.
 * Cards that had no password get one. New cards keep the one in cards.json. 0: the game's own. */
#define DLC_NUMBER_PASSWORDS 1

/* 1: every monster can be made a deck leader from the start (card_leader.c): the game's monsters
 * that have never led count as the first rank, and new monster cards can lead too (always the
 * first rank, without leader abilities). 0: only monsters the game has given a rank. */
#define DLC_ALL_LEADERS 1

/* 1: no demotion (card_leader.c): experience is never taken away (lost duels, destroyed units)
 * and a leader keeps every rank it has reached. 0: the game's rules. */
#define DLC_NO_DEMOTION 1

/* 1: on the campaign map, L3 shows world coordinates for placing new podiums: a probe moved with
 * the right stick (R1 faster, R3 back to the selected location) and the selected location's
 * (map_debug.c). 0: off. */
#define DLC_DEBUG_MAP 1

#endif
