#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__001620b0(int x, int y)
{
    int cardId;
    int pos;
    int side;
    unsigned *effect;
    int spot;
    int here;
    int there;
    cardId = SzDuel_GetUnitCardNo(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    side = func_00250360_pair(x, y);
    effect = SzDuel_GetUnitEffect(x, y, 0);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic019_IsDoCerem(func_00250360_pair(x, y), cardId) == 1) {
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
        case 4: /* Destruction */
            spot = TaLogic019_GetUseThunderBolt(side);
            here = SzDuel_GetUnitLoc(x, y);
            there = SzDuel_GetUnitLoc(spot >> 8, spot & 0xFF);
            if ((here & 0xFF) == (there & 0xFF) || here >> 8 == there >> 8) {
                return 1;
            }
            break;
        case 73: /* Flip and Spellbind (SORL) */
            if (TaLogic019_IsUseGFK(side) == 1) {
                return 1;
            }
            break;
        case 37: /* Teleport */
            if (cardId == 749) { /* Dimensionhole */
                if (TaLogic_IsAttackedNow(x, 0, 0) == 1) {
                    return 1;
                }
            }
            if (cardId == 746) { /* Shift */
                if (TaLogic_IsAttackedNow(x, y, 0) == 1) {
                    return 1;
                }
            }
            break;
        case 31: /* Revolt/Steal */
            if (cardId == 733) { /* Brain Control */
                if (TaLogic019_IsUseRevolt(side) == 1) {
                    return 1;
                }
            }
            break;
        }
        break;
    default:
        return 2;
    }
    return 2;
}
