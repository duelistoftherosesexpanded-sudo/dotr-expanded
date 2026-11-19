#include "dotr/ee/rh_motion_setup.h"

/* 0x210260. C with an inline hardware block (the stack-pointer switch to the scratchpad stack at
 * 0x70004000 and back, which has no C spelling). The host build runs the walk on its own stack
 * (no switch). */

int lRhModel_SetupMultiMotion(void *arg0, void *arg1, void *arg2, float frame, float frame2, float value, float value2) {
    struct DotrMotionMultiSpr *spr = (struct DotrMotionMultiSpr *)DOTR_MOTION_SPR(0);
    struct DotrMotionMulti *m1 = arg1;
    struct DotrMotionMulti *m2 = arg2;
    int c1;
    int c2;
    unsigned char *data = (unsigned char *)arg0 + *(int *)((unsigned char *)arg0 + 0x10);

    spr->data = data;
    spr->motion94 = *m1->motion;
    spr->frame98 = frame;
    c1 = m1->count;
    c2 = m2->count;
    spr->count9C = (c1 < c2) ? c1 : c2;
    spr->motionA0 = *m2->motion;
    spr->frameA4 = frame2;
    spr->valueA8 = value;
    spr->valueAC = value2;
    spr->countE0 = ((unsigned short *)m1->motion)[4];
    {
    register void **save = &gRhModel_dwStack;
#ifdef __MWERKS__
    asm {
        sw sp, 0(save)
        la at, data_70004000_rhmotion
        addiu sp, at, 0
    }
#else
    (void)save; /* host: no stack switch */
#endif
    }
    lRhModel_SetupMultiMotionRecursive(data, 0, gRhModel_vecScale);
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
