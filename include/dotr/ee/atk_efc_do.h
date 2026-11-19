#ifndef DOTR_EE_ATK_EFC_DO_H
#define DOTR_EE_ATK_EFC_DO_H

/* Do_AtkEfc (0x262190, AtkEfc.c by the Japanese build's debug information), the effect step called
 * by Manage_AtkEfc.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* struct jp_570b5 and the prototype of Do_AtkEfc (both from the Japanese build's debug information). */
#include "dotr/ee/atk_efc.h"

/* Callees under the assumed prototypes of the groups that define them (small_misc, state_grid,
 * side_table, state_records); func_0019a5c8 (library block, no original name) returns the next
 * pseudo-random number. */
int Match_AtkEfcObj(int compMode, int searchType, int b, int c, int d, int e);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetField(int x, int y);
extern int SzDuel_SetField(int x, int y, int value);
extern int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern int func_0019a5c8(void);

#endif
