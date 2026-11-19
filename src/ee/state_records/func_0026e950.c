#include "dotr/ee/state_records.h"

int Ta_GetSummonCost(void)
{
    int i;
    int index;
    int count;
    int best;
    index = SzDuel_GetTurnOwner();
    count = SzDuel_GetHandCount(index);
    best = 0;
    for (i = 0; i < count; i++) {
        int word = SzDuel_InqHand(index, i);
        if (((word & 0xf000) >> 12) > 0) {
            int v = TaHand_GetCardSummonCost(word, index);
            if (best < v) {
                best = v;
            }
        }
    }
    return best;
}
