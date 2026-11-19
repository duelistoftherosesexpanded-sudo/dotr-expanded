#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

int SzDuel_SetMapAlpha(int x, int y, int alpha)
{
    struct DotrStateCell *mapInfo = cell(x, y);
    if (mapInfo == 0) {
        return 0;
    }
    mapInfo->mapAlpha = alpha;
    return 1;
}
