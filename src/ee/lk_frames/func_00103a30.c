#include "dotr/ee/lk_frames.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* LkVec2Rot (0x103a30). Compiled warm, with the float-to-int conversions written as casts. */
int LkVec2Rot(float *v, int *rot)
{
    rot[1] = (int)(65536.0f * (func_001a6c18(v[0], v[2]) / 6.2831855f)) % 65536;
    rot[0] = (int)(65536.0f * (func_001a6c18(v[2], v[1]) / 6.2831855f)) % 65536;
    rot[2] = (int)(65536.0f * (func_001a6c18(v[1], v[0]) / 6.2831855f)) % 65536;
    rot[3] = 0;
    return 0;
}
