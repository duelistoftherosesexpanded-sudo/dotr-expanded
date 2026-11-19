#include "dotr/ee/pick_dispatch.h"

int TaLogic_IsHandPlay__0011e6e0(int side, struct DotrStateBlock *unit)
{
    int hand[8];
    unsigned char minTask[32];
    unsigned char numberOfMonsters;
    unsigned char numberOfSpells;
    unsigned char cnt;
    int result;
    int n;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    cnt = 0;
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic004_GetSpellPlay(side, minTask, unit, 999) >= 0) {
            result = 1;
        } else {
            result = -1;
        }
        return result;
    }
    if (numberOfSpells == 5) {
        int i = 0;
        int *p;
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
    if (TaLogic004_GetSpecialComboPlay(side, minTask, (struct DotrPickBlock *)unit, 0) >= 0) {
        return 2;
    }
    {
        for (n = 1; n < 5; n++) {
            if ((SzDuel_GetUnitKind(side, n) & 0x1f) == 14) {
                if (Ta_IsUnitControl(side, n) == side) {
                    cnt++;
                }
            }
            if ((SzDuel_GetUnitKind(side == 0 ? 1 : 0, n) & 0x1f) == 14) {
                if (Ta_IsUnitControl(side == 0 ? 1 : 0, n) == side) {
                    cnt++;
                }
            }
        }
        if (cnt >= 3) {
            if (TaLogic004_GetSpecialComboPlay(side, minTask, (struct DotrPickBlock *)unit, 1) >= 0) {
                return 3;
            }
            result = TaLogic004_GetSpellPlay(side, minTask, unit, 738);
            if (result >= 0) {
                return result;
            }
        }
    }
    if ((result = TaLogic004_GetSpellPlay(side, minTask, unit, 823)) >= 0) {
        unsigned char cnt = 0;
        int n;
        for (n = 1; n < 11; n++) {
            if (Ta_IsUnitControl(side, n) == side) {
                if (SzDuel_GetUnitCardNo(side, n) == 823) {
                    n = 11;
                    cnt = 0;
                }
            } else if (SzDuel_GetUnitStatus(side, n, 4) == 0) {
                cnt++;
            }
            if (Ta_IsUnitControl(side == 0 ? 1 : 0, n) != side) {
                if (SzDuel_GetUnitCardNo(side == 0 ? 1 : 0, n) == 823) {
                    n = 11;
                    cnt = 0;
                }
            } else if (SzDuel_GetUnitStatus(side == 0 ? 1 : 0, n, 4) == 0) {
                cnt++;
            }
        }
        if (cnt >= 6) {
            return result;
        }
    }
    {
        int *q;
        int j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = hand;
            do {
                if ((TaHand_GetCardKind(*q) >> 5) == 0 && TaHand_IsCardSummon2(side, *q) == 1 && *q != 342) {
                    SzDuel_InitCardInfo(unit, hand[j]);
                    return 0;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    result = TaLogic004_GetSpellPlay(side, minTask, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
