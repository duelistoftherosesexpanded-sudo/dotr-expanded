#include "dotr/ee/pick_copies.h"

int TaLogic015_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals)
{
    int ids[5];
    int i;
    int *p;
    unsigned char n;
    int *q;
    int j;

    for (i = 0; i < 5; i++) {
        item[i + 6] = 0xff;
    }
    TaHand_GetNextHand(side, ids);
    if (totals == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (i != item[6] && i != item[7] && totals->id_3c == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                item[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if (TaHand_GetCardKind(ids[item[6]]) >> 5 == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                if (j != item[6] && j != item[7] && Ta_GetHandEnchant(ids[item[6]], *q) >= 0 &&
                    TaHand_GetCardKind(*q) >> 5 == 2 && *q != 795 && *q != 796) {
                    item[n + 6] = j;
                    n++;
                    totals->total_38 += (short)TaEnc_GetEncScore(*q);
                    totals->total_3a += (short)TaEnc_GetEncScore(*q);
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return 1;
}
