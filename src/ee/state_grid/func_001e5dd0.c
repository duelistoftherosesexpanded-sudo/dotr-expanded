#include "dotr/ee/state_grid.h"

static inline int place(int cursor, int kind)
{
    return lSzBattle_DrawFakeShadow(cursor, kind, -1, -25.0f, 800.0f);
}

int lSzBattle_DrawGrass_Pipe(int cursor, int index, int skip)
{
    int i;
    int *p = gSzBattle_bl;
    int r;
    r = SzModel_DrawWithOutCalc_Pipe(cursor, index, 0, 0, 0, 0, 0);
    if (skip == 0) {
        r = lSzBattle_DrawFakeShadow(r, 7, -1, -25.0f, 800.0f);
        for (i = 0; i < 2; i++, p++) {
            int w = *p;
            if (w >= 0) {
                struct DotrStateItem *item = SzDuel_GetUnit(w >> 8, w & 0xff);
                if (item != 0) {
                    r = place(r, item->modelId);
                }
            }
        }
    }
    return r;
}
