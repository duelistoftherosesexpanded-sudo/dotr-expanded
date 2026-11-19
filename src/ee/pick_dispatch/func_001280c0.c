#include "dotr/ee/pick_dispatch.h"

int TaLogic006_IsHandPlay(int side, struct DotrStateBlock *block)
{
    int ids[8];
    unsigned char item[32];
    unsigned char first;
    unsigned char second;
    int i;
    int *p;
    int n;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    if (block == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic006_GetSpellPlay(side, item, block, 999) >= 0) {
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
    if (TaLogic006_GetSpecCardPlay(side, item, block, 699) >= 0) {
        if (TaLogic_IsAttackedNow(side, 0, 0) == 1 || TaLogic006_IsBlackHoleCondition(side) == 1) {
            return 2;
        }
    }
    if (TaLogic006_GetSpecCardPlay(side, item, block, 108) >= 0) {
        return 2;
    }
    n = TaLogic_GetMaxAPGraveCard(side, 3);
    if (n >= 0) {
        if ((n & 0xffff) >= 2400) {
            if (TaLogic006_GetSpecCardPlay(side, item, block, 685) >= 0) {
                return 2;
            }
        }
    }
    if (TaLogic006_GetSpecCardPlay(side, item, block, 851) >= 0) {
        if (TaLogic006_IsDoCerem(side, 851) == 1) {
            return 2;
        }
    }
    if (first == 0) {
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
    {
        int *q;
        int j;

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
    }
    result = TaLogic006_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
