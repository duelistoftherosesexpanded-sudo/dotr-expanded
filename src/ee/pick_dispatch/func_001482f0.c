#include "dotr/ee/pick_dispatch.h"

int TaLogic013_IsHandPlay(int side, struct DotrStateBlock *block)
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
        if (TaLogic013_GetSpellPlay(side, item, block, 999) >= 0) {
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
    if (TaLogic013_GetSpecCardPlay(side, item, block, 843) >= 0) {
        if (TaLogic013_IsDoCerem(side, 843) == 1) {
            return 2;
        }
    }
    if (TaLogic013_GetSpecialComboPlay(side, item, (struct DotrPickBlock *)block) == 1) {
        return 3;
    }
    if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 0x1E001C, 14)) > 0) {
        if (TaLogic013_GetSpecCardPlay(side, item, block, 30) >= 0) {
            return 2;
        }
    }
    if (TaLogic013_IsUseRevive(side) >= 0) {
        if (TaLogic013_GetSpecCardPlay(side, item, block, 685) >= 0) {
            return 2;
        }
    }
    for (n = 0; n < 11; n++) {
        if (SzDuel_GetUnitCurseRest(side, n) != 0) {
            if (TaLogic013_GetSpecCardPlay(side, item, block, 796) >= 0) {
                return 2;
            }
        }
    }
    j = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        q = ids;
        do {
            if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1 && *q != 272) {
                SzDuel_InitCardInfo(block, ids[j]);
                return 0;
            }
            q++;
            j++;
        } while (j < TaHand_GetNextHandNum(side));
    }
    result = TaLogic013_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
