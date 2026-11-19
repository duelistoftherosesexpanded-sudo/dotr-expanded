#include "dotr/ee/state_grid.h"

void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrStateVector *v)
{
    v->z = 0.0f;
    v->x = 0.0f;
    if (x >= 0 && x < 7) {
        v->x = (x - 3) * 50;
    }
    if (y >= 0 && y < 7) {
        v->z = (y - 3) * 50;
    }
    v->y = (float)(z * 35) / 128.0f;
    v->unknown_w = 1.0f;
}
