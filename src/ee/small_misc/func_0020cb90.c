#include "dotr/ee/small_misc.h"
void RhGeom_RotXYZMatrix(struct DotrMiscMatrix *out, int *ang)
{
    struct DotrMiscMatrix m;

    RhGeom_GetRotXYZMatrix((float *)&m, ang);
    RhGeom_MulMatrix(out, &m);
}
