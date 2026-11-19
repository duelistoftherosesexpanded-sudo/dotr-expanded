#include "dotr/ee/pick_copies.h"

void TaLogic006_InitRogicControl(struct DotrPickMask *p, unsigned char n)
{
    if (n < 2) {
        if (n == 1) {
            p->half_4 = (p->half_4 & 0xff) | 0xff00;
        } else {
            p->half_4 = (p->half_4 & 0xff00) | 0xff;
        }
        if (n == 1) {
            p->goalLoc1 = (p->goalLoc1 & 0xffff) | 0xffff0000;
        } else {
            p->goalLoc1 = (p->goalLoc1 & 0xffff0000) | 0xffff;
        }
    }
}
