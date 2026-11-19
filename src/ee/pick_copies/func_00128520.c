#include "dotr/ee/pick_copies.h"

int TaLogic006_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id)
{
    int ids[5];
    int i;
    int *p;
    int *q;
    int j;
    int *r;
    int k;

    TaHand_GetNextHand(side, ids);
    if (block == 0) {
        return -1;
    }
    for (i = 0; i < 5; i++) {
        item[i + 6] = 0xff;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (id == *p && id != 999) {
                item[6] = i;
                SzDuel_InitCardInfo(block, ids[i]);
                return 1;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = ids;
        do {
            if ((TaHand_GetCardKind(*q) >> 5) != 0 && (TaHand_GetCardKind(*q) >> 5) != 2) {
                item[6] = j;
                SzDuel_InitCardInfo(block, ids[j]);
                return 1;
            }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = ids;
        do {
            if ((TaHand_GetCardKind(*r) >> 5) != 0) {
                item[6] = k;
                SzDuel_InitCardInfo(block, ids[k]);
                return 1;
            }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    return -1;
}
