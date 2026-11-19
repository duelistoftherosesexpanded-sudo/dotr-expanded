#include "dotr/ee/rh_model_cpu.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */
/* 0x213a40. C with inline VU0 blocks. Compiled warm (see its build entry). Address words are
 * DotrCpuWord. The host build replaces each block with its plain-C effect (rh_model_cpu.h). */

#ifndef __MWERKS__
/* Host build only: `last != m` compares int with unsigned int, as the original code does. */
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#define OUT_00213A40 ((struct DotrCpuPointOut *)arg0)
#define IN_00213A40 ((struct DotrCpuSharedIn *)arg1)

signed char lRhModelCpu_CalcSharedPoint(DotrCpuWord arg0, int *arg1) {
    struct DotrCpuPointSpr *spr = (struct DotrCpuPointSpr *)DOTR_CPU_SPR(0);
    register struct DotrCpuWeight *rec;
    int i;
    int n;
    int last;
    register float *src;
    float w;
    float fog;

#ifdef __MWERKS__
    asm {
        vsub.xyzw vf1, vf0, vf0
    }
#else
    DOTR_CPU_VF[1][0] = DOTR_CPU_VF[1][1] = DOTR_CPU_VF[1][2] = DOTR_CPU_VF[1][3] = 0.0f;
#endif
    n = IN_00213A40->count;
    last = -1;
    rec = IN_00213A40->w;
    src = spr->in;
    for (i = 0; i < n; i++, rec++) {
        register unsigned int m = rec->matrix;
        register unsigned int wgt;
        if (last != m) {
            last = m;
#ifdef __MWERKS__
            asm {
                ctc2.ni m, vi1
                vlqi.xyzw vf16, (vi1++)
                vlqi.xyzw vf17, (vi1++)
                vlqi.xyzw vf18, (vi1++)
                vlqi.xyzw vf19, (vi1++)
            }
#else
            dotr_rh_model_cpu_vi1 = m;
            dotr_rh_model_cpu_vlqi(16);
            dotr_rh_model_cpu_vlqi(17);
            dotr_rh_model_cpu_vlqi(18);
            dotr_rh_model_cpu_vlqi(19);
#endif
        }
        spr->in[0] = rec->x;
        spr->in[1] = rec->y;
        spr->in[2] = rec->z;
        wgt = rec->weight;
#ifdef __MWERKS__
        asm {
            qmtc2.ni wgt, vf4
            lqc2 vf2, 0x0(src)
            vitof12.x vf4, vf4
            vmulax.xyzw ACC, vf16, vf2x
            vmadday.xyzw ACC, vf17, vf2y
            vmaddaz.xyzw ACC, vf18, vf2z
            vmaddw.xyzw vf3, vf19, vf2w
            vmulx.xyzw vf2, vf3, vf4x
            vadd.xyzw vf1, vf1, vf2
        }
#else
        dotr_rh_model_cpu_load(2, src);
        DOTR_CPU_VF[4][0] = (float)(int)wgt / 4096.0f;
        dotr_rh_model_cpu_xform(3, 16, 2);
        { int j; for (j = 0; j < 4; j++) { DOTR_CPU_VF[1][j] += DOTR_CPU_VF[3][j] * DOTR_CPU_VF[4][0]; } }
#endif
    }
    {
        register float *dst = spr->res;
#ifdef __MWERKS__
        asm {
            vdiv Q, vf0w, vf1w
            vwaitq
            vmulq.xyz vf1, vf1, Q
            sqc2 vf1, 0x0(dst)
        }
#else
        dotr_rh_model_cpu_project(1);
        dotr_rh_model_cpu_store(dst, 1);
#endif
    }
    w = spr->res[3];
    if (w < 1.0f || !(w < spr->fF8) || spr->res[0] <= 1.0f || !(spr->res[0] < 4095.0f) || spr->res[1] <= 1.0f ||
        !(spr->res[1] < 4095.0f)) {
        return 0;
    }
    OUT_00213A40->x = (unsigned int)(16.0f * spr->res[0]);
    OUT_00213A40->y = (unsigned int)(16.0f * spr->res[1]);
    OUT_00213A40->z = (unsigned int)(spr->res[2]);
    fog = spr->f110 - spr->f114 * spr->res[3];
    if (fog <= 0.0f) {
        OUT_00213A40->fog = 0;
    } else if (!(fog < 255.0f)) {
        OUT_00213A40->fog = 0xFF;
    } else {
        OUT_00213A40->fog = (unsigned int)(fog);
    }
    OUT_00213A40->color = spr->f100;
    OUT_00213A40->s = IN_00213A40->u / spr->res[3] / 4096.0f;
    OUT_00213A40->t = IN_00213A40->v / spr->res[3] / 4096.0f;
    OUT_00213A40->q = 1.0f / spr->res[3];
    return 1;
}

#undef OUT_00213A40
#undef IN_00213A40
