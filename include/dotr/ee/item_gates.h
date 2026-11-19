#ifndef DOTR_EE_ITEM_GATES_H
#define DOTR_EE_ITEM_GATES_H

/* 0x1d9420 lSzDuel_IsUnitHide: a yes/no gate on a state item that asks the pad, side counts
 * and cell owner.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/state_queries.h"

/* Callees as declared in state_grid.h and state_queries.h (SzDuel_GetMapInfo,
 * SzGame_GetMenuNum, SzPad_Check, SzDuel_GetPlayer, Ta_IsUnitHide, func_00250360_pair).
 * Assumed: one item argument, int result. The control byte +0xd3c is read as a side
 * index; the cell's first word is compared with the item pointer. */
int lSzDuel_IsUnitHide(struct DotrStateItem *unit);

#endif
