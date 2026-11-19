#include "dotr/ee/view_state.h"
float Ex3d_GetSampleW(struct DotrViewVector *out, float dz)
{
    struct DotrViewVector tmp;
    struct DotrViewMatrix m1;
    struct DotrViewMatrix m2;

    RhView_GetWorldViewMatrix(&m1);
    RhView_GetViewScreenMatrix(&m2);
    out->unknown_w = 1.0f;
    RhGeom_MulMatrixVector(&tmp, &m1, out);
    tmp.z -= dz;
    RhGeom_MulMatrixVector(&tmp, &m2, &tmp);
    return tmp.unknown_w;
}
