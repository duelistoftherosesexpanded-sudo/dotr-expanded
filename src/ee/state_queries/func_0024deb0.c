#include "dotr/ee/state_queries.h"

/* Forwards to Do_CheckLandAgreement unless the item's upper three bits at +0x3e are set
 * or its byte at +0x45 is zero. */
int CheckLandAgreement(int field, struct DotrStateItem *unit)
{
    if ((unit->kind >> 5) != 0 || unit->slot == 0) {
        return 0;
    }
    return Do_CheckLandAgreement(field, unit);
}
