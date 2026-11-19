#include "dotr/ee/pick_copies.h"

int TaLogic021_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int card)
{
    int hand[5];
    int i;
    int *p;
    int *q;
    int j;
    int *r;
    int k;

    TaHand_GetNextHand(side, hand);
    if (unit == 0) {
        return -1;
    }
    for (i = 0; i < 5; i++) {
        task[i + 6] = 0xff;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = hand;
        do {
            if (card == *p && card != 999) {
                task[6] = i;
                SzDuel_InitCardInfo(unit, hand[i]);
                return 1;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = hand;
        do {
            if ((TaHand_GetCardKind(*q) >> 5) != 0 && (TaHand_GetCardKind(*q) >> 5) != 2) {
                task[6] = j;
                SzDuel_InitCardInfo(unit, hand[j]);
                return 1;
            }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = hand;
        do {
            if ((TaHand_GetCardKind(*r) >> 5) != 0) {
                task[6] = k;
                SzDuel_InitCardInfo(unit, hand[k]);
                return 1;
            }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    return -1;
}
