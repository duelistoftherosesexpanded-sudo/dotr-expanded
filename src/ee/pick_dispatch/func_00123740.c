#include "dotr/ee/pick_dispatch.h"

/* One unit position on both sides: a unit that passes the test and is controlled by `side`
 * is counted. Uses the caller's `n` and `cnt`. */
#define COUNT_BOTH(test) \
    if (test(side)) { \
        if (Ta_IsUnitControl(side, n) == side) { \
            cnt++; \
        } \
    } \
    if (test(side == 0 ? 1 : 0)) { \
        if (Ta_IsUnitControl(side == 0 ? 1 : 0, n) == side) { \
            cnt++; \
        } \
    }
#define T_STATUS(s) SzDuel_GetUnitStatus(s, n, 4) != 0
#define T_KIND(s) (SzDuel_GetUnitKind(s, n) & 0x1f) == 16
#define T_600(s) SzDuel_GetUnitCardNo(s, n) == 600
#define T_560(s) SzDuel_GetUnitCardNo(s, n) == 560

int TaLogic_IsHandPlay__00123740(int side, struct DotrStateBlock *unit)
{
    int hand[8];
    unsigned char minTask[32];
    unsigned char numberOfMonsters;
    unsigned char numberOfSpells;
    unsigned char cnt;
    int result;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    cnt = 0;
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic005_GetSpellPlay(side, minTask, unit, 999) >= 0) {
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
    if (Ta_GetUnitNumFrom22bitFlagList(TaLogic005_Is2Head_ThunderDragon(side, hand) & 0xff) == 3) {
        if (TaLogic_SetSpecialComboQue__00124c90(side, minTask, (struct DotrPickBlock *)unit, 1) >= 0) {
            return 3;
        }
    }
    if (TaLogic005_GetSpecialSpellPlay(side, minTask, unit) >= 0) {
        return 2;
    }
    if (numberOfMonsters == 0) {
        int *p;
        int i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            p = hand;
            do {
                if ((TaHand_GetCardKind(*p) >> 5) == 0 && TaHand_IsCardSummon2(side, *p) == 1) {
                    if (TaLogic005_IsKeepHandCombo(side, hand, *p) == 0) {
                        return 0;
                    }
                }
                p++;
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
    }
    {
        int n;
        for (n = 6; n < 11; n++) {
            COUNT_BOTH(T_STATUS)
        }
        if (cnt > 0) {
            if (TaLogic005_GetSpecialCardPlay(side, minTask, unit, 565) >= 0) {
                return 2;
            }
        }
        cnt = 0;
        for (n = 1; n <= 5; n++) {
            COUNT_BOTH(T_KIND)
        }
        if (cnt >= 3) {
            if (TaLogic005_GetSpecialCardPlay(side, minTask, unit, 593) >= 0) {
                return 2;
            }
        }
    }
    {
        int n;
        unsigned char cnt = 0;
        for (n = 1; n <= 5; n++) {
            COUNT_BOTH(T_600)
        }
        if (cnt >= 2) {
            if (TaLogic005_GetSpecialCardPlay(side, minTask, unit, 560) >= 0) {
                return 2;
            }
        }
    }
    {
        int n;
        unsigned char cnt = 0;
        for (n = 1; n <= 5; n++) {
            COUNT_BOTH(T_560)
        }
        if (cnt >= 2) {
            if (TaLogic005_GetSpecialCardPlay(side, minTask, unit, 600) >= 0) {
                return 2;
            }
        }
    }
    if (TaLogic_IsAttackedNow(side, 0, 0) == 1) {
        if (TaLogic005_GetSpecialCardPlay(side, minTask, unit, 84) >= 0) {
            return 2;
        }
    }
    {
        int *q;
        int j = 0;
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
    result = TaLogic005_GetSpellPlay(side, minTask, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
