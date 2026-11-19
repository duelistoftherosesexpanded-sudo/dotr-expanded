#include "dotr/ee/state_queries.h"

/* Item query result minus `base`; on a cell of kind 9 the base is first
 * limited by GetCap(1500, base). */
int TaLogic_IsUnitWin3(struct DotrStateItem *unit, int base, int x, int y)
{
    int value;
    if (unit == 0) {
        return 0;
    }
    if (func_00250900_item(unit) == 1) {
        value = 0;
    } else if (unit->locCol == x && unit->locRow == y && unit->status.unknown_bit_3) {
        value = TaUtil_GetUnitDP2(unit, x, y);
    } else {
        value = TaUtil_GetUnitAP2(unit, x, y);
    }
    if (SzDuel_GetField(x, y) == 9) {
        base = GetCap(1500, base);
    }
    return value - base;
}
