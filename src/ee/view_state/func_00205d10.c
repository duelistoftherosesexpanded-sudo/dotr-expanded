#include "dotr/ee/view_state.h"
int Ex3d_GetSampleZ(struct DotrViewVector *out, float dz)
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
    if (tmp.unknown_w <= 0.0f) {
        return 0xfffff6;
    }
    if (tmp.z <= 0.0f) {
        return 0;
    }
    return func_001a23b0(tmp.z / tmp.unknown_w);
}
