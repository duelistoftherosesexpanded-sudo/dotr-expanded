#include "dotr/ee/small_misc.h"

int Ta_IsUnitControl2(struct DotrMiscFlagObject *unit)
{
    if (unit == 0) {
        return -1;
    }
    if (!unit->status.unknown_bit_1) {
        return unit->team;
    }
    return unit->team == 0 ? 1 : 0;
}
