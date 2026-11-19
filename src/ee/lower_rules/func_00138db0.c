#include "dotr/ee/lower_rules.h"

int TaLogic009_IsUseCurseDragonPot(int side)
{
    int pos;
    int lo;
    int hi;

    if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 2, 0, 14)) >= 2) {
        return 1;
    }
    pos = Ta_GetUnitOwnerIDFromLstCount(SearchUnitByRange(0, 0, side, 3, 33, 14), 0);
    hi = pos >> 8;
    lo = pos & 0xFF;
    if (side != func_00250360_pair(hi, lo) && (SzDuel_GetUnitKind(hi, lo) >> 5) == 0
        && (SzDuel_GetUnitKind(hi, lo) & 0x1F) == 0) {
        return 1;
    }
    return 0;
}
