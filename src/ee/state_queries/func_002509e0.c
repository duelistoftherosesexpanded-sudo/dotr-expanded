#include "dotr/ee/state_queries.h"

int TaUtil_GetUnitAP(int side, int pos, int col, int row)
{
    struct DotrStateItem *unit = SzDuel_GetUnit(side, pos);
    int extra = Ta_GetParamOfs(unit, col, row, 1);
    int base = SzDuel_GetUnitAP(side, pos, 0);
    return GetParCap(base + extra);
}
