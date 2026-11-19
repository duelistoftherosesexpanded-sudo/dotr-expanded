#include "dotr/ee/pick_copies.h"

void TaLogic005_InitRogicControl(struct DotrPickMask *task, unsigned char type)
{
    if (type < 2) {
        if (type == 1) {
            task->half_4 = (task->half_4 & 0xff) | 0xff00;
        } else {
            task->half_4 = (task->half_4 & 0xff00) | 0xff;
        }
        if (type == 1) {
            task->goalLoc1 = (task->goalLoc1 & 0xffff) | 0xffff0000;
        } else {
            task->goalLoc1 = (task->goalLoc1 & 0xffff0000) | 0xffff;
        }
    }
}
