#include "dotr/ee/pick_copies.h"

int TaLogic015_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    int ids[5];
    int i;
    int found;
    int *p;
    int *q;
    int j;
    int paired;
    unsigned char n;
    int *r;
    int k;

    TaHand_GetNextHand(side, ids);
    i = 0;
    found = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (*p == 799) {
                found = 1;
                item[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    if (found == 0) {
        return -1;
    }
    j = 0;
    paired = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = ids;
        do {
            if (*q == 7 && TaHand_IsCardSummon2(side, *q) == 1) {
                item[7] = j;
                paired = 1;
            }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    if (paired == 0) {
        return -1;
    }
    n = 2;
    SzDuel_InitCardInfo((struct DotrStateBlock *)block, 501);
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = ids;
        do {
            if (k != item[6] && k != item[7] && Ta_GetHandEnchant(501, *r) >= 0 && (TaHand_GetCardKind(*r) >> 5) == 2
                && *r != 795 && *r != 796) {
                item[n + 6] = k;
                n++;
                block->apBoost += (short)TaEnc_GetEncScore(*r);
                block->dpBoost += (short)TaEnc_GetEncScore(*r);
            }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    return 1;
}
