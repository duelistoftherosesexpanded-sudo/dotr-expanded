#include "dotr/ee/lower_rules.h"

int TaLogic012_IsPinch(int side)
{
    int i;
    int set;

    if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 52, 14)) > 2) {
        return 0;
    }
    set = SearchUnitByRange(0, 0, side, 2, 52, 14);
    if (Ta_GetUnitNumFrom22bitFlagList(set) < 5) {
        return 0;
    }
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(set); i++) {
        int pos = Ta_GetUnitOwnerIDFromLstCount(set, i);

        if ((SzDuel_GetUnitLoc(pos >> 8, pos & 0xFF) >> 8) <= 2) {
            return 1;
        }
    }
    return 0;
}
