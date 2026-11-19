#include "dotr/ee/state_queries.h"

/* Same gates as Ta_IsVirus_Chk2 (0x24fdd0), but the call to TaUtil_GetUnitAP2 sets only the
 * first argument register; the other two keep this routine's incoming values. */
int Ta_IsVirus_Chk3(struct DotrStateItem *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (unit->kind >> 5 != 0 || unit->slot == 0 || (unit->kind & 0x1f) == 20) {
        return 0;
    }
    if (func_00250a60_item(unit) < 1500) {
        return 0;
    }
    return 1;
}
