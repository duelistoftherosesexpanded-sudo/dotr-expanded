#include "dotr/ee/pick_copies.h"

int TaLogic017_PlayExzCombo(int side, unsigned char *item, struct DotrPickTotals *unit)
{
    unsigned char picked[5];
    unsigned short rest[5];
    int ids[5];
    int rests;
    int n;
    int i;
    int *p;
    int m;

    n = 0;
    rests = 0;
    for (i = 0; i < 5; i++) {
        picked[i] = rest[i] = 0xff;
    }
    TaHand_GetNextHand(side, ids);
    for (i = 0; i < TaHand_GetNextHandNum(side); i++) {
        if (TaLogic017_IsExzParts(ids[i]) == 1) {
            picked[n] = i;
            n++;
            i = TaHand_GetNextHandNum(side);
        }
    }
    if (n == 0) {
        return -1;
    }
    SzDuel_InitCardInfo((struct DotrStateBlock *)unit, ids[picked[0]]);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (Ta_GetHandEnchant(ids[picked[0]], *p) >= 0 && TaHand_GetCardKind(*p) >> 5 == 2 && *p != 795 && *p != 796) {
                unit->total_38 += (short)TaEnc_GetEncScore(*p);
                unit->total_3a += (short)TaEnc_GetEncScore(*p);
                picked[n] = i;
                n++;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    for (i = 0; i < TaHand_GetNextHandNum(side); i++) {
        if (TaLogic017_IsRefuseEmptyCombo(ids[i]) == 0) {
            rest[rests] = i;
            rests++;
        }
    }
    TaLogic017_SortTempArray(rest, 0, rests - 1, side, ids);
    m = TaHand_GetNextHandNum(side) - n;
    for (i = 0; i < m; i++) {
        item[i + 6] = rest[i];
    }
    for (i = 0; i < n; i++) {
        item[m + 6] = picked[i];
        m++;
    }
    return 1;
}
