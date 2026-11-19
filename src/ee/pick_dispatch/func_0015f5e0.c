#include "dotr/ee/pick_dispatch.h"

int TaLogic019_IsHandPlay(int side, struct DotrStateBlock *block)
{
    int ids[8];
    unsigned char item[32];
    int first;
    int second;
    int i;
    int *p;
    int n;
    int result;

    /* The two bytes are masked at the store and again at each test, so they are kept as ints
     * here; their original type is not known. */
    first = (unsigned char)TaLogic_GetUnitNum(side, 0);
    second = (unsigned char)TaLogic_GetUnitNum(side, 1);
    if (block == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if ((unsigned char)first == 5) {
        if (TaLogic019_GetSpellPlay(side, item, block, 999) >= 0) {
            result = 1;
        } else {
            result = -1;
        }
        return result;
    }
    if ((unsigned char)second == 5) {
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
    if (TaLogic019_GetSpecCardPlay(side, item, block, 833) >= 0) {
        if (TaLogic019_IsDoCerem(side, 833) == 1) {
            return 2;
        }
    }
    if (TaLogic019_GetSpecCardPlay(side, item, block, 853) >= 0) {
        if (TaLogic019_IsDoCerem(side, 853) == 1) {
            return 2;
        }
    }
    if (TaLogic019_GetUseThunderBolt(side) >= 0) {
        if (TaLogic019_GetSpecCardPlay(side, item, block, 700) >= 0) {
            return 2;
        }
    }
    if (TaLogic019_IsUseGFK(side) >= 0) {
        if (TaLogic019_GetSpecCardPlay(side, item, block, 715) >= 0) {
            return 2;
        }
    }
    if (TaLogic019_IsUseRevive(side) >= 0) {
        if (TaLogic019_GetSpecCardPlay(side, item, block, 685) >= 0) {
            return 2;
        }
    }
    if (TaLogic019_IsUseSaint(side) >= 0) {
        if (TaLogic019_GetSpecCardPlay(side, item, block, 43) >= 0) {
            return 2;
        }
    }
    if (TaLogic019_GetSpecialComboPlay(side, item, (struct DotrPickBlock *)block) == 1) {
        return 3;
    }
    for (n = 0; n < 11; n++) {
        if (SzDuel_GetUnitCurseRest(side, n) != 0) {
            if (TaLogic019_GetSpecCardPlay(side, item, block, 796) >= 0) {
                return 2;
            }
        }
    }
    for (n = 0; n < 11; n++) {
        if (SzDuel_GetUnitCurseRest(side, n) != 0) {
            if (TaLogic019_GetSpecCardPlay(side, item, block, 795) >= 0) {
                return 2;
            }
        }
    }
    {
        int *q;
        int j;

        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1 && *q != 297) {
                    SzDuel_InitCardInfo(block, ids[j]);
                    return 0;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    result = TaLogic019_GetSpellPlay(side, item, block, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
