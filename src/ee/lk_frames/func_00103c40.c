#include "dotr/ee/lk_frames.h"

int LkVec2Mat(struct DotrLkVector *v, float *m)
{
    struct DotrLkVector dir;
    struct DotrLkVector up;
    struct DotrLkVector axis;

    dir = *v;
    RhGeom_NormalizeVector(&dir);
    up.z = 0.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    RhGeom_NormalizeVector(&up);
    RhGeom_OuterProduct(&axis, &up, &dir);
    RhGeom_GetRotVectorMatrix(m, &axis, func_001a22c0(func_001a6c00(RhGeom_GetVectorScalor(&axis)) / 6.2831855f * 65536.0f));
    return 0;
}
