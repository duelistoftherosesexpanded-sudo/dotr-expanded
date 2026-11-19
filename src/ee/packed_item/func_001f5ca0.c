#include "dotr/ee/packed_item.h"

int SzEffect_GetCardRot2(int packed, struct DotrStateVector *v)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    v->unknown_w = 0.0f;
    v->z = 0.0f;
    v->y = 0.0f;
    v->x = 0.0f;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    return SzDuel_GetCardRot(row, column, v);
}
