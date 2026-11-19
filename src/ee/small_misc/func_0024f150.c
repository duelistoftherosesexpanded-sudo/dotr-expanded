#include "dotr/ee/small_misc.h"

/* As Ta_GetMinimumOrderUnitFromFlagList (0x24efb0), but selects the unit with the largest
 * summon order above -1. */
int Ta_GetMaximumOrderUnitFromFlagList(int word)
{
    int i;
    int value = -1;
    int best = -1;
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        int position = Ta_GetUnitOwnerIDFromLstCount(word, i);
        int current = SzDuel_GetUnitSummonOrder(position >> 8, position & 0xff);
        if (value < current) {
            best = position;
            value = current;
        }
    }
    return best;
}
