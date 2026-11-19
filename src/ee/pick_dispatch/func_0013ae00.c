#include "dotr/ee/pick_dispatch.h"

int TaLogic010_IsHandPlay(int side, struct DotrStateBlock *block)
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
    int n;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    if (block == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic010_GetSpellPlay(side, item, block, 999) >= 0) {
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
                    SzDuel_InitCardInfo(block, ids[i]);
                    return 0;
                }
                p++;
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        return -1;
    }
    if (TaLogic010_GetSpecCardPlay(side, item, block, 840) >= 0) {
        if (TaLogic010_IsDoCerem(side, 840) == 1) {
            return 2;
        }
    }
    if (TaLogic010_GetUseThunderBolt(side) >= 0) {
        if (TaLogic010_GetSpecCardPlay(side, item, block, 700) >= 0) {
            return 2;
        }
    }
    k = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = ids;
        do {
            if (SzDuel_Kind2Type(TaHand_GetCardKind(*r) >> 5) == 2) {
                SzDuel_InitCardInfo(block, ids[k]);
                return 1;
            }
            r++;
            k++;
        } while (k < TaHand_GetNextHandNum(side));
    }
    for (n = 0; n < 11; n++) {
        if (SzDuel_GetUnitCurseRest(side, n) != 0) {
            if (TaLogic010_GetSpecCardPlay(side, item, block, 796) >= 0) {
                return 2;
            }
        }
    }
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = ids;
        do {
            if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1) {
                SzDuel_InitCardInfo(block, ids[j]);
                return 0;
            }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    result = TaLogic010_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
