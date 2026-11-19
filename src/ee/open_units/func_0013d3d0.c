#include "dotr/ee/open_units.h"
int TaLogic_IsOpenUnit__0013d3d0(int x, int y, struct DotrRuleOrder *order)
{
    int i;
    int kind;
    int ctl;
    int pos;
    int own;
    unsigned char cnt;
    (void)order;
    kind = SzDuel_GetUnitCardNo(x, y);
    ctl = func_00250360_pair(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    SzDuel_GetUnitEffect(x, y, 0);
    cnt = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 197:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 153 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 153 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 153:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 197 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 197 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 170:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 171 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 171 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 171:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 170 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 170 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic010_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        own = TaLogic010_GetUseThunderBolt(ctl);
        own = SzDuel_GetUnitLoc(own >> 8, own & 0xff);
        if ((pos & 0xff) == (own & 0xff) || (pos >> 8) == (own >> 8)) {
            return 1;
        }
        break;
    default:
        return 2;
    }
    return 2;
}
