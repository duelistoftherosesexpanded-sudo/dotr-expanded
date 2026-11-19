#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

int SzDuel_GetUnitPos(int a, int b, float *out)
{
    struct DotrStateItem *q = dotr_state_item_checked(a, b);
    int h;
    int y;
    int x;

    if (q == 0) {
        return 0;
    }
    x = q->locCol;
    y = q->locRow;
    h = cell(x, y)->fieldRate;
    out[2] = 0.0f;
    out[0] = 0.0f;
    if (x >= 0 && x < 7) {
        out[0] = (x - 3) * 50;
    }
    if (y >= 0 && y < 7) {
        out[2] = (y - 3) * 50;
    }
    out[1] = (float)(h * 35) / 128.0f;
    out[3] = 1.0f;
    out[0] += ((short *)q->unknown_34)[0];
    out[2] += ((short *)q->unknown_34)[1];
    return 1;
}
