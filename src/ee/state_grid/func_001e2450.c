#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

int SzDuel_GetFieldRate(int x, int y)
{
    struct DotrStateCell *c = cell(x, y);
    if (c == 0) {
        return 0;
    }
    return c->fieldRate;
}
