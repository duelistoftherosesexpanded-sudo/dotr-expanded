#include "dotr/ee/view_state.h"

float RhGeom_GetPointsDistance(struct DotrViewVector *a, struct DotrViewVector *b)
{
    float x = a->x - b->x;
    float y = a->y - b->y;
    float z = a->z - b->z;
    return func_001a6c60(x * x + y * y + z * z);
}
