#ifndef DOTR_EE_PAIR_PICKS_H
#define DOTR_EE_PAIR_PICKS_H

/* 0x27b360: filters a word list through the cell-pair test 0x27b520 and hands the survivors
 * to 0x27b650.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

#include "dotr/ee/small_misc.h"

/* TaLogic_GetAtkUnitNow: the first argument (side) is never read. `ownerIds` is walked from its
 * first halfword while the index is below TaLogic_GetUnitCtlLstLen(ownerIds); `units` is the
 * 16-byte entry array TaLogic_GetActionUnitFromOwnerID searches, and the entry found is handed to
 * TaLogic_IsAtkNowToGoal as a cell pair (same storage, cast in the source). The local
 * list has 22 halfwords and its fill is not bounded by the code. Callees as
 * declared in small_misc.h. */
int TaLogic_GetAtkUnitNow(int side, unsigned short *ownerIds, struct DotrMiscEntry16 *units);

#endif
