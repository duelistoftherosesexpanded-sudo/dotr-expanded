#include "dotr/ee/pick_dispatch.h"

int TaLogic007_IsHandPlay(int side, struct DotrStateBlock *unit, unsigned char isActive)
{
    int hand[8];
    unsigned char minTask[32];
    unsigned char numberOfMonsters;
    int i;
    int n;
    unsigned char numberOfSpells;
    int k;
    unsigned char count;
    int *p;
    int result;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    count = 0;
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic007_GetSpellPlay(side, minTask, unit, 999) >= 0) {
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
    if (isActive == 0) {
        if ((n = TaLogic007_GetCurseCardPlay(side, hand, (struct DotrPickBlock *)unit, minTask)) >= 0) {
            return 2;
        }
        if (TaLogic007_IsDoCerem(side, 834) == 1) {
            if ((n = TaLogic007_GetSpecCardPlay(side, hand, unit, minTask, 834)) >= 0) {
                return 2;
            }
        }
        for (k = 0; k < 11; k++) {
            if (SzDuel_GetUnit(0, k) != 0) {
                if (Ta_IsUnitControl(0, k) != side) {
                    count++;
                }
            }
            if (SzDuel_GetUnit(1, k) != 0) {
                if (Ta_IsUnitControl(1, k) != side) {
                    count++;
                }
            }
        }
        if (n >= 4) {
            if (TaLogic007_GetSpecCardPlay(side, hand, unit, minTask, 326) >= 0) {
                return 2;
            }
        }
        if (TaLogic007_IsRevivePlay(side) >= 0) {
            if (TaLogic007_GetSpecCardPlay(side, hand, unit, minTask, 305) >= 0) {
                return 2;
            }
        }
    } else {
        if (TaLogic007_IsDoCerem(side, 834) == 1) {
            if ((n = TaLogic007_GetSpecCardPlay(side, hand, unit, minTask, 834)) >= 0) {
                return 2;
            }
        }
        for (k = 0; k < 11; k++) {
            if (SzDuel_GetUnit(0, k) != 0) {
                if (Ta_IsUnitControl(0, k) != side) {
                    count++;
                }
            }
            if (SzDuel_GetUnit(1, k) != 0) {
                if (Ta_IsUnitControl(1, k) != side) {
                    count++;
                }
            }
        }
        if (n >= 4) {
            if (TaLogic007_GetSpecCardPlay(side, hand, unit, minTask, 326) >= 0) {
                return 2;
            }
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
                    return 0;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    result = TaLogic007_GetSpellPlay(side, minTask, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
