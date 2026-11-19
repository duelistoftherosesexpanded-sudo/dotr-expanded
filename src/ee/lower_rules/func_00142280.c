#include "dotr/ee/lower_rules.h"

int TaLogic011_IsUseReversalGoddess(int side)
{
    if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 0x3E80025, 14)) >= 3) {
        return 1;
    }
    return 0;
}
