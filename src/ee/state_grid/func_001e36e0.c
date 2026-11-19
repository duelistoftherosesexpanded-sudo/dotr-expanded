#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    struct DotrStateCell *square = cell(x, y);
    struct DotrStateItem *unit;
    if (square == 0) {
        return 0;
    }
    unit = (struct DotrStateItem *)square->unit;
    if (unit == 0) {
        return 0;
    }
    if (unit->slot >= 11) {
        return 0;
    }
    return unit;
}
