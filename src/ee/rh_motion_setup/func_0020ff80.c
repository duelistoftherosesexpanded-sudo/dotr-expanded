#include "dotr/ee/rh_motion_setup.h"

/* 0x20ff80. C with an inline hardware block (the stack-pointer switch to the scratchpad stack at
 * 0x70004000 and back, which has no C spelling). The host build runs the walk on its own stack
 * (no switch). */

int lRhModel_SetupNoMotion(void *arg0) {
    struct DotrMotionNoSpr *spr = (struct DotrMotionNoSpr *)DOTR_MOTION_SPR(0);
    unsigned char *data = (unsigned char *)arg0 + *(int *)((unsigned char *)arg0 + 0x10);

    if (spr->dst != NULL) {
        *spr->dst = *(struct DotrMotionVec4 *)(data + 0x20);
    }
    spr->data = data;
#ifdef __MWERKS__
    asm {
        la v0, gRhModel_dwStack
        sw sp, 0(v0)
        la at, data_70004000_rhmotion
        addiu sp, at, 0
    }
#else
    /* host: no stack switch */
#endif
    lRhModel_SetupNoMotionRecursive((void *)DOTR_MOTION_SPR(0), data, gRhModel_vecScale);
#ifdef __MWERKS__
    asm {
        la v0, gRhModel_dwStack
        lw sp, 0(v0)
    }
#else
    /* host: no stack switch */
#endif
    return 1;
}
