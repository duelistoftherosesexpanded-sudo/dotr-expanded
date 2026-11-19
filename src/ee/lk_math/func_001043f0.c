#include "dotr/ee/lk_math.h"

int LkGetRandVec(struct DotrLkVec *out, float len)
{
    float m[16];
    struct DotrLkVec v = data_0028c220_lk;
    int rot[4];
    v.y = len;
    rot[0] = func_0019a5c8() % 0x10000;
    rot[1] = func_0019a5c8() % 0x10000;
    rot[2] = 0;
    RhGeom_GetRotXYZMatrix(m, rot);
    RhGeom_MulMatrixVector(out, m, &v);
    return 0;
}
