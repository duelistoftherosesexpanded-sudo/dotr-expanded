#include "dotr/ee/state_grid.h"

void TaTuto_DrawTrapArea(struct DotrGridTicker2 *p, unsigned char period, int x, int y)
{
    unsigned char col[4];
    int i;
    int j;
    unsigned char *c;
    col[2] = 192;
    col[1] = 0;
    col[0] = 192;
    col[3] = 128;
    c = col;
    if (p->tick++ % period == 0) {
        for (i = 0; i <= 6; i++) {
            for (j = 0; j <= 6; j++) {
                if (((x > i ? x : i) - (x < i ? x : i)) + ((y > j ? y : j) - (y < j ? y : j)) < 2) {
                    SzDuel_SetMapColor(i, j, 0, (DotrGridWord)c, 1);
                }
            }
        }
    }
    if (p->tick > period) {
        p->tick = 0;
    }
}
