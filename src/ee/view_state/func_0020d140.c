#include "dotr/ee/view_state.h"

void RhGeom_OuterProduct(struct DotrViewVector *out, struct DotrViewVector *a, struct DotrViewVector *b)
{
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
    out->unknown_w = a->unknown_w;
}
