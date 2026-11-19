#include "dotr/ee/rh_model_cpu.h"

/* 0x2134d0. C with inline VU0 blocks. Address words are DotrCpuWord. The host build replaces each
 * block with its plain-C effect (rh_model_cpu.h). */

#define prim ((struct DotrCpuPrim *)arg0)

DotrCpuWord lRhModelCpu_CalcIndepStrip(DotrCpuWord arg0, int *arg1) {
    unsigned char *scratch;
    unsigned char *var_s1;
    int var_s2;
    int temp_s3;
    int var_s4;

    var_s1 = (unsigned char *)arg1 + 0x18;
    scratch = (unsigned char *)DOTR_CPU_SPR(0);
    var_s4 = 0;
    temp_s3 = arg1[1] & 0x7FFF;
    for (var_s2 = 0; var_s2 < temp_s3; var_s2++) {
        register int base = arg1[4];
#ifdef __MWERKS__
        asm {
            ctc2.ni base, vi1
            vlqi.xyzw vf16, (vi1++)
            vlqi.xyzw vf17, (vi1++)
            vlqi.xyzw vf18, (vi1++)
            vlqi.xyzw vf19, (vi1++)
        }
#else
        dotr_rh_model_cpu_vi1 = base;
        dotr_rh_model_cpu_vlqi(16);
        dotr_rh_model_cpu_vlqi(17);
        dotr_rh_model_cpu_vlqi(18);
        dotr_rh_model_cpu_vlqi(19);
#endif
        scratch[0xF0 + var_s4] = lRhModelCpu_CalcIndepPoint((DotrCpuWord)(scratch + var_s4 * 0x18 + 0x90), (int *)var_s1);
        if (var_s2 >= 2) {
            if (scratch[0xF0] && scratch[0xF1] && scratch[0xF2]) {
                RhMem_CopyMemory16(prim, scratch + 0x60, 8);
                prim->f78 = 0;
                prim->depth = ((prim->v0.z > prim->v1.z ? prim->v0.z : prim->v1.z) > prim->v2.z)
                    ? (prim->v0.z > prim->v1.z ? prim->v0.z : prim->v1.z) : prim->v2.z;
                arg0 += sizeof(struct DotrCpuPrim);
            }
        }
        var_s1 += 0x12;
        var_s4 = (var_s4 + 1) % 3;
    }
    *(unsigned int *)(scratch + 0xFC) = (DotrCpuUWord)(var_s1 + 3) & ~3;
    return arg0;
}

#undef prim
