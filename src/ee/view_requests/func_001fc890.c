#include "dotr/ee/view_requests.h"

/* 0x1fc890. Compiled warm (see its build entry). */
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

static inline int toAngle_001FC890(float radians)
{
    return (int)(32768.0f * radians / 3.1415927f);
}

void SzView_V2IV(float *src, int *dst)
{
    dst[0] = (int)src[0];
    dst[1] = toAngle_001FC890(src[1]);
    dst[2] = toAngle_001FC890(src[2]);
    dst[3] = (int)src[3];
}
