#include "dotr/ee/pick_copies.h"
void TaLogic003_InitRogicControl(struct DotrPickMask *p, unsigned char n)
{
    if (n < 2) {
        if (n == 1) {
            p->goalLoc1 = (p->goalLoc1 & 0xffff) | 0xffff0000;
        } else {
            p->goalLoc1 = (p->goalLoc1 & 0xffff0000) | 0xffff;
        }
    }
}
