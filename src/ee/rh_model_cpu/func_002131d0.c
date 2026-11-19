#include "dotr/ee/rh_model_cpu.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */
/* 0x2131d0. C with inline VU0 blocks. Compiled warm (see its build entry). Address words are
 * DotrCpuWord. The host build replaces each block with its plain-C effect (rh_model_cpu.h). */

#define P_002131D0 ((unsigned char *)data)

unsigned char *lRhModelCpu_TraceDynamicBlock(unsigned char *out, DotrCpuWord data, DotrCpuWord size) {
    unsigned char *spr = (unsigned char *)DOTR_CPU_SPR(0);
    unsigned int cmd;

    size = data + size;
    while (P_002131D0 < (unsigned char *)size) {
        cmd = *(unsigned int *)P_002131D0;
        if (cmd == 0x10000008) {
            *(DotrCpuQuad *)(spr + 0x60) = *(DotrCpuQuad *)P_002131D0;
            *(unsigned int *)(spr + 0x60) = 0x30000008;
            *(unsigned char **)(spr + 0x64) = P_002131D0 + 0x10;
            data += 0x90;
        } else if (cmd == 0x10000000) {
            data += 0x10;
        } else if (cmd == 0x10000003) {
            unsigned char *q = P_002131D0;

            RhMem_CopyMemory16(spr, q, 4);
            *(unsigned int *)(spr + 0x100) = (unsigned char)(unsigned int)(*(float *)(spr + 0x20) / 2.0f + *(float *)(spr + 0x10));
            *(unsigned int *)(spr + 0x100) |= (unsigned char)(unsigned int)(*(float *)(spr + 0x24) / 2.0f + *(float *)(spr + 0x14)) << 8;
            *(unsigned int *)(spr + 0x100) |= (unsigned char)(unsigned int)(*(float *)(spr + 0x28) / 2.0f + *(float *)(spr + 0x18)) << 16;
            *(unsigned int *)(spr + 0x100) |= (unsigned int)*(float *)(spr + 0x2C) << 24;
            data = (DotrCpuWord)(q + 0x40);
        } else if ((cmd & 0xF0000000) == 0x30000000) {
            int x = *(int *)(P_002131D0 + 0xC);

            if (x == 0) {
                unsigned char *q = P_002131D0;

                out = (unsigned char *)lRhModelCpu_TraceStaticBlock((DotrCpuWord)out, (int)*(unsigned int *)(q + 4), *(unsigned short *)q << 4);
                data = (DotrCpuWord)(q + 0x10);
            } else {
                register unsigned char *m;
                register unsigned int dst = x & 0x3FF;
                unsigned char *q = P_002131D0;

                data = (DotrCpuWord)(q + 0x10);
                m = *(unsigned char **)(q + 4);
#ifdef __MWERKS__
                asm {
                    lqc2 vf16, 0x0(m)
                    lqc2 vf17, 0x10(m)
                    lqc2 vf18, 0x20(m)
                    lqc2 vf19, 0x30(m)
                    ctc2.ni dst, vi1
                    vmulax.xyzw ACC, vf20, vf16x
                    vmadday.xyzw ACC, vf21, vf16y
                    vmaddaz.xyzw ACC, vf22, vf16z
                    vmaddw.xyzw vf16, vf23, vf16w
                    vmulax.xyzw ACC, vf20, vf17x
                    vmadday.xyzw ACC, vf21, vf17y
                    vmaddaz.xyzw ACC, vf22, vf17z
                    vmaddw.xyzw vf17, vf23, vf17w
                    vmulax.xyzw ACC, vf20, vf18x
                    vmadday.xyzw ACC, vf21, vf18y
                    vmaddaz.xyzw ACC, vf22, vf18z
                    vmaddw.xyzw vf18, vf23, vf18w
                    vmulax.xyzw ACC, vf20, vf19x
                    vmadday.xyzw ACC, vf21, vf19y
                    vmaddaz.xyzw ACC, vf22, vf19z
                    vmaddw.xyzw vf19, vf23, vf19w
                    vsqi.xyzw vf16, (vi1++)
                    vsqi.xyzw vf17, (vi1++)
                    vsqi.xyzw vf18, (vi1++)
                    vsqi.xyzw vf19, (vi1++)
                }
#else
                dotr_rh_model_cpu_load(16, (float *)m);
                dotr_rh_model_cpu_load(17, (float *)(m + 0x10));
                dotr_rh_model_cpu_load(18, (float *)(m + 0x20));
                dotr_rh_model_cpu_load(19, (float *)(m + 0x30));
                dotr_rh_model_cpu_vi1 = dst;
                dotr_rh_model_cpu_xform(16, 20, 16);
                dotr_rh_model_cpu_xform(17, 20, 17);
                dotr_rh_model_cpu_xform(18, 20, 18);
                dotr_rh_model_cpu_xform(19, 20, 19);
                dotr_rh_model_cpu_vsqi(16);
                dotr_rh_model_cpu_vsqi(17);
                dotr_rh_model_cpu_vsqi(18);
                dotr_rh_model_cpu_vsqi(19);
#endif
            }
        } else {
            return NULL;
        }
    }
    return out;
}

#undef P_002131D0
