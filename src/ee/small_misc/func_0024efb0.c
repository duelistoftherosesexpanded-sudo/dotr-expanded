#include "dotr/ee/small_misc.h"

/* Among the set bits of the word, the position whose SzDuel_GetUnitSummonOrder value is
 * smallest (first one on ties); -1 when no value is below 255. */
int Ta_GetMinimumOrderUnitFromFlagList(int flagList)
{
    int i;
    int value = 255;
    int best = -1;
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(flagList); i++) {
        int ownerId = Ta_GetUnitOwnerIDFromLstCount(flagList, i);
        int summonOrder = SzDuel_GetUnitSummonOrder(ownerId >> 8, ownerId & 0xff);
        if (summonOrder < value) {
            best = ownerId;
            value = summonOrder;
        }
    }
    return best;
}
