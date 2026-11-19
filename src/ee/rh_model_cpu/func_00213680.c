#include "dotr/ee/rh_model_cpu.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */
/* 0x213680. C with inline VU0 blocks. Compiled warm (see its build entry). Address words are
 * DotrCpuWord. The host build replaces each block with its plain-C effect (rh_model_cpu.h). */

#define OUT_00213680 ((struct DotrCpuPointOut *)arg0)
#define IN_00213680 ((struct DotrCpuIndepIn *)arg1)

signed char lRhModelCpu_CalcIndepPoint(DotrCpuWord arg0, int *arg1) {
    struct DotrCpuPointSpr *spr = (struct DotrCpuPointSpr *)DOTR_CPU_SPR(0);
    register float *src = spr->in;
    register float *dst = spr->res;
    float w;
    float fog;

    OUT_00213680->color = spr->f100;
    spr->in[0] = IN_00213680->x;
    spr->in[1] = IN_00213680->y;
    spr->in[2] = IN_00213680->z;
#ifdef __MWERKS__
    asm {
        lqc2 vf1, 0x0(src)
        vmulax.xyzw ACC, vf16, vf1x
        vmadday.xyzw ACC, vf17, vf1y
        vmaddaz.xyzw ACC, vf18, vf1z
        vmaddw.xyzw vf1, vf19, vf1w
        vdiv Q, vf0w, vf1w
        vwaitq
        vmulq.xyz vf1, vf1, Q
        sqc2 vf1, 0x0(dst)
    }
#else
    dotr_rh_model_cpu_load(1, src);
    dotr_rh_model_cpu_xform(1, 16, 1);
    dotr_rh_model_cpu_project(1);
    dotr_rh_model_cpu_store(dst, 1);
#endif
    w = spr->res[3];
    if (w < 1.0f || !(w < spr->fF8) || spr->res[0] <= 1.0f || !(spr->res[0] < 4095.0f) || spr->res[1] <= 1.0f ||
        !(spr->res[1] < 4095.0f)) {
        return 0;
    }
    OUT_00213680->x = (unsigned int)(16.0f * spr->res[0]);
    OUT_00213680->y = (unsigned int)(16.0f * spr->res[1]);
    OUT_00213680->z = (unsigned int)(spr->res[2]);
    fog = spr->f110 - spr->f114 * spr->res[3];
    if (fog <= 0.0f) {
        OUT_00213680->fog = 0;
    } else if (!(fog < 255.0f)) {
        OUT_00213680->fog = 0xFF;
    } else {
        OUT_00213680->fog = (unsigned int)(fog);
    }
    OUT_00213680->s = IN_00213680->u / spr->res[3] / 4096.0f;
    OUT_00213680->t = IN_00213680->v / spr->res[3] / 4096.0f;
    OUT_00213680->q = 1.0f / spr->res[3];
    return 1;
}

#undef OUT_00213680
#undef IN_00213680
