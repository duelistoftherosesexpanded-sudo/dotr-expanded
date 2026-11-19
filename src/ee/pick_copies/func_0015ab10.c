#include "dotr/ee/pick_copies.h"

int TaLogic018_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *unit)
{
    int hand[8];
    int n;
    int cardId;
    int i;
    int *p;
    int *q;
    int v;
    int j;

    n = 1;
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    cardId = unit->cardNo;
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = hand;
        do {
            if (cardId == *p && TaHand_GetCardKind(*p) >> 5 == 0) {
                item[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            q = &hand[j];
            if (Ta_GetHandEnchant(cardId, *q) >= 0 && TaHand_GetCardKind(*q) >> 5 == 2 && (v = *q) != 795 && v != 796) {
                if (func_001e02f0_pick(unit, 0) + TaEnc_GetEncScore(v) < 1400) {
                    unit->apBoost += (short)TaEnc_GetEncScore(*q);
                    unit->dpBoost += (short)TaEnc_GetEncScore(*q);
                    item[n + 6] = j;
                    n++;
                } else {
                    j = TaHand_GetNextHandNum(side);
                }
            }
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    return 1;
}
