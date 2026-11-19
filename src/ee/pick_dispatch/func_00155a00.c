#include "dotr/ee/pick_dispatch.h"

int TaLogic017_IsHandPlay(int side, struct DotrStateBlock *unit)
{
    int ids[8];
    unsigned char item[32];
    unsigned char first;
    unsigned char second;
    int i;
    int *p;
    int *q;
    int j;
    int *r;
    int k;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic017_GetSpellPlay(side, item, unit, 999) >= 0) {
            result = 1;
        } else {
            result = -1;
        }
        return result;
    }
    if (second == 5) {
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            p = ids;
            do {
                if ((TaHand_GetCardKind(*p) >> 5) == 0 && TaHand_IsCardSummon2(side, *p) == 1) {
                    SzDuel_InitCardInfo(unit, ids[i]);
                    return 0;
                }
                p++;
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        return -1;
    }
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = ids;
        do {
            if (SzDuel_Kind2Type(TaHand_GetCardKind(*r) >> 5) == 2) {
                SzDuel_InitCardInfo(unit, ids[k]);
                return 2;
            }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    if (TaLogic017_IsExzSafe(side) == 1) {
        if (TaLogic017_PlayExzCombo(side, item, unit) >= 0) {
            return 3;
        }
    }
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = ids;
        do {
            if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1) {
                return 0;
            }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    result = TaLogic017_GetSpellPlay(side, item, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
