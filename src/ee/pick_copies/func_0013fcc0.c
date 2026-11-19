#include "dotr/ee/pick_copies.h"

int TaLogic011_SetSpecialComboQue(int side, unsigned char *minTask, struct DotrPickBlock *targetUnit)
{
    int hand[8];
    int i;
    int *p;
    unsigned char n;
    int *q;
    int j;

    for (i = 0; i < 5; i++) {
        minTask[i + 6] = 0xFF;
    }
    TaHand_GetNextHand(side, hand);
    if (targetUnit == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = hand;
        do {
            if (i != minTask[6] && i != minTask[7] && targetUnit->cardNo == *p) {
                minTask[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if (TaHand_GetCardKind(hand[minTask[6]]) >> 5 == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = hand;
            do {
                if (j != minTask[6] && j != minTask[7] && Ta_GetHandEnchant(hand[minTask[6]], *q) >= 0
                    && TaHand_GetCardKind(*q) >> 5 == 2 && *q != 795 && *q != 796) { /* 795, 796: Paralyzing Potion, Cursebreaker */
                    minTask[n + 6] = j;
                    n++;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return targetUnit->kind >> 5;
}
