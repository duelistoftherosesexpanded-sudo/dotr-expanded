#include "dotr/ee/pick_copies.h"

int TaLogic_SetSpecialComboQue__00124c90(int side, unsigned char *minTask, struct DotrPickBlock *block, unsigned char mode)
{
    int list[8];
    int i;
    int *p;
    int *q;
    int j;
    int *r;
    int k;
    int *t;
    int m;
    int kind;

    for (i = 0; i < 5; i++) {
        minTask[i + 6] = 0xFF;
    }
    TaHand_GetNextHand(side, list);
    if (block == 0) {
        return -1;
    }
    if (mode == 0) {
        i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = list;
        do {
            if (i != minTask[6] && i != minTask[7] && block->cardNo == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                minTask[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
        return 0;
    }
    TaLogic005_Is2Head_ThunderDragon(side, list);
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = list;
        do {
        kind = TaHand_GetCardKind(*q) & 0xFF;
        if (kind >> 5 == 0 && (kind & 0x1F) == 0 && TaHand_IsCardSummon2(side, *q) == 1) { /* a Dragon monster */
            minTask[6] = j;
        }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = list;
        do {
        kind = TaHand_GetCardKind(*r) & 0xFF;
        if (kind >> 5 == 0 && (kind & 0x1F) == 18 && TaHand_IsCardSummon2(side, *r) == 1) {
            minTask[7] = k;
        }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    m = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        t = list;
        do {
        kind = TaHand_GetCardKind(*t) & 0xFF;
        if (kind >> 5 == 0 && (kind & 0x1F) == 15 && TaHand_IsCardSummon2(side, *t) == 1) {
            minTask[8] = m;
        }
            t++;
            m++;
        } while (m < TaHand_GetNextHandNum(side));
    }
    return 0;
}
