#ifndef DOTR_EE_HAND_UP_H
#define DOTR_EE_HAND_UP_H

/* lSzDuel_InitHandUp: refills one player's five hand slots through the SzDuel hand accessors.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The duel work area at 0x317490 (the Japanese build's symbol table places it inside
 * gSzGame_gl), under the declaration owner_draws.h already has. Only byte offsets from its
 * start are used here: +3372 a list of hand slot numbers, +3401 the count of occupied hand
 * slots, +3402 the hand count stored at the end, and per player (848 bytes each) +836 a
 * flag byte. */
#include "dotr/ee/owner_draws.h"

/* Callees; assumed prototypes as in their own groups. */
void SzDuel_PackHand(int side);
int lSzDuel_InitHandInfo(int a, int fieldSlot, int cardId, int d, int e, int f, int g);
int SzDuel_InqHand(int side, int position);
int SzDuel_GetHandCount(int side);
int SzDuel_GetDeckCount(int side);
int Ta_GetDestinyDraw(int side, unsigned short *cardPool, short *poolWeights, int *note);
int SzDuel_DrawDeck(int side);
int SzDuel_AddHand(int side, int cardId);
/* Library block: func_0019a5c8 is assumed to return the next pseudo-random
 * number (0..0x7fffffff), func_001a22c0 to convert a float to an integer. */
extern int func_0019a5c8(void);
extern int func_001a22c0(float value);

/* 0x1d7d10. Parameter names are assumed; no debug information for this function. */
void lSzDuel_InitHandUp(int side, int draw);

#endif
