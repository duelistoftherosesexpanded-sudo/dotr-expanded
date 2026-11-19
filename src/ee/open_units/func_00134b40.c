#include "dotr/ee/open_units.h"
int TaLogic_IsOpenUnit__00134b40(int x, int y)
{
    int i;
    int cardNo;
    int pos;
    unsigned *effect;
    unsigned char cnt;
    cardNo = SzDuel_GetUnitCardNo(x, y);
    func_00250360_pair(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    effect = SzDuel_GetUnitEffect(x, y, 0);
    cnt = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (cardNo) {
        case 190:
            for (i = 1; i <= 5; i++) {
                if ((SzDuel_GetUnitKind(x, i) & 0x1f) == 9 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if ((SzDuel_GetUnitKind(x == 0 ? 1 : 0, i) & 0x1f) == 9 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 3) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic008_IsDoCerem(func_00250360_pair(x, y), cardNo) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (effect == 0) {
            return 2;
        }
        switch (*effect >> 8) {
        case 2: /* terrain change */
        case 72:
            i = TaLogic008_GetWallEmpty(x);
            if (i >= 0 && i == SzDuel_GetUnitLoc(x, y)) {
                return 1;
            }
            break;
        case 13: /* alter LP */
            return 1;
        case 37: /* teleport */
            for (i = 1; i <= 5; i++) {
                if (func_001df540_word(x, i) && SzDuel_GetUnitCardNo(x, i) != 145 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (func_001df540_word(x == 0 ? 1 : 0, i) && SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) != 145 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt > 0) {
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
