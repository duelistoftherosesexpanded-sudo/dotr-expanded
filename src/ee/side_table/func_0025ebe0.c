#include "dotr/ee/small_misc.h"

/* With k = Ta_SearchMoveTemp() non-negative: reads the cells of (a, b) and
 * (c, d), then writes k to (a, b), the old (a, b) cell to (c, d) and the
 * old (c, d) cell to (a, b) through SzDuel_MoveUnit (low byte, upper bits). */
void Ta_SwapUnit(int a, int b, int c, int d)
{
    int w1;
    int w2;
    int k;
    k = Ta_SearchMoveTemp();
    if (k < 0) {
        return;
    }
    w1 = SzDuel_GetUnitLoc(a, b);
    w2 = SzDuel_GetUnitLoc(c, d);
    SzDuel_MoveUnit(a, b, k & 0xff, k >> 8);
    SzDuel_MoveUnit(c, d, w1 & 0xff, w1 >> 8);
    SzDuel_MoveUnit(a, b, w2 & 0xff, w2 >> 8);
}
