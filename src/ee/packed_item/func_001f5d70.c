#include "dotr/ee/packed_item.h"

int SzEffect_GetCardScale2(int packed, struct DotrStateVector *v)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    v->z = 0.0f;
    v->y = 0.0f;
    v->x = 0.0f;
    v->unknown_w = 1.0f;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    return SzDuel_GetCardScale(row, column, v);
}
