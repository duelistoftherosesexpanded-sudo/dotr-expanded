#include "dotr/ee/state_queries.h"

int TaUtil_GetUnitAP2(struct DotrStateItem *unit, int col, int row)
{
    int base = SzDuel_GetUnitAP2(unit, 0);
    return GetParCap(base + Ta_GetParamOfs(unit, col, row, 1));
}
