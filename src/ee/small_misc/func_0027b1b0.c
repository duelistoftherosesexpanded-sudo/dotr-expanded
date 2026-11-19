#include "dotr/ee/small_misc.h"

void TaLogic_InitRogicControl(struct DotrMiscWordAt8 *unit, unsigned char goalId)
{
    if (goalId < 2) {
        if (goalId == 1) {
            unit->goalLoc1 = (unsigned short)unit->goalLoc1 | 0xffff0000;
        } else {
            unit->goalLoc1 = (unit->goalLoc1 & 0xffff0000) | 0xffff;
        }
    }
}
