#include "dotr/ee/pick_copies.h"

int TaLogic013_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *PUnit)
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
            if (*p == 797) { /* 797: Elegant Egotist */
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
            /* 272: Harpie Lady, and it can be summoned */
            if (*q == 272 && TaHand_IsCardSummon2(side, *q) == 1) {
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
    SzDuel_InitCardInfo((struct DotrStateBlock *)PUnit, 273); /* 273: Harpie Lady Sisters */
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = ids;
        do {
            if (k != item[6] && k != item[7] && Ta_GetHandEnchant(273, *r) >= 0 && (TaHand_GetCardKind(*r) >> 5) == 2
                && *r != 795 && *r != 796) { /* 795, 796: Paralyzing Potion, Cursebreaker */
                item[n + 6] = k;
                n++;
                PUnit->apBoost += (short)TaEnc_GetEncScore(*r);
                PUnit->dpBoost += (short)TaEnc_GetEncScore(*r);
            }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    return 1;
}
