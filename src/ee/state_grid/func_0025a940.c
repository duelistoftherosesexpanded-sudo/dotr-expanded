#include "dotr/ee/state_grid.h"

/* Slides o->pos by 30 per call toward 480 (mode 1) or toward 0/960 by dir (other modes);
 * returns 1 when it has arrived. */
int TaEff_PopHand(int unused, struct DotrSlideObject *o, unsigned char dir, int x, unsigned char mode)
{
    int done = 0;
    (void)unused;
    if (o->pos == 0) {
        if (o->counter++ == 0) {
            o->pos = dir == 1 ? 0 : 960;
        }
    }
    SzDuel_OpenHandPopup(x, 0, o->pos - 480);
    if (mode == 1) {
        if (o->pos != 480) {
            o->pos += dir == 1 ? 30 : -30;
        } else {
            done = 1;
        }
    } else {
        if (o->pos != (dir == 1 ? 0 : 960)) {
            o->pos -= dir == 1 ? 30 : -30;
        } else {
            done = 1;
        }
    }
    return done;
}
