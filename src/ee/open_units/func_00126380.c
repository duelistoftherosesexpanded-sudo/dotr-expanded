#include "dotr/ee/open_units.h"
int TaLogic_IsOpenUnit__00126380(int x, int y, struct DotrRuleOrder *order)
{
    int i;
    int cardId;
    int pos;
    int ctl;
    unsigned *eff;
    unsigned char cnt;
    (void)order;
    cardId = SzDuel_GetUnitCardNo(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    ctl = func_00250360_pair(x, y);
    eff = SzDuel_GetUnitEffect(x, y, 0);
    cnt = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (SzDuel_GetUnitCardNo(x, y)) {
        case 560:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 600 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 600 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 600:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 560 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 560 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 593:
            for (i = 1; i <= 5; i++) {
                if ((SzDuel_GetUnitKind(x, i) & 0x1f) == 16 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if ((SzDuel_GetUnitKind(x == 0 ? 1 : 0, i) & 0x1f) == 16 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 3) {
                return 1;
            }
            break;
        case 565:
            for (i = 6; i < 11; i++) {
                if (SzDuel_GetUnitStatus(x, y, 4) && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitStatus(x == 0 ? 1 : 0, y, 4) && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt > 0) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic005_IsDoCerem(func_00250360_pair(x, y), cardId) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (eff == 0) {
            return 2;
        }
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, ctl, 1, *eff & 0x3f, 14)) > 2) {
            return 1;
        }
        break;
    case 2:
    case 3:
    case 4:
    default:
        return 2;
    }
    return 2;
}
