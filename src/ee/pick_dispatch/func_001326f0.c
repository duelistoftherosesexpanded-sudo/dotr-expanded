#include "dotr/ee/pick_dispatch.h"

int TaLogic008_IsHandPlay(int side, struct DotrStateBlock *block)
{
    int ids[8];
    unsigned char item[32];
    unsigned char first;
    unsigned char second;
    int i;
    int *p;
    int *q;
    int j;
    int n;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    if (block == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic008_GetSpellPlay(side, item, block, 999) >= 0) {
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
    if (TaLogic008_GetSpecCardPlay(side, item, block, 832) >= 0) {
        if (TaLogic008_IsDoCerem(side, 832) == 1) {
            return 2;
        }
    }
    for (n = 1; n <= 5; n++) {
        if (SzDuel_GetUnit(side, n) != 0 && SzDuel_GetUnitCardNo(side, n) == 145 && func_00250360_pair(side, n) == side) {
            if (TaLogic008_GetSpecCardPlay(side, item, block, 746) >= 0) {
                return 2;
            }
        }
        if (SzDuel_GetUnit(side == 0 ? 1 : 0, n) != 0 && SzDuel_GetUnitCardNo(side == 0 ? 1 : 0, n) == 145 && func_00250360_pair(side == 0 ? 1 : 0, n) == side) {
            if (TaLogic008_GetSpecCardPlay(side == 0 ? 1 : 0, item, block, 746) >= 0) {
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
    result = TaLogic008_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
