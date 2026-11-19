#ifndef DOTR_EE_RH_MODEL_CPU_H
#define DOTR_EE_RH_MODEL_CPU_H

/* lRhModelCpu functions that transform model vertices on the CPU with VU0 macro instructions
 * through the scratchpad work block at 0x70000000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The code addresses the scratchpad by literal addresses. The target build keeps the literal; the host build
 * points it at an object the test defines. */
#ifdef __MWERKS__
#define DOTR_CPU_SPR(off) (0x70000000 + (off))
#else
#include <stdint.h>
extern unsigned char dotr_rh_model_cpu_spr[0x200];
#define DOTR_CPU_SPR(off) ((intptr_t)dotr_rh_model_cpu_spr + (off))
#endif

#ifndef NULL
#define NULL 0
#endif

/* Addresses kept in `s32` / `u32` words (packet cursors, record pointers): 32 bits on the target,
 * pointer-sized on the host. A 16-byte word moved with lq/sq. */
#ifdef __MWERKS__
typedef int DotrCpuWord;
typedef unsigned int DotrCpuUWord;
typedef __int128 DotrCpuQuad;
#else
typedef intptr_t DotrCpuWord;
typedef uintptr_t DotrCpuUWord;
__extension__ typedef __int128 DotrCpuQuad;
#endif

/* Scratchpad views as each function reads them (field names are offsets or use; meanings beyond
 * use are not established). The view-screen matrix sits at +0; packet tags at +0x60. */
struct DotrCpuGlobalSpr {
    float m[4][4];
    unsigned char pad40[0x1C];
    float f5C;
    unsigned int tag[8];
    unsigned long long f80;
    unsigned long long f88;
    unsigned char pad90[0x64];
    float fF4;
    float fF8;
    unsigned char padFC[8];
    int f104;
    unsigned char pad108[8];
    float f110;
    float f114;
};
/* One output vertex of a strip (screen xy, 24-bit z, 8-bit fog) and the 0x80-byte triangle a
 * strip writes: three vertices copied from scratchpad +0x60, a cleared word and the largest z. */
struct DotrCpuVert {
    unsigned int xy;
    unsigned int z : 24;
    unsigned int f : 8;
};
struct DotrCpuPrim {
    unsigned char pad00[0x40];
    struct DotrCpuVert v0;
    unsigned char pad48[0x10];
    struct DotrCpuVert v1;
    unsigned char pad60[0x10];
    struct DotrCpuVert v2;
    int f78;
    int depth;
};
/* An 18-byte weight record of a shared strip. */
struct DotrCpuSharedItem {
    unsigned char data[0x12];
};
/* The scratchpad as the point functions read it: the projected result at +0x40, the input vector
 * at +0x50, the far clip at +0xf8, the colour word at +0x100 and the fog factors at +0x110. */
struct DotrCpuPointSpr {
    unsigned char pad00[0x40];
    float res[4];
    float in[4];
    unsigned char pad60[0x98];
    float fF8;
    unsigned char padFC[4];
    unsigned int f100;
    unsigned char pad104[0xC];
    float f110;
    float f114;
};
/* The 24-byte output vertex (s, t, colour, q, x, y, z, fog). */
struct DotrCpuPointOut {
    float s;
    float t;
    unsigned int color;
    float q;
    unsigned short x;
    unsigned short y;
    unsigned int z : 24;
    unsigned int fog : 8;
};
/* Input point records: texture u/v and a position (independent), or u/v, a count and that many
 * matrix/weight/position records (shared). */
struct DotrCpuIndepIn {
    short u;
    short v;
    unsigned char pad4[8];
    short x;
    short y;
    short z;
};
struct DotrCpuWeight {
    unsigned short matrix;
    unsigned short weight;
    unsigned char pad4[8];
    short x;
    short y;
    short z;
};
struct DotrCpuSharedIn {
    short u;
    short v;
    unsigned short count;
    struct DotrCpuWeight w[1];
};

/* Callees, with address words as above.
 * RhView_* are defined in view_state, RhMem_CopyMemory16 in memory_ops and
 * lRhModelCpu_TraceStaticBlock in item_walks; func_00186120 is sceDmaGetChan in the SDK library
 * block. */
void RhView_GetViewScreenMatrix(void *dst);
int RhView_GetClipZ(float *arg0, float *arg1);
int RhView_GetFogParameter(float *arg0, float *arg1, float *arg2, float *arg3, unsigned char *arg4);
int func_00186120(int arg0);
void *RhMem_CopyMemory16(void *dest, void *source, int count);
DotrCpuWord lRhModelCpu_TraceStaticBlock(DotrCpuWord arg0, unsigned int cursor, int size);

/* 0x2130a0: loads the view-screen matrix into vf20-vf23 and fills the scratchpad's packet tags,
 * clip range, DMA channel and fog factors. */
int lRhModelCpu_SetupGlobal(int arg0);
/* 0x2131d0: walks a block of tagged records, transforming matrices into VU0 memory and handing
 * static blocks to lRhModelCpu_TraceStaticBlock; returns the packet cursor or NULL. */
unsigned char *lRhModelCpu_TraceDynamicBlock(unsigned char *out, DotrCpuWord data, DotrCpuWord size);
/* 0x2134d0 / 0x2138d0: strips of independent or weighted (shared) points into triangles. */
DotrCpuWord lRhModelCpu_CalcIndepStrip(DotrCpuWord arg0, int *arg1);
DotrCpuWord lRhModelCpu_CalcSharedStrip(DotrCpuWord arg0, int *arg1);
/* 0x213680 / 0x213a40: one point projected through vf16-vf19 (shared: a weighted sum over
 * matrices from VU0 memory); returns 1 when it lies inside the screen and depth range. */
signed char lRhModelCpu_CalcIndepPoint(DotrCpuWord arg0, int *arg1);
signed char lRhModelCpu_CalcSharedPoint(DotrCpuWord arg0, int *arg1);

#ifndef __MWERKS__
/* Host only: a model of the VU0 state the inline blocks use (vf0 is (0, 0, 0, 1); VU0 data memory
 * by quadword; vi1), and the effect of each block in plain C. The VU0 blocks themselves are
 * verified only by make match. */
extern float dotr_rh_model_cpu_vf[32][4];
extern float dotr_rh_model_cpu_vumem[0x400][4];
extern unsigned int dotr_rh_model_cpu_vi1;
#define DOTR_CPU_VF dotr_rh_model_cpu_vf
/* lqc2 / vlqi / sqc2 / vsqi. */
static inline void dotr_rh_model_cpu_load(int r, const float *p)
{
    int j;

    for (j = 0; j < 4; j++) {
        DOTR_CPU_VF[r][j] = p[j];
    }
}
static inline void dotr_rh_model_cpu_store(float *p, int r)
{
    int j;

    for (j = 0; j < 4; j++) {
        p[j] = DOTR_CPU_VF[r][j];
    }
}
static inline void dotr_rh_model_cpu_vlqi(int r)
{
    dotr_rh_model_cpu_load(r, dotr_rh_model_cpu_vumem[dotr_rh_model_cpu_vi1++ & 0x3ff]);
}
static inline void dotr_rh_model_cpu_vsqi(int r)
{
    dotr_rh_model_cpu_store(dotr_rh_model_cpu_vumem[dotr_rh_model_cpu_vi1++ & 0x3ff], r);
}
/* vmulax/vmadday/vmaddaz/vmaddw: vf[dst] = vf[m]*s.x + vf[m+1]*s.y + vf[m+2]*s.z + vf[m+3]*s.w. */
static inline void dotr_rh_model_cpu_xform(int dst, int m, int src)
{
    float s[4];
    int j;

    for (j = 0; j < 4; j++) {
        s[j] = DOTR_CPU_VF[src][j];
    }
    for (j = 0; j < 4; j++) {
        DOTR_CPU_VF[dst][j] = DOTR_CPU_VF[m][j] * s[0] + DOTR_CPU_VF[m + 1][j] * s[1]
            + DOTR_CPU_VF[m + 2][j] * s[2] + DOTR_CPU_VF[m + 3][j] * s[3];
    }
}
/* vdiv Q, vf0w, vf[r]w; vwaitq; vmulq.xyz vf[r], vf[r], Q. */
static inline void dotr_rh_model_cpu_project(int r)
{
    float q = 1.0f / DOTR_CPU_VF[r][3];
    int j;

    for (j = 0; j < 3; j++) {
        DOTR_CPU_VF[r][j] *= q;
    }
}
#endif

#endif
