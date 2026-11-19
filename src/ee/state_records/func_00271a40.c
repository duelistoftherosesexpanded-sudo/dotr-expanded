#include "dotr/ee/state_queries.h"

/* Looks up the packed cell of side's leader through SzDuel_GetUnitLoc(side, 0) and
 * returns whether Ta_IsSquareRange(low byte, upper bits, col, row, mode) is non-zero;
 * mode is 5 when Ta_IsLDAbl(9, side) is non-zero (ability 9: extended support range),
 * else 3. */
int Ta_IsLDRange(int side, int col, int row)
{
    int loc = SzDuel_GetUnitLoc(side, 0);
    if (Ta_IsLDAbl(9, side) != 0) {
        return Ta_IsSquareRange(loc & 0xff, loc >> 8, col, row, 5) ? 1 : 0;
    }
    return Ta_IsSquareRange(loc & 0xff, loc >> 8, col, row, 3) ? 1 : 0;
}
