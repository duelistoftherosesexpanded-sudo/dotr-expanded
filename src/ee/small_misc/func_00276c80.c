#include "dotr/ee/small_misc.h"

int TaLogic_GetActionUnitFromOwnerID(struct DotrMiscEntry16 *actionUnits, int ownerId)
{
    int i;
    for (i = 0; i < 22; i++, actionUnits++) {
        if (ownerId == actionUnits->unknown_00) {
            return i;
        }
    }
    return -1;
}
