#ifndef DOTR_EE_TRAP_A_H
#define DOTR_EE_TRAP_A_H

/* Chk_TrapA, the check of whether moving a unit onto a cell springs one of the opponent's adjacent
 * trap units or a leader ability.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

#include "dotr/ee/action_reaction.h"

/* struct jp_7b52 (the record Chk_TrapA fills) and the declarations of SzDuel_GetUnitLoc,
 * SzDuel_GetUnitStatus, Ta_SearchUncTypeNaturalEffectUserNum and Ta_IsActiveTrapB come from
 * action_reaction.h, the group of Chk_TrapA's caller. */

/* Callees as this function calls them. Assumed: every parameter name and count beyond what
 * the call sites pass, and the result types. Ta_GetCurrentSide is called with a unit's
 * (side, id) pair here (state_queries.h defines it; small_misc.h has a no-argument view of
 * the same address for a thunk). The others are declared as in the groups that define
 * them (state_grid.h, state_records.h, small_misc.h, side_table.h, cell_tints.h,
 * state_queries.h). */
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_IsLDRange(int side, int col, int row);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int Ta_IsLDAbl(int ability, int side);
extern int Ta_IsLD_DestObj(int side, int objKind);
extern int Ta_IsLD_CurseObj(int side, int objKind);
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int GetCap(int limit, int value);
extern void SortByOrder(int *ownerIds, int low, int high);
extern int SzDuel_GetUnitKindOfs(int side, int pos);
extern int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
extern void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);
extern int Ta_IsResistAll(int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);

/* Ta_GetUnitEffect (0x250740) as this caller uses it: the result is read as a pointer to
 * unsigned words, of which only word 0 (its bits above the low byte) and word 1 are read.
 * A separate symbol because other groups declare the same address with other result types.
 * Assumed: the result type, the three parameters (unit side, unit id, and a selector passed
 * as 1 for the moving unit and 0 for the trap units) and that a null result is possible
 * (it is tested only at the first call). */
extern unsigned *func_00250740_trapa(int a, int b, int n);

/* 0x24b310 Chk_TrapA (name and prototype from the Japanese build's debug information, as
 * declared in action_reaction.h). Decides whether moving unit (nSide, nID) to cell (nX, nZ)
 * springs a trap and fills *eInfo with the effect to run. Returns 0 (nothing), 1 (an
 * answer that stops the trap: eNO 102 or 254), 257 (a trap or a leader ability fires) or
 * 0x10201 (leader ability 13). Assumed: the meaning given to the return values, to the two
 * halves of PrNo (low 16 bits zero enables the leader-ability part; bit 16 limits the scan
 * to trap units with status bit 4) and to the unit-kind classes (kind >> 5: 0 and 3). */
int Chk_TrapA(int nSide, int nID, int nX, int nZ, struct jp_7b52 *eInfo, int PrNo);

#endif
