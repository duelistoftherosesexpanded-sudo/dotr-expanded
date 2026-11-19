#include "dotr/ee/small_misc.h"
void RhGeom_RotVectorMatrix(struct DotrMiscMatrix *out, float *axis, int phase)
{
    struct DotrMiscMatrix m;

    RhGeom_GetRotVectorMatrix((float *)&m, axis, phase);
    RhGeom_MulMatrix(out, &m);
}
