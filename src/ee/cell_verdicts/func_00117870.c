#include "dotr/ee/cell_verdicts.h"
/* Inlined: either of the two (0, 0, side, 1, .., 14) sets is non-empty. */
static inline int func_00117870_either(int side)
{
    if (SearchUnitByRange(0, 0, side, 1, 0x2001C, 14) != 0) {
        return 1;
    }
    if (SearchUnitByRange(0, 0, side, 1, 28, 14) != 0) {
        return 1;
    }
    return 0;
}
int TaLogic_IsOpenUnit__00117870(int x, int y, int z)
{
    int kind;
    int side;
    unsigned *p;
    SzDuel_GetUnitLoc(x, y);
    kind = SzDuel_GetUnitCardNo(x, y);
    side = func_00250360_pair(x, y);
    p = SzDuel_GetUnitEffect(x, y, 0);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 83: /* Lord of D */
            if (func_00117870_either(side) == 1) {
                return 1;
            }
            break;
        case 146: /* Swordstalker */
            if (TaUtil_GetGraveCardNum(side, 0) >= 6) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic001_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(x, y) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        switch (*p >> 8) {
        case 13: /* Alter LP (Heal/Damage) */
            return 1;
        case 2: /* Terrain Change */
            if (TaLogic001_IsLandNearCntr(SzDuel_GetUnitLoc(x, y)) == 1) {
                return 1;
            }
            break;
        case 20: /* Revive */
            return 2;
        default:
            return TaLogicCom_IsOpenUnit__00111320(x, y, z);
        }
        break;
    default:
        return 2;
    }
    return 2;
}
