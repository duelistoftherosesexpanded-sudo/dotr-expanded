#include "dotr/ee/open_units.h"
int TaLogic_IsOpenUnit__0012b210(int x, int y, struct DotrRuleOrder *order)
{
    int i;
    int kind;
    int pos;
    int best;
    unsigned char cnt;
    (void)order;
    kind = SzDuel_GetUnitCardNo(x, y);
    func_00250360_pair(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    cnt = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 101:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 101 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 101 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 102:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 102 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 102 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        case 108:
            return 1;
        case 119:
            for (i = 1; i <= 5; i++) {
                if (SzDuel_GetUnitCardNo(x, i) == 119 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCardNo(x == 0 ? 1 : 0, i) == 119 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
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
        if (TaLogic006_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        switch (SzDuel_GetUnitCardNo(x, y)) {
        case 699:
            if (TaLogic_IsAttackedNow(x, 0, 0) == 1 || TaLogic006_IsBlackHoleCondition(x) == 1) {
                return 1;
            }
            break;
        case 685:
            best = TaLogic_GetMaxAPGraveCard(x, 3);
            if (best >= 0 && (best & 0xffff) >= 2400 && SzDuel_GetUnitMonsterCount(x) < 5) {
                if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                    return 1;
                }
            }
            break;
        case 745:
            for (i = 1; i <= 5; i++) {
                if (func_001df540_word(x, i) && (SzDuel_GetUnitKind(x, i) & 0x1f) != 2 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (func_001df540_word(x == 0 ? 1 : 0, i) && (SzDuel_GetUnitKind(x == 0 ? 1 : 0, i) & 0x1f) != 2 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 3) {
                return 1;
            }
            break;
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
