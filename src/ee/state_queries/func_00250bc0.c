#include "dotr/ee/state_queries.h"

int TaUtil_GetUnitDP2(struct DotrStateItem *unit, int col, int row)
{
    int base = SzDuel_GetUnitDP2(unit, 0);
    return GetParCap(base + Ta_GetParamOfs(unit, col, row, 0));
}
