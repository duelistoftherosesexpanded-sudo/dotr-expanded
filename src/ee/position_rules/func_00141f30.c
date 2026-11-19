#include "dotr/ee/position_rules.h"

int TaLogic011_IsUseBindChain(int side)
{
    int i;
    int word;
    unsigned char count;
    int list;

    count = 0;
    list = SearchUnitByRange(0, 0, side, 3, 3, 14);
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(list); i++) {
        word = Ta_GetUnitOwnerIDFromLstCount(list, i);
        if (side == func_00250360_pair(word >> 8, word & 0xff)) {
            return 0;
        }
        if (TaLogic_IsJudgeUnitDanger(word >> 8, word & 0xff, 0) == 1) {
            count++;
        }
    }
    if (count > 0) {
        return 1;
    }
    return 0;
}
