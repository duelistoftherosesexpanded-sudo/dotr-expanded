#include "dotr/ee/small_misc.h"

int Ex3d_GetPointsAverage(int count, struct DotrMiscVector4 *items, struct DotrMiscVector4 *out)
{
    int i;
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    out->unknown_w = 0.0f;
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        out->x += items->x;
        out->y += items->y;
        out->z += items->z;
        out->unknown_w += items->unknown_w;
        items++;
    }
    out->x = out->x / (float)count;
    out->y = out->y / (float)count;
    out->z = out->z / (float)count;
    out->unknown_w = out->unknown_w / (float)count;
    return 1;
}
