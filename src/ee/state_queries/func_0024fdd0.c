#include "dotr/ee/state_queries.h"

int Ta_IsVirus_Chk2(struct DotrStateItem *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (unit->kind >> 5 != 0 || unit->slot == 0 || (unit->kind & 0x1f) == 20) {
        return 0;
    }
    if (TaUtil_GetUnitAP2(unit, unit->locCol, unit->locRow) < 1500) {
        return 0;
    }
    return 1;
}
