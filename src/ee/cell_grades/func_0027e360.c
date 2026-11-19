#include "dotr/ee/cell_grades.h"

int TaLogic_IsDefStyleUnit(int side, int pos, unsigned short word, unsigned char isMove3, int last)
{
    int reaction;
    int y;
    int x;
    int unitAp;

    if (pos == 0) {
        return 2;
    }
    if (word != SzDuel_GetUnitLoc(side, pos)) {
        return 2;
    }
    x = word & 0xff;
    y = word >> 8;
    /* reaction 5 is Flip/Activate */
    reaction = TaLogic_GetActionReaction(side, pos, x, y, 4, last);
    TaLogic_GetActionReaction(side, pos, x, y, 5, last);
    if (SzDuel_GetUnitStatus(side, pos, 5) != 0) {
        if (SzDuel_GetUnitKind(side, pos) >> 5 != 0) {
            if (reaction != 2 && reaction != 5) {
                return 2;
            }
            return 0;
        }
        unitAp = func_001e0290_try(side, pos, 0);
        if (SzDuel_GetUnitDP(side, pos, 0) < unitAp) {
            if (isMove3 == 0) {
                if (reaction != 2 && reaction != 5) {
                    return 2;
                }
                return 0;
            }
        }
        if (reaction != 2 && reaction != 5) {
            return 2;
        }
        return 0;
    } else {
        if (SzDuel_GetUnitKind(side, pos) >> 5 != 0) {
            if (reaction != 2 && reaction != 5) {
                return 1;
            }
        }
        unitAp = func_001e0290_try(side, pos, 0);
        if (unitAp < SzDuel_GetUnitDP(side, pos, 0) || isMove3 == 1) {
            if (reaction != 2 && reaction != 5) {
                return 1;
            }
        }
        return 2;
    }
}
