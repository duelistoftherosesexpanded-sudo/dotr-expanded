#include "dotr/ee/pick_dispatch.h"

/* The four-key search reproduces only as an inlined helper returning the
 * first key TaLogic003_IsEmptyAtkLine rejects, or -1; the helper name is not original. */
static inline int find_00119a40(int side)
{
    int k;
    for (k = 0; k < 4; k++) {
        if (TaLogic003_IsEmptyAtkLine(side, (unsigned char)k) == 0) {
            return k;
        }
    }
    return -1;
}

int TaLogic_IsHandPlay__00119a40(int side, struct DotrStateBlock *unit)
{
    int hand[8];
    unsigned char minTask[32];
    unsigned char numberOfMonsters;
    unsigned char numberOfSpells;
    int i;
    int *p;
    int *q;
    int j;
    int k;
    int *r;
    int m;
    int result;

    numberOfMonsters = TaLogic_GetUnitNum(side, 0);
    numberOfSpells = TaLogic_GetUnitNum(side, 1);
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, hand);
    if (numberOfMonsters == 5) {
        if (TaLogic003_GetSpellPlay(side, minTask, unit, 999) >= 0) {
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
    result = TaLogic003_IsSpellPlay(side, minTask, (struct DotrPickBlock *)unit);
    if (result >= 0) {
        return TaLogic003_GetSpellPlay(side, minTask, unit, result);
    }
    k = find_00119a40(side);
    if (k >= 0) {
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
        return TaLogic003_GetSpellPlay(side, minTask, unit, 999);
    }
    m = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        r = hand;
        do {
            if ((TaHand_GetCardKind(*r) >> 5) == 0 && TaHand_IsCardSummon2(side, *r) == 1) {
                SzDuel_InitCardInfo(unit, hand[m]);
                return 0;
            }
            r++;
            m++;
        } while (m < TaHand_GetNextHandNum(side));
    }
    result = TaLogic003_GetSpellPlay(side, minTask, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
