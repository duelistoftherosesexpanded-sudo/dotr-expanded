#include "dotr/ee/state_grid.h"

/* Operation: 0 add, 1 subtract, 2 multiply, 3 divide, 4 set. Operation 3 divides
 * without a zero check; the original has none either. */
int SzDuel_CalcUnitAP2(struct DotrStateItem *unit, int operation, int value)
{
    int oldBoost;
    int r;

    if (unit == 0) {
        return 0;
    }
    oldBoost = unit->apBoost;
    switch (operation) {
    case 0:
        r = oldBoost + value;
        break;
    case 1:
        r = oldBoost - value;
        break;
    case 2:
        r = oldBoost * value;
        break;
    case 3:
        r = oldBoost / value;
        break;
    case 4:
        r = value;
        break;
    default:
        return 0;
    }
    unit->apBoost = DOTR_STATE_MIN(DOTR_STATE_MAX(r, -9999), 9999);
    return 1;
}
