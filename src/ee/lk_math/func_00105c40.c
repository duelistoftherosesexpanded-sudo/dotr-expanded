#include "dotr/ee/lk_math.h"

int LkAssignFrameST(float *p)
{
    float s;
    float a[2];
    s = (0.0f == p[0]) ? 0.0f : p[0] / p[2];
    a[1] = 1.0f / p[3];
    a[0] = (0.0f == p[1]) ? 0.0f : p[1] / p[3];
    s = (float)func_001a22c0(s);
    return func_001a22c0(a[0] + s * a[1]);
}
