#include "dotr/ee/view_state.h"

void RhGeom_GetTransMatrix(struct DotrViewMatrix *matrix, struct DotrViewVector *v)
{
    RhGeom_UnitMatrix(matrix);
    matrix->unknown_values[12] = v->x;
    matrix->unknown_values[13] = v->y;
    matrix->unknown_values[14] = v->z;
}
