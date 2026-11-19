#include "dotr/ee/state_grid.h"

struct DotrStateCell *SzDuel_GetMapInfo(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}
