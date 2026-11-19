#include "dotr/ee/state_queries.h"

int TaHand_GetNextHandNum(int side)
{
    int total = SzDuel_GetHandCount(side) + SzDuel_GetDeckCount(side);
    if (total >= 5) {
        total = 5;
    }
    return total;
}
