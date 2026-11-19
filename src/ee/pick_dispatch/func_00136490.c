#include "dotr/ee/pick_dispatch.h"

int TaLogic009_IsHandPlay(int side, struct DotrStateBlock *unit)
{
    int hand[8];
    unsigned char item[32];
    unsigned char numberOfMonsters;
    unsigned char numberOfSpells;
    int i;
    int *p;
    int v;
    int result;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic009_GetSpellPlay(side, item, unit, 999) >= 0) {
            result = 1;
        } else {
            result = -1;
        }
        return result;
    }
    if (numberOfSpells == 5) {
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            p = hand;
            do {
                if ((TaHand_GetCardKind(*p) >> 5) == 0 && TaHand_IsCardSummon2(side, *p) == 1) {
                    SzDuel_InitCardInfo(unit, hand[i]);
                    return 0;
                }
                p++;
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        return -1;
    }
    if (TaLogic009_GetSpecCardPlay(side, item, unit, 830) >= 0) {
        if (TaLogic009_IsDoCerem(side, 830) == 1) {
            return 2;
        }
    }
    if (TaLogic009_GetSpecCardPlay(side, item, unit, 849) >= 0) {
        if (TaLogic009_IsDoCerem(side, 849) == 1) {
            return 2;
        }
    }
    v = TaLogic009_GetRevoltObj(side);
    if (v >= 0) {
        if (((v >> 16) & 0xffff) != 0xffff) {
            v = TaLogic009_GetSpecCardPlay(side, item, unit, 732);
            if (v >= 0) {
                return 2;
            }
        }
        if ((v & 0xffff) != 0xffff) {
            if (TaLogic009_GetSpecCardPlay(side, item, unit, 733) >= 0) {
                return 2;
            }
        }
    }
    if (TaLogic009_IsUseCurseDragonPot(side) == 1) {
        if (TaLogic009_GetSpecCardPlay(side, item, unit, 683) >= 0) {
            return 2;
        }
    }
    {
        int *r;
        int k;

        k = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            r = hand;
            do {
                if (SzDuel_Kind2Type(TaHand_GetCardKind(*r) >> 5) == 2) {
                    SzDuel_InitCardInfo(unit, hand[k]);
                    return 1;
                }
                r++;
                k++;
            } while (k < TaHand_GetNextHandNum(side));
        }
    }
    if (TaLogic009_GetSpecCardPlay(side, item, unit, 713) >= 0) {
        return 2;
    }
    if (TaLogic009_IsUseCopyCat(side) >= 0) {
        if (TaLogic009_GetSpecCardPlay(side, item, unit, 687) >= 0) {
            return 2;
        }
    }
    {
        int *q;
        int j;

        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = hand;
            do {
                if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1) {
                    SzDuel_InitCardInfo(unit, hand[j]);
                    return 0;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    result = TaLogic009_GetSpellPlay(side, item, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
