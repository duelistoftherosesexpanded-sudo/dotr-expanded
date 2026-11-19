#include "dotr/ee/rh_motion_setup.h"

/* 0x2114b0. C with an inline hardware block (the stack-pointer switch to the scratchpad stack at
 * 0x70004000 and back, which has no C spelling). The host build runs the walk on its own stack
 * (no switch). */

int lRhModel_SetupPulseMotion(unsigned char *model, unsigned char *motion, float time) {
    unsigned char *spr = (unsigned char *)DOTR_MOTION_SPR(0);
    register void **saved;

    model += *(int *)(model + 0x10);
    *(unsigned char **)(spr + 0x90) = model;
    *(int *)(spr + 0x94) = **(int **)(motion + 8);
    *(float *)(spr + 0x98) = time;
    *(int *)(spr + 0x9C) = *(unsigned short *)(motion + 4);
    *(unsigned short *)(spr + 0xE0) = *(unsigned short *)(*(unsigned char **)(motion + 8) + 8);
    saved = &gRhModel_dwStack;
#ifdef __MWERKS__
    asm {
        sw sp, 0x0(saved)
        la at, data_70004000_rhmotion
        addiu sp, at, 0x0
    }
#else
    (void)saved; /* host: no stack switch */
#endif
    lRhModel_SetupPulseMotionRecursive(model, 0, gRhModel_vecScale);
    {
        register void **back = &gRhModel_dwStack;
#ifdef __MWERKS__
        asm {
            lw sp, 0x0(back)
        }
#else
        (void)back;
#endif
    }
    return 1;
}
