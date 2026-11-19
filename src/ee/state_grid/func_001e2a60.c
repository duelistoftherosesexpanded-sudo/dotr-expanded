#include "dotr/ee/state_grid.h"

/* 0x1e2a60 lSzDuel_CalcMapMovableRecursive (name from the Japanese build's symbols). The
 * range-checked cell bodies are the inlined ones of lSzDuel_CalcMapSummonEnable. */
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

static inline int get_17(int x, int y)
{
    struct DotrStateCell *c = cell(x, y);
    if (c != 0) {
        return c->mapMovable;
    }
    return 255;
}

void lSzDuel_CalcMapMovableRecursive(int pass, int check, int side, int x, int y, int limit, unsigned char *xs,
                                     unsigned char *ys, int n, unsigned char *px, unsigned char *py, int depth,
                                     int budget)
{
    struct DotrStateItem *item;
    int kind;
    int i;

    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return;
    }
    item = SzDuel_GetMapUnit(x, y);
    kind = get_16(x, y);
    if (depth > 0) {
        for (i = 0; i < n; i++) {
            if (xs[i] == x && ys[i] == y) {
                return;
            }
        }
        if (item != 0) {
            int owner = func_00250360_pair(item->team, item->slot);
            if (check != 0 && owner != side) {
                return;
            }
            if (item->slot == 0 && item->team == side) {
                return;
            }
        }
    }
    px[depth] = x;
    py[depth] = y;
    if (item != 0 || kind != 8 || pass != 0) {
        if (get_17(x, y) > n + depth) {
            set_17(x, y, n + depth);
        }
    }
    if ((item != 0 || (kind == 8 && pass == 0)) && n + depth > 0) {
        budget--;
        if (budget <= 0) {
            return;
        }
    }
    if (++depth > limit) {
        return;
    }
    for (i = 0; i < depth; i++) {
        if (px[i] == x + 1 && py[i] == y) {
            break;
        }
    }
    if (i >= depth) {
        lSzDuel_CalcMapMovableRecursive(pass, check, side, x + 1, y, limit, xs, ys, n, px, py, depth, budget);
    }
    for (i = 0; i < depth; i++) {
        if (px[i] == x - 1 && py[i] == y) {
            break;
        }
    }
    if (i >= depth) {
        lSzDuel_CalcMapMovableRecursive(pass, check, side, x - 1, y, limit, xs, ys, n, px, py, depth, budget);
    }
    for (i = 0; i < depth; i++) {
        if (px[i] == x && py[i] == y + 1) {
            break;
        }
    }
    if (i >= depth) {
        lSzDuel_CalcMapMovableRecursive(pass, check, side, x, y + 1, limit, xs, ys, n, px, py, depth, budget);
    }
    for (i = 0; i < depth; i++) {
        if (px[i] == x && py[i] == y - 1) {
            break;
        }
    }
    if (i >= depth) {
        lSzDuel_CalcMapMovableRecursive(pass, check, side, x, y - 1, limit, xs, ys, n, px, py, depth, budget);
    }
}
