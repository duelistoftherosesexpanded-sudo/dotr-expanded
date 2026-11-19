#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__0015cbb0(int x, int y)
{
    int kind;
    int pos;
    int side;
    unsigned *p;
    kind = SzDuel_GetUnitCardNo(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    side = func_00250360_pair(x, y);
    p = SzDuel_GetUnitEffect(x, y, 0);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 224: /* Gate Deeg */
            if (SzDuel_GetSummonPower(side) < 2) {
                return 1;
            }
            break;
        case 652: /* Man Eater */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(pos & 0xFF, pos >> 8, side, 1, 52, 20)) > 1) {
                return 2;
            }
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(pos & 0xFF, pos >> 8, side, 2, 52, 20)) > 5) {
                return 1;
            }
            break;
        case 655: /* Ancient Tree of Enlightenment */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 2, 52, 14)) > 5) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic018_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (p == 0) {
            return 2;
        }
        switch (*p >> 8) {
        case 24: /* Flip card */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 2, 52, 14)) > 5) {
                return 1;
            }
            break;
        case 18: /* AP/DP Buff */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(pos & 0xFF, pos >> 8, side, 1, 29, 29)) > 0) {
                return 2;
            }
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 19, 14)) > 2) {
                return 1;
            }
            break;
        }
        break;
    default:
        return 2;
    }
    return 2;
}
