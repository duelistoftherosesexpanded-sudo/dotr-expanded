#include "dotr/ee/pick_dispatch.h"

/* A function of its own: the original compares its 0/1 answer with 1 at the call, as for an
 * inlined call. The name is not the original's. */
static inline int cerem_ok(int side, int id)
{
    int triple[3];
    struct DotrDispatchPair *card;
    int cell;
    int x;
    int y;
    int i;
    int r;
    int word;
    unsigned ap;

    card = TaHand_GetCardEffect(id, 0);
    Ta_GetCeremSacrifice(triple, card->unknown_4);
    ap = (unsigned)TaHand_GetCardAPDP(card->unknown_4) >> 16;
    if ((word = Ta_GetSacrificeUnit(triple, side)) < 0) {
        return 0;
    }
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        cell = Ta_GetUnitOwnerIDFromLstCount(word, i);
        x = cell >> 8;
        y = cell & 0xff;
        r = SzDuel_GetUnitLoc(x, y);
        if ((unsigned)TaUtil_GetUnitAP(x, y, r & 0xff, r >> 8) >= ap) {
            return 0;
        }
    }
    return 1;
}

int TaLogic022_IsHandPlay(int side, struct DotrStateBlock *unit)
{
    /* MATCHING DEVICE: one counter and pointer per loop, declared in this order, give the
     * original's register assignment. */
    int ids[8];
    unsigned char item[32];
    unsigned char first;
    unsigned char second;
    int *p2;
    int i2;
    int *p4;
    int i;
    int i3;
    int i4;
    int *p;
    int result;

    first = TaLogic_GetUnitNum(side, 0);
    second = TaLogic_GetUnitNum(side, 1);
    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    if (first == 5) {
        if (TaLogic022_GetSpellPlay(side, item, unit, 999) >= 0) {
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
                    SzDuel_InitCardInfo(unit, ids[i]);
                    return 0;
                }
                p++;
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        return -1;
    }
    {
        int *q;
        int j;

        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                if ((TaHand_GetCardKind(*q) >> 5) == 5) {
                    if (cerem_ok(side, *q) == 1) {
                        if (TaLogic022_GetSpecCardPlay(side, item, unit, (unsigned short)*q) >= 0) {
                            return 2;
                        }
                    }
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    i2 = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p2 = ids;
        do {
            if (*p2 == 700 || *p2 == 702) {
                if (TaLogicCom_GetUseXYRangeSpell__0016cbb0(side, (unsigned *)TaHand_GetCardEffect(*p2, 0)) >= 0) {
                    if (TaLogic022_GetSpecCardPlay(side, item, unit, (unsigned short)*p2) >= 0) {
                        return 2;
                    }
                }
            }
            p2++;
            i2++;
        } while (i2 < TaHand_GetNextHandNum(side));
    }
    for (i3 = 0; i3 < 11; i3++) {
        if (SzDuel_GetUnitCurseRest(side, i3) != 0) {
            if (TaLogic022_GetSpecCardPlay(side, item, unit, 796) >= 0) {
                return 2;
            }
        }
    }
    for (i3 = 0; i3 < 11; i3++) {
        if (SzDuel_GetUnitCurseRest(side, i3) != 0) {
            if (TaLogic022_GetSpecCardPlay(side, item, unit, 795) >= 0) {
                return 2;
            }
        }
    }
    i4 = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p4 = ids;
        do {
            if ((TaHand_GetCardKind(*p4) >> 5) == 0 && TaHand_IsCardSummon2(side, *p4) == 1) {
                SzDuel_InitCardInfo(unit, ids[i4]);
                return 0;
            }
            p4++;
            i4++;
        } while (i4 < TaHand_GetNextHandNum(side));
    }
    result = TaLogic022_GetSpellPlay(side, item, unit, 999);
    if (result >= 0) {
        return result;
    }
    return -1;
}
