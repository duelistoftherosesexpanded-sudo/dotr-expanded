#include "dotr/ee/state_queries.h"

int TaUtil_GetUnitDP(int side, int pos, int col, int row)
{
    struct DotrStateItem *unit = SzDuel_GetUnit(side, pos);
    int extra = Ta_GetParamOfs(unit, col, row, 0);
    int base = SzDuel_GetUnitDP(side, pos, 0);
    return GetParCap(base + extra);
}
