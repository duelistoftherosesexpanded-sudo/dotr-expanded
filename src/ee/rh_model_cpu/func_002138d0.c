#include "dotr/ee/rh_model_cpu.h"

/* 0x2138d0. Address words are DotrCpuWord. Compiled with #pragma opt_lifetimes off, set
 * directly before the function and reset after it (docs/DECOMPILATION.md). */

#ifdef __MWERKS__
#pragma opt_lifetimes off
#endif

DotrCpuWord lRhModelCpu_CalcSharedStrip(DotrCpuWord arg0, int *arg1) {
    extern signed char lRhModelCpu_CalcSharedPoint(DotrCpuWord, int *);
    unsigned char *scratch;
    unsigned char *var_s1;
    int var_s2;
    int temp_s3;
    int var_s4;
    struct DotrCpuPrim *prim;
    unsigned int z0;
    unsigned int z1;

    var_s1 = (unsigned char *)arg1 + 0x14;
    prim = (struct DotrCpuPrim *)arg0;
    scratch = (unsigned char *)DOTR_CPU_SPR(0);
    var_s4 = 0;
    temp_s3 = arg1[1] & 0x7FFF;
    for (var_s2 = 0; var_s2 < temp_s3; var_s2++) {
        scratch[0xF0 + var_s4] = lRhModelCpu_CalcSharedPoint((DotrCpuWord)(scratch + var_s4 * 0x18 + 0x90), (int *)var_s1);
        if (var_s2 >= 2) {
            if (scratch[0xF0] && scratch[0xF1] && scratch[0xF2]) {
                RhMem_CopyMemory16(prim, scratch + 0x60, 8);
                prim->f78 = 0;
                z0 = prim->v0.z;
                z1 = prim->v1.z;
                z0 = ((z0 > z1 ? z0 : z1) > prim->v2.z) ? (z0 > z1 ? z0 : z1) : prim->v2.z;
                prim->depth = z0;
                prim++;
            }
        }
        var_s1 = (unsigned char *)((struct DotrCpuSharedItem *)(var_s1 + 6) + *(unsigned short *)(var_s1 + 4));
        var_s4 = (var_s4 + 1) % 3;
    }
    *(unsigned int *)DOTR_CPU_SPR(0xFC) = (DotrCpuUWord)(var_s1 + 3) & ~3;
    return (DotrCpuWord)prim;
}
#ifdef __MWERKS__
#pragma opt_lifetimes reset
#endif
