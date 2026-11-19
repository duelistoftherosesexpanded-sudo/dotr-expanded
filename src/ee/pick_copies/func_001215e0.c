#include "dotr/ee/pick_copies.h"
void TaLogic004_InitRogicControl(struct DotrPickMask *unitTask, unsigned char n)
{
    if (n < 2) {
        if (n == 1) {
            unitTask->goalLoc1 = (unitTask->goalLoc1 & 0xffff) | 0xffff0000;
        } else {
            unitTask->goalLoc1 = (unitTask->goalLoc1 & 0xffff0000) | 0xffff;
        }
    }
}
