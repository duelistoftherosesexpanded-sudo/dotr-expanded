#include "dotr/ee/open_units.h"
int TaLogic_IsOpenUnit__00158360(int x, int y, struct DotrRuleOrder *order)
{
        unsigned char item[32];
    struct DotrOpenCardInfo info;
    int i;
    unsigned char cnt;
    int own;
    int cardNo;
    int lst;
        int side;
    int hand[5];
    unsigned *effect;
    (void)order;
    cnt = 0;
    cardNo = SzDuel_GetUnitCardNo(x, y);
    SzDuel_GetUnitLoc(x, y);
    side = func_00250360_pair(x, y);
    effect = SzDuel_GetUnitEffect(x, y, 0);
    TaHand_GetNextHand(x, hand);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    if (TaLogic017_IsExzParts(cardNo) == 1 && TaLogic017_IsExzodia(side) == 4) {
        return 1;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (cardNo) {
        case 95:
            if (SzDuel_GetLP(x) < 1200) {
                return 1;
            }
            break;
        case 64:
            lst = SearchUnitByRange(0, 0, side, 2, 3, 14);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(lst); i++) {
                own = Ta_GetUnitOwnerIDFromLstCount(lst, i);
                if (TaLogic_IsJudgeUnitDanger(own >> 8, own & 0xff, 0) == 1) {
                    cnt++;
                }
            }
            if (cnt > 1) {
                return 1;
            }
            break;
        }
        return 2;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (effect == 0) {
            return 2;
        }
        switch (*effect >> 8) {
        case 34: /* buff DP */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 2, 29, 14)) > 3) {
                return 1;
            }
            break;
        case 23: /* spellbind */
            lst = SearchUnitByRange(0, 0, side, 2, 52, 14);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(lst); i++) {
                own = Ta_GetUnitOwnerIDFromLstCount(lst, i);
                if ((SzDuel_GetUnitLoc(own >> 8, own & 0xff) >> 8) >= 2) {
                    return 1;
                }
            }
            break;
        case 38: /* summon again */
            for (i = 0; i < TaHand_GetNextHandNum(side); i++) {
                if (SzDuel_Kind2Type(TaHand_GetCardKind(hand[i]) >> 5) == 2) {
                    return 1;
                }
            }
            if (TaLogic017_IsExzSafe(side) == 1) {
                if (TaLogic017_PlayExzCombo(side, item, &info) >= 0) {
                    return 1;
                }
            }
            break;
        case 13: /* alter LP */
            if ((*effect & 0x3f) == 50) {
                if (Ta_GetUnitNum(side == 0 ? 1 : 0, 3) >= 3) {
                    return 1;
                }
                break;
            }
            return 1;
        }
        break;
    default:
        return 2;
    }
    return 2;
}
