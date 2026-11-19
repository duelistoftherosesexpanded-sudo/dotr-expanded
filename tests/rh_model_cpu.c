/* Synthetic host tests for the rh_model_cpu group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* The VU0 blocks are replaced on the host by their plain-C effect on a model of vf0-vf31, VU0
 * memory and vi1 (rh_model_cpu.h); these tests check that path only. The VU0 blocks themselves
 * are verified only by make match. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/rh_model_cpu.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned char dotr_rh_model_cpu_spr[0x200];
float dotr_rh_model_cpu_vf[32][4];
float dotr_rh_model_cpu_vumem[0x400][4];
unsigned int dotr_rh_model_cpu_vi1;

/* The view-screen matrix stub writes an identity matrix. */
void RhView_GetViewScreenMatrix(void *dst)
{
    float *m = dst;
    int i;

    memset(m, 0, 16 * sizeof(float));
    for (i = 0; i < 4; i++) {
        m[i * 5] = 1.0f;
    }
}
int RhView_GetClipZ(float *arg0, float *arg1) { return 0; }
int RhView_GetFogParameter(float *arg0, float *arg1, float *arg2, float *arg3, unsigned char *arg4) { return 0; }
int func_00186120(int arg0) { return 0; }
void *RhMem_CopyMemory16(void *dest, void *source, int count) { return dest; }
DotrCpuWord lRhModelCpu_TraceStaticBlock(DotrCpuWord arg0, unsigned int cursor, int size) { return arg0; }

static struct DotrCpuPointSpr *point_pad(void)
{
    struct DotrCpuPointSpr *spr = (struct DotrCpuPointSpr *)dotr_rh_model_cpu_spr;

    memset(dotr_rh_model_cpu_spr, 0, sizeof(dotr_rh_model_cpu_spr));
    spr->in[3] = 1.0f;
    spr->fF8 = 1000.0f;
    return spr;
}

/* Identity in vf16-vf19 and in VU0 memory quadwords 0-3. */
static void identity(void)
{
    int i;

    memset(dotr_rh_model_cpu_vf, 0, sizeof(dotr_rh_model_cpu_vf));
    memset(dotr_rh_model_cpu_vumem, 0, sizeof(dotr_rh_model_cpu_vumem));
    for (i = 0; i < 4; i++) {
        dotr_rh_model_cpu_vf[16 + i][i] = 1.0f;
        dotr_rh_model_cpu_vumem[i][i] = 1.0f;
    }
}

int main(void)
{
    struct DotrCpuGlobalSpr *global = (struct DotrCpuGlobalSpr *)dotr_rh_model_cpu_spr;
    struct DotrCpuPointOut out;
    struct DotrCpuIndepIn indep;
    unsigned short shared[16];
    unsigned int block[4] = { 0x10000000, 0, 0, 0 };
    unsigned char packet[0x100];
    int strip[16];

    /* lRhModelCpu_SetupGlobal without fog: vf20 is the matrix's first row, fog factor 255. */
    memset(dotr_rh_model_cpu_spr, 0, sizeof(dotr_rh_model_cpu_spr));
    assert(lRhModelCpu_SetupGlobal(0) == 1 && dotr_rh_model_cpu_vf[20][0] == 1.0f && global->f110 == 255.0f);

    /* lRhModelCpu_TraceDynamicBlock over one empty record (0x10000000): the cursor comes back. */
    assert(lRhModelCpu_TraceDynamicBlock(packet, (DotrCpuWord)block, sizeof(block)) == packet);

    /* lRhModelCpu_CalcIndepPoint: (100, 200, 300) through identity is on screen; x is 16 * 100. */
    identity();
    point_pad();
    memset(&indep, 0, sizeof(indep));
    indep.x = 100, indep.y = 200, indep.z = 300;
    assert(lRhModelCpu_CalcIndepPoint((DotrCpuWord)&out, (int *)&indep) == 1 && out.x == 1600);

    /* lRhModelCpu_CalcSharedPoint: one record, matrix 0 at full weight (4096): the same point. */
    identity();
    point_pad();
    memset(shared, 0, sizeof(shared));
    shared[2] = 1;            /* count */
    shared[4] = 4096;         /* weight */
    shared[9] = 100, shared[10] = 200, shared[11] = 300;
    assert(lRhModelCpu_CalcSharedPoint((DotrCpuWord)&out, (int *)shared) == 1 && out.y == 3200);

    /* The two strips with one point: no triangle yet, the output cursor comes back unchanged. */
    identity();
    point_pad();
    memset(strip, 0, sizeof(strip));
    strip[1] = 1;
    assert(lRhModelCpu_CalcIndepStrip((DotrCpuWord)packet, strip) == (DotrCpuWord)packet);
    point_pad();
    assert(lRhModelCpu_CalcSharedStrip((DotrCpuWord)packet, strip) == (DotrCpuWord)packet);
    return 0;
}
