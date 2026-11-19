#include "dotr/ee/view_state.h"

/* The last statement is a self-assignment that the original also performs. */
void RhGeom_NormalizeVector(struct DotrViewVector *v)
{
    float length = func_001a6c60(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x = v->x / length;
    v->y = v->y / length;
    v->z = v->z / length;
    v->unknown_w = v->unknown_w;
}
