#include "dotr/ee/state_grid.h"

void TaTuto_DrawFinger(struct DotrGridTicker *p, unsigned char period, int a, int b, int c)
{
    if (p->tick % period == 0) {
        SzSound_PlaySE(0, 19);
    }
    if (p->tick++ % period < period / 2) {
        SzDuel_OpenHandCursor(a, b, c);
    } else {
        SzDuel_CloseHandCursor();
    }
    if (p->tick > period) {
        p->tick = 0;
    }
}
