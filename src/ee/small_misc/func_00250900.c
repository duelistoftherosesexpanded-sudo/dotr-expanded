#include "dotr/ee/small_misc.h"

int TaUtil_IsVarInfoLeader(struct DotrMiscFlagObject *unit)
{
    if (unit->unknown_45[0] == 0 && (unit->team == 0 || unit->team == 1)) {
        return 1;
    }
    return 0;
}
