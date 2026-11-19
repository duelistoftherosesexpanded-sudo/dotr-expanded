#include "dotr/ee/pick_copies.h"


int TaLogic007_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickBlock *block)
{
    int ids[5];
    int *p;
    int i;
    unsigned char n;
    int *q;
    int j;
    int k;

    for (k = 0; k < 5; k++) {
        item[k + 6] = 0xff;
    }
    TaHand_GetNextHand(side, ids);
    if (block == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (i != item[6] && i != item[7] && block->cardNo == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                item[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if ((TaHand_GetCardKind(ids[item[6]]) >> 5) == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                if (j != item[6] && j != item[7] && Ta_GetHandEnchant(ids[item[6]], *q) >= 0 &&
                    (TaHand_GetCardKind(*q) >> 5) == 2 && *q != 795 && *q != 796) {
                    item[n + 6] = j;
                    n++;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return block->kind >> 5;
}
