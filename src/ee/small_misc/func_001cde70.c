#include "dotr/ee/small_misc.h"

struct DotrMiscEntry160 *SzDuel_GetCardControl(int index)
{
    if (index < 0 || index >= 6) {
        return 0;
    }
    return &gSzDuel_aCardControl[index];
}
