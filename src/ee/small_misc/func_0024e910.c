#include "dotr/ee/small_misc.h"

int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList)
{
    int unitNum = 0;
    int i;
    for (i = 0; i < 22; i++) {
        if (((flagList >> i) & 1) == 1) {
            unitNum++;
        }
    }
    return unitNum;
}
