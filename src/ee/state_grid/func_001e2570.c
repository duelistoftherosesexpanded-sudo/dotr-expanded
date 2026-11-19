#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

static inline void set_17(int x, int y, int value)
{
    struct DotrStateCell *c = cell(x, y);
    if (c != 0) {
        c->mapMovable = value;
    }
}

static inline int get_16(int x, int y)
{
    struct DotrStateCell *c = cell(x, y);
    if (c != 0) {
        return c->field;
    }
    return -1;
}

void lSzDuel_CalcMapSummonEnable(int side, int cx, int cy)
{
    int x;
    int y;
    int ok;
    int n;
    struct DotrStateItem *item;

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            set_17(x, y, 0xff);
        }
    }
    for (y = cy - 1; y <= cy + 1; y++) {
        for (x = cx - 1; x <= cx + 1; x++) {
            if (cx == x && cy == y) {
                continue;
            }
            item = SzDuel_GetMapUnit(x, y);
            ok = 1;
            if (get_16(x, y) == 8) {
                ok = 0;
            }
            if (item != 0) {
                n = func_00250360_pair(item->team, item->slot);
                if (n != side) {
                    ok = 0;
                }
            }
            if (ok) {
                set_17(x, y, 0);
            }
        }
    }
}
