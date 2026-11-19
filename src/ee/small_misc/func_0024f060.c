#include "dotr/ee/small_misc.h"

/* Among the set positions of `word`, returns the packed position with the
 * lowest 0x1e13c0 score; an equal or lower later score replaces it only
 * when 0x24de70 of that position equals `side`. Locals are declared in
 * ascending saved-register order. */
int Ta_GetMinimumOrderUnitFromFlagList2(int word, int side)
{
    int i;
    int best = 255;
    int result = -1;
    int packed;
    int low;
    int high;
    int score;
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        packed = Ta_GetUnitOwnerIDFromLstCount(word, i);
        high = packed >> 8;
        low = packed & 0xff;
        score = SzDuel_GetUnitSummonOrder(high, low);
        if (score <= best) {
            best = score;
            if (result < 0) {
                result = packed;
            } else if (func_0024de70_pair(high, low) == side) {
                result = packed;
            }
        }
    }
    return result;
}
