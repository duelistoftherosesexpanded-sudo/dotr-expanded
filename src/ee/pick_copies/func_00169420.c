#include "dotr/ee/pick_copies.h"

int TaLogic021_SetSpecialComboQue(int side, unsigned char *task, struct DotrPickTotals *unit)
{
    int hand[5];
    int i;
    int *currentHandCardPtr1;
    unsigned char n;
    int *currentHandCardPtr2;
    int j;

    for (i = 0; i < 5; i++) {
        task[i + 6] = 0xff;
    }
    TaHand_GetNextHand(side, hand);
    if (unit == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        currentHandCardPtr1 = hand;
        do {
            if (i != task[6] && i != task[7] && unit->id_3c == *currentHandCardPtr1 && TaHand_IsCardSummon2(side, *currentHandCardPtr1) == 1) {
                task[6] = i;
            }
            currentHandCardPtr1++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if (TaHand_GetCardKind(hand[task[6]]) >> 5 == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            currentHandCardPtr2 = hand;
            do {
                if (j != task[6] && j != task[7] && Ta_GetHandEnchant(hand[task[6]], *currentHandCardPtr2) >= 0 &&
                    TaHand_GetCardKind(*currentHandCardPtr2) >> 5 == 2 && *currentHandCardPtr2 != 795 && *currentHandCardPtr2 != 796) {
                    task[n + 6] = j;
                    n++;
                }
                currentHandCardPtr2++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return 1;
}
