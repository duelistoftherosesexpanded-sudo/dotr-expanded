#include "dotr/ee/state_grid.h"

/* 0x1e27d0 lSzDuel_CalcMapMovable (name from the Japanese build's symbols). */
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

static inline int get_17(int x, int y)
{
    struct DotrStateCell *c = cell(x, y);
    if (c != 0) {
        return c->mapMovable;
    }
    return 255;
}

void lSzDuel_CalcMapMovable(int reset, int pass, int check, int side, int x0, int y0, int limit, unsigned char *xs,
                            unsigned char *ys, int n)
{
    int x;
    int y;
    int i;
    unsigned char work0[64];
    unsigned char work1[64];

    if (reset != 0) {
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                set_17(x, y, 255);
            }
        }
    } else {
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                if (get_17(x, y) < 255) {
                    set_17(x, y, 254);
                }
            }
        }
    }
    lSzDuel_CalcMapMovableRecursive(pass, check, side, x0, y0, limit, xs, ys, n, work0, work1, 0, 1);
    for (i = 0; i < n; i++) {
        set_17(xs[i], ys[i], i);
    }
}
