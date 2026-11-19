#ifndef DOTR_EE_LOGIC010_SUMMON_H
#define DOTR_EE_LOGIC010_SUMMON_H

/* TaLogic010_GetSummonPiece (0x13ba30), logic 010's copy of the summon-place search of the
 * free_spots group.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

#include "dotr/ee/free_spots.h"

/* Callees outside free_spots.h, as order_tries.h and position_rules.h declare them. */
extern int TaLogic_GetWarnHideLvl__0013da70(int side);
extern int TaLogic010_GetEmptyLine(int side);

/* TaLogic010_GetSummonPiece (name from the Japanese build's symbols). Assumed: the parameters of its
 * sibling copies (TaLogic012_GetSummonPiece); the second parameter is not read. */
int TaLogic010_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *block, int packed);

#endif
