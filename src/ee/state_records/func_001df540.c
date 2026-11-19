#include "dotr/ee/state_records.h"

struct DotrStateItem *SzDuel_GetUnit(int side, int pos)
{
    struct DotrStateItem *unit = SzDuel_GetCardInfo(side, pos);
    if (unit == 0) {
        return 0;
    }
    if (unit->slot >= 11) {
        return 0;
    }
    return unit;
}
