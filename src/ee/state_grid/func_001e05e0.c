#include "dotr/ee/state_grid.h"

/* Operation: 0 add, 1 subtract, 2 multiply, 3 divide, 4 set. Operation 3 divides
 * without a zero check; the original has none either. */
int SzDuel_CalcUnitDP2(struct DotrStateItem *item, int operation, int value)
{
    int n;
    int r;

    if (item == 0) {
        return 0;
    }
    n = item->dpBoost;
    switch (operation) {
    case 0:
        r = n + value;
        break;
    case 1:
        r = n - value;
        break;
    case 2:
        r = n * value;
        break;
    case 3:
        r = n / value;
        break;
    case 4:
        r = value;
        break;
    default:
        return 0;
    }
    item->dpBoost = DOTR_STATE_MIN(DOTR_STATE_MAX(r, -9999), 9999);
    return 1;
}
