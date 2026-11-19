#include "dotr/ee/pick_dispatch.h"

int TaLogic014_IsHandPlay(int side, struct DotrStateBlock *block)
{
    int ids[8];
    unsigned char item[32];
    unsigned char first;
    unsigned char second;
    int i;
    int *p;
    int *q;
    int j;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    if (block == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic014_GetSpellPlay(side, item, block, 999) >= 0) {
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
    if (TaLogic014_GetSpecCardPlay(side, item, block, 852) >= 0) {
        if (TaLogic014_IsDoCerem(side, 852) == 1) {
            return 2;
        }
    }
    if (TaLogic014_IsActiveMode() == 1) {
        if (SzDuel_GetUnitSpellCount(side) == 0) {
            if (TaLogic014_GetSpecCardPlay(side, item, block, 701) >= 0) {
                return 2;
            }
        }
    }
    if (SzDuel_GetUnitMonsterCount(side) >= 4) {
        if (TaLogic014_GetSpecCardPlay(side, item, block, 742) >= 0) {
            return 2;
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
    result = TaLogic014_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
