#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

/* The inlined lookup can return null; the original dereferences it
 * regardless. The loop bounds keep the lookup in range. */
void SzDuel_DoUnitRequest(void)
{
    int x, y;
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            struct DotrStateCell *c = cell(x, y);
            lSzDuel_DoUnitRequest(c, c->unit);
        }
    }
}
