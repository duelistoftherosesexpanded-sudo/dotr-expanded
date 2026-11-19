#include "dotr/ee/small_misc.h"

/* The final conditional expression reproduces the retained branch. */
int Ta_GetRandomUnit(int flagList)
{
    int unitCount = Ta_GetUnitNumFrom22bitFlagList(flagList);
    int result;
    if (unitCount == 0) {
        return -1;
    }
    result = Ta_GetUnitOwnerIDFromLstCount(flagList, func_0019a5c8() % unitCount);
    return result >= 0 ? result : -1;
}
