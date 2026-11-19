#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

int SzDuel_SetField(int x, int y, int value)
{
    struct DotrStateCell *square = cell(x, y);
    if (square == 0) {
        return 0;
    }
    square->field = value;
    return 1;
}
