#include "dotr/ee/view_state.h"

int RhView_GetSampleZ(struct DotrViewVector *in, float dz)
{
    unsigned char raw[0x20];
    struct DotrViewVector *v;
    struct DotrViewEntry *e;
    e = &gRhView_avl[gRhView_nStackIndex[0]];
    v = (struct DotrViewVector *)(((DotrViewWord)raw + 15) & -16);
    *v = *in;
    v->z = v->z - dz;
    RhGeom_MulMatrixVector(v, &e->unknown_080, v);
    return func_001a23b0(v->z / v->unknown_w);
}
