#include "dotr/ee/lk_frames.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* LkVec2RotXY (0x103b50). Compiled warm, with the float-to-int conversions written as casts. */
int LkVec2RotXY(float *v, int *rot)
{
    rot[1] = (int)(65536.0f * (func_001a6c18(v[0], v[2]) / 6.2831855f)) % 65536;
    rot[0] = (int)(65536.0f * (func_001a6c18(func_001a6c60(v[0] * v[0] + v[2] * v[2]), v[1]) / 6.2831855f)) % 65536;
    rot[3] = 0;
    rot[2] = 0;
    return 0;
}
