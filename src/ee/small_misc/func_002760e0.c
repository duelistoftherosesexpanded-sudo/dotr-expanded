#include "dotr/ee/small_misc.h"

int TaLogic_IsUnitAssign(unsigned short ownerId, struct DotrMiscEntry16 *p)
{
    int i;
    for (i = 0; i < 22; i++, p++) {
        if (ownerId == p->unknown_00) {
            return 1;
        }
    }
    return 0;
}
