#include "dotr/ee/view_state.h"
void Ex3d_GetPointOnScreen(struct DotrViewVector *out, float x, float y)
{
    struct DotrViewMatrix m;

    out->x = x;
    out->y = y;
    out->z = SzView_GetScreenDistance(0);
    out->unknown_w = 1.0f;
    RhView_GetWorldViewMatrix(&m);
    RhGeom_InvertMatrix(&m, &m);
    RhGeom_MulMatrixVector(out, &m, out);
}
