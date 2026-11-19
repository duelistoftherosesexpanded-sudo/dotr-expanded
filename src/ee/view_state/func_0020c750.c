#include "dotr/ee/view_state.h"
void RhGeom_GetRotVectorMatrix(float *m, struct DotrViewVector *axis, int phase)
{
    struct DotrViewVector v;
    float c, s, t;
    v = *axis;
    c = RhMath_CosI(phase);
    t = 1.0f - c;
    s = RhMath_SinI(phase);
    RhGeom_NormalizeVector(&v);
    m[0] = c + t * (v.x * v.x);
    m[4] = t * (v.x * v.y) - v.z * s;
    m[8] = t * (v.x * v.z) + v.y * s;
    m[1] = t * (v.x * v.y) + v.z * s;
    m[5] = c + t * (v.y * v.y);
    m[9] = t * (v.y * v.z) - v.x * s;
    m[2] = t * (v.x * v.z) - v.y * s;
    m[6] = t * (v.y * v.z) + v.x * s;
    m[10] = c + t * (v.z * v.z);
    m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f;
    m[15] = 1.0f;
}
