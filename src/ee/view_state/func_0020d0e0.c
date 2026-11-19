#include "dotr/ee/view_state.h"

float RhGeom_GetVectorScalor(struct DotrViewVector *vector)
{
    return func_001a6c60(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}
