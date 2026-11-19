#include "dotr/ee/pick_dispatch.h"

int TaLogic_IsHandPlay__0010dfc0(int side, struct DotrStateBlock *unit)
{
    int hand[8];
    unsigned char minTask[32];
    unsigned char numberOfMonsters;
    unsigned char numberOfSpells;
    int i;
    int *p;
    int result;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic002_GetSpellPlay(side, minTask, unit) >= 0) {
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
    if (TaLogic002_GetSpecialComboPlay(side, minTask, (struct DotrPickBlock *)unit, 0) >= 0) {
        return 2;
    }
    if (numberOfMonsters == 0) {
        int *q;
        int j;

        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = hand;
            do {
                if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1) {
                    return 0;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
        return TaLogic002_GetSpellPlay(side, minTask, unit);
    }
    if (TaLogic002_IsMakeDefense(1) == 0) {
        int *q;
        int j;

        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = hand;
            do {
                if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1) {
                    return 0;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    if (TaLogic002_IsMakeDefense(2) == 0) {
        result = TaLogic002_GetSpellPlay(side, minTask, unit);
        if (result >= 0) {
            return result;
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
    result = TaLogic002_GetSpellPlay(side, minTask, unit);
    if (result >= 0) {
        return result;
    }
    return -1;
}
