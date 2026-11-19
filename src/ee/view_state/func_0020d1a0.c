#include "dotr/ee/view_state.h"

float RhGeom_InnerProduct(struct DotrViewVector *a, struct DotrViewVector *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}
