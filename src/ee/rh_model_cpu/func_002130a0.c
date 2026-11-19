#include "dotr/ee/rh_model_cpu.h"

/* 0x2130a0. C with inline VU0 blocks. Address words are DotrCpuWord. The host build replaces each
 * block with its plain-C effect (rh_model_cpu.h). */

int lRhModelCpu_SetupGlobal(int arg0) {
    struct DotrCpuGlobalSpr *spr = (struct DotrCpuGlobalSpr *)DOTR_CPU_SPR(0);
    unsigned char e[4];
    float near;
    float far;
    float c;
    float d;

    RhView_GetViewScreenMatrix(spr);
    {
        register unsigned char *m = (unsigned char *)DOTR_CPU_SPR(0);
#ifdef __MWERKS__
        asm {
            lqc2 vf20, 0x0(m)
            lqc2 vf21, 0x10(m)
            lqc2 vf22, 0x20(m)
            lqc2 vf23, 0x30(m)
        }
#else
        dotr_rh_model_cpu_load(20, (float *)m);
        dotr_rh_model_cpu_load(21, (float *)(m + 0x10));
        dotr_rh_model_cpu_load(22, (float *)(m + 0x20));
        dotr_rh_model_cpu_load(23, (float *)(m + 0x30));
#endif
    }
    spr->tag[0] = 0x10000000;
    spr->tag[1] = 0;
    spr->tag[2] = 0;
    spr->tag[3] = 0;
    spr->tag[4] = 0x10000006;
    spr->tag[5] = 0;
    spr->tag[6] = 0;
    spr->tag[7] = 0x51000006;
    spr->f80 = 0x3400000000008003;
    spr->f88 = 0x412;
    RhView_GetClipZ(&spr->fF4, &spr->fF8);
    spr->f104 = func_00186120(8);
    spr->f5C = 1.0f;
    RhView_GetFogParameter(&near, &far, &c, &d, e);
    if (arg0 != 0) {
        spr->f110 = 255.0f * far / (far - near);
        spr->f114 = 255.0f / (far - near);
    } else {
        spr->f110 = 255.0f;
        spr->f114 = 0.0f;
    }
    return 1;
}
