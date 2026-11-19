#include "dotr/ee/state_queries.h"

/* Score difference between sourceUnit and whatever stands at position (high byte,
 * low byte). Each side's score is 0 when TaUtil_IsVarInfoLeader answers 1 or the
 * upper three bits of its byte (+0x3e, or SzDuel_GetUnitKind) are set; otherwise
 * the item's comes from TaUtil_GetUnitDP2 when +0x40/+0x41 equal col/row and bit 3 of
 * +0x57 is set, else TaUtil_GetUnitAP2, and the other's from TaUtil_GetUnitDP when
 * SzDuel_GetUnitStatus(.., 5) answers, else TaUtil_GetUnitAP. TaLogic_GetActOfs adds a
 * term to each side. Returns 0 for a null sourceUnit. */
int TaLogic_IsUnitWin2(struct DotrStateItem *sourceUnit, int position, int col, int row)
{
    int own;
    int other;
    int lo;
    int hi;

    if (sourceUnit == 0) {
        return 0;
    }
    if (func_00250900_score(sourceUnit) == 1) {
        own = 0;
    } else if (sourceUnit->kind >> 5) {
        own = 0;
    } else if (sourceUnit->locCol == col && sourceUnit->locRow == row && sourceUnit->status.unknown_bit_3) {
        own = func_00250bc0_score(sourceUnit, col, row);
    } else {
        own = func_00250a60_score(sourceUnit, col, row);
    }
    hi = position >> 8;
    lo = position & 0xff;
    own += func_002787f0_score(sourceUnit, func_001df540_score(hi, lo));
    if (func_00250900_score(func_001df540_score(hi, lo)) == 1) {
        other = 0;
    } else if (func_001e0700_score(hi, lo) >> 5) {
        other = 0;
    } else if (func_001e2100_score(hi, lo, 5)) {
        other = func_00250b40_score(hi, lo, col, row);
    } else {
        other = func_002509e0_score(hi, lo, col, row);
    }
    other += func_002787f0_score(func_001df540_score(hi, lo), sourceUnit);
    return own - other;
}
