#include "dotr/ee/state_queries.h"

/* The index is initialized before the count. */
int TaUtil_GetGraveCardNum(int side, int kind)
{
    int i = 0;
    int count = 0;
    for (; i < SzDuel_GetGraveCount(side); i++) {
        if (kind == TaHand_GetCardKind(SzDuel_InqGrave(side, i)) >> 5) {
            count++;
        }
    }
    return count;
}
