#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitDP2(struct DotrStateItem *unit, int ignoreBoosts)
{
    int v;
    if (unit == 0) {
        return -1;
    }
    if (unit->constInfo == 0) {
        return -1;
    }
    v = unit->constInfo->dpWithFlags & 0x1fff;
    if (ignoreBoosts == 0) {
        v += unit->dpBoost;
    }
    return DOTR_STATE_MIN(DOTR_STATE_MAX(v, 0), 9999);
}
