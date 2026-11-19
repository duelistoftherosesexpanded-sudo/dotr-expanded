#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__00141b90(int x, int y)
{
    unsigned *word;
    int kind;
    int side;
    int own;
    kind = SzDuel_GetUnitCardNo(x, y);
    side = func_00250360_pair(x, y);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 388: /* Tenderness */
            own = func_001df470_side(x);
            if (own < func_001df470_side(x == 0 ? 1 : 0)) {
                return 1;
            }
            break;
        case 368: /* Gyakutenno Megami */
            if (TaLogic011_IsUseReversalGoddess(side) == 1) {
                return 1;
            }
            break;
        case 383: /* Hoshiningen */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 20, 14)) >= 3) {
                return 1;
            }
            break;
        case 386: /* Binding Chain */
            if (TaLogic011_IsUseBindChain(side) == 1) {
                return 1;
            }
            break;
        }
        return 2;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        word = func_00250740_word(x, y, 0);
        if (word == 0) {
            return 2;
        }
        switch (*word >> 8) {
        case 13: /* Alter LP (Heal/Damage) */
            if (func_001df470_side(side) < 2000) {
                return 1;
            }
            break;
        case 18: /* AP/DP Buff */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 8, 14)) >= 4) {
                return 1;
            }
            break;
        case 37: /* Teleport */
            if (TaLogic_IsAttackedNow(side, 0, 0) == 1) {
                return 1;
            }
            break;
        }
        return 2;
    default:
        return 2;
    }
    return 2;
}
