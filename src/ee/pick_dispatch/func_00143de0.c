#include "dotr/ee/pick_dispatch.h"

int TaLogic012_IsHandPlay(int side, struct DotrStateBlock *block)
{
    int ids[8];
    unsigned char item[32];
    unsigned char first;
    unsigned char second;
    int i;
    int *p;
    int id;
    int best;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    best = -1;
    if (block == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic012_GetSpellPlay(side, item, block, 999) >= 0) {
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
    if (TaLogic012_GetSpecCardPlay(side, ids, block, item, 846) >= 0) {
        if (TaLogic012_IsDoCerem(side, 846) == 1) {
            return 2;
        }
    }
    if (TaLogic012_IsPinch(side) == 1) {
        if (TaLogic012_GetSpecCardPlay(side, ids, block, item, 731) >= 0) {
            return 2;
        }
        if (TaLogic012_GetSpecCardPlay(side, ids, block, item, 751) >= 0) {
            return 2;
        }
    }
    {
        int y;
        int *q;
        int j;
        int v;

        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                for (y = 1; y <= 5; y++) {
                    if (TaLogic012_IsLineEmpty(*q, y) == 1) {
                        if (TaLogic012_GetSpecCardPlay(side, ids, block, item, (unsigned short)*q) >= 0) {
                            v = SzDuel_GetUnitAP2((struct DotrStateItem *)block, 0);
                            if (best < v) {
                                id = *q;
                                best = v;
                            }
                        }
                    }
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    if (best >= 0) {
        SzDuel_InitCardInfo(block, id);
        return 2;
    }
    {
        int *q;
        int j;

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
    }
    result = TaLogic012_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
