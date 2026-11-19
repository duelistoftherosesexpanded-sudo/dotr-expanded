#include "dotr/ee/small_misc.h"

/* Builds a bit word of the set positions of `flagList` that share the lowest
 * Ta_GetAPPlusDP value. The assignment inside the test and the declaration order
 * reproduce the delay slots and saved registers. */
int Ta_GetMinimumParamUnitFromFlagList(int flagList)
{
    int i;
    int min = 0xffff;
    int flagListOfWeakest = 0;
    int statSum;
    int low;
    int high;
    int packed;
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(flagList); i++) {
        packed = Ta_GetUnitOwnerIDFromLstCount(flagList, i);
        high = packed >> 8;
        low = packed & 0xff;
        if ((statSum = Ta_GetAPPlusDP(high, low)) < min) {
            flagListOfWeakest = ConvUnitIDtoFlagList(0, high, low);
            min = statSum;
        } else if (statSum == min) {
            flagListOfWeakest = ConvUnitIDtoFlagList(flagListOfWeakest, high, low);
        }
    }
    return flagListOfWeakest;
}
