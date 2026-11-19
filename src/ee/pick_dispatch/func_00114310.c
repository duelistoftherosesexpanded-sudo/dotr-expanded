#include "dotr/ee/pick_dispatch.h"

int TaLogic001_IsHandPlay(int side, struct DotrStateBlock *unit)
{
    int hand[8];
    unsigned char minTask[32];
    unsigned char numberOfMonsters;
    unsigned char numberOfSpells;
    int i;
    int *p;
    int *q;
    int j;
    int n;
    int result;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic001_GetSpellPlay(side, minTask, unit, 999) >= 0) {
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
    if (TaLogic001_GetSpecCardPlay(side, minTask, unit, 837) >= 0) {
        if (TaLogic001_IsDoCerem(side, 837) == 1) {
            return 2;
        }
    }
    if (TaLogic001_IsUseLordOfDragon(side) == 1) {
        if (SearchUnitByRange(0, 0, side, 1, 0x53001C, 14) == 0) {
            if (TaLogic001_GetSpecCardPlay(side, minTask, unit, 83) >= 0) {
                return 2;
            }
            if (TaLogic001_GetProtectCardPlay(side, minTask, unit) >= 0) {
                return 2;
            }
        }
    }
    if (TaLogic001_IsUseLifeHourGlass(side) == 1) {
        if (TaLogic001_GetSpecCardPlay(side, minTask, unit, 377) >= 0) {
            return 2;
        }
    }
    if (TaLogic001_IsUseBraveHourGlass(side) == 1) {
        if (TaLogic001_GetSpecCardPlay(side, minTask, unit, 390) >= 0) {
            return 2;
        }
    }
    if (TaLogic001_IsUseRevive(side) >= 0) {
        if (TaLogic001_GetSpecCardPlay(side, minTask, unit, 685) >= 0) {
            return 2;
        }
    }
    for (n = 0; n < 11; n++) {
        if (SzDuel_GetUnitCurseRest(side, n) != 0) {
            if (TaLogic001_GetSpecCardPlay(side, minTask, unit, 796) >= 0) {
                return 2;
            }
        }
    }
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
    result = TaLogic001_GetSpellPlay(side, minTask, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
