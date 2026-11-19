#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

int SzDuel_SetFieldRate(int x, int y, int value)
{
    struct DotrStateCell *square = cell(x, y);
    if (square == 0) {
        return -1;
    }
    if (value < -128) {
        value = -128;
    }
    if (value > 128) {
        value = 128;
    }
    square->fieldRate = value;
    return value;
}
