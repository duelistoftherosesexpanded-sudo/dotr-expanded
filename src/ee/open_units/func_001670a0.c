#include "dotr/ee/open_units.h"
int TaLogic_IsOpenUnit__001670a0(int x, int y, struct DotrRuleOrder *order)
{
    unsigned w;
    int list2;
    int j;
    unsigned char cnt;
    int cardNumber;
    int own;
    int i;
    int list;
    int ctl;
    unsigned *eff;
    int pos;
    (void)order;
    cnt = 0;
    cardNumber = SzDuel_GetUnitCardNo(x, y);
    ctl = func_00250360_pair(x, y);
    eff = SzDuel_GetUnitEffect(x, y, 0);
    pos = SzDuel_GetUnitLoc(x, y);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 5:
        if (TaLogic020_IsDoCerem(func_00250360_pair(x, y), cardNumber) == 1 && SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
            return 1;
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 1) == 5) {
            return 2;
        }
        if (eff == 0) {
            return 2;
        }
        w = *eff;
        switch (w >> 8) {
        case 23: /* spellbind */
            own = Ta_GetUnitOwnerIDFromLstCount(Ta_RemoveObjFromFlagLst(ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, ctl, (w >> 6) & 3, w & 0x3f, 14), x, y), ctl), 0);
            if (own >= 0 && ctl != func_00250360_pair(own >> 8, own & 0xff)) {
                return 1;
            }
            break;
        case 4: /* destruction */
            switch (eff[1] >> 16) {
            case 18:
                list = ConvUnitIDtoFlagList_OFF(ConvUnitIDtoFlagList_OFF(ConvUnitIDtoFlagList_OFF(SearchUnitByRange(pos & 0xff, pos >> 8, ctl, (w >> 6) & 3, w & 0x3f, 18), x, y), x, 0), x == 0 ? 1 : 0, 0);
                for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(list); i++) {
                    own = Ta_GetUnitOwnerIDFromLstCount(list, i);
                    if (own >= 0) {
                        if (ctl == func_00250360_pair(own >> 8, own & 0xff)) {
                            return 2;
                        }
                        cnt++;
                    }
                }
                /* Host build: `+ 0` only keeps GCC's -Wtype-limits quiet; the byte
                 * counter is tested for a negative value, which cannot happen. */
                if (cnt + 0 >= 0) {
                    return 1;
                }
                break;
            default:
                list2 = Ta_RemoveObjFromFlagLst(ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, ctl, (w >> 6) & 3, w & 0x3f, 14), x, y), ctl);
                for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(list2); j++) {
                    own = Ta_GetUnitOwnerIDFromLstCount(list2, j);
                    if (own >= 0) {
                        if (ctl == func_00250360_pair(own >> 8, own & 0xff)) {
                            return 2;
                        }
                        cnt++;
                    }
                }
                /* `+ 0` as above (host build: -Wtype-limits only). */
                if (cnt + 0 >= 0) {
                    return cnt;
                }
                break;
            }
            break;
        case 13: /* alter LP */
            switch (SzDuel_GetUnitCardNo(x, y)) {
            case 707:
                if (func_001df470_side(ctl) < 2000) {
                    return 1;
                }
                break;
            case 713:
                return 1;
            case 714:
                if (SzDuel_GetUnitCount(ctl == 0 ? 1 : 0) >= 3) {
                    return 1;
                }
                break;
            }
            break;
        case 24: /* flip */
            if (Ta_GetUnitNumFrom22bitFlagList(Ta_RemoveObjFromFlagLst(ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, ctl, (w >> 6) & 3, w & 0x3f, 14), x, y), ctl)) >= 5) {
                return 1;
            }
            break;
        case 38: /* summon again */
            if (SzDuel_InqHandPhase(ctl) == 0) {
                return 1;
            }
            break;
        }
        break;
    }
    return 2;
}
