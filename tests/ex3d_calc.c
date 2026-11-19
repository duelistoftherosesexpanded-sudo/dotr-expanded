/* Synthetic host tests for the ex3d_calc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/ex3d_calc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned char data_00325cc0_ex3dcalc[30 * sizeof(float)] __attribute__((aligned(16)));
static int push_calls, pop_calls;

void RhView_GetWorldScreenMatrix(void *matrix) {}
void RhGeom_PushMatrix(DotrEx3dCalcWord matrix) { push_calls++; }
void RhGeom_PopMatrix(void) { pop_calls++; }
/* Stub: writes one transformed vertex (32, 64, 100, w 2) to the output. */
void RhGeom_MulMatrixVectorArray(DotrEx3dCalcWord out, int matrix, DotrEx3dCalcWord in, int count)
{
    float *o = (float *)out;

    o[0] = 32.0f;
    o[1] = 64.0f;
    o[2] = 100.0f;
    o[3] = 2.0f;
}
long long func_001a23b0(float value) { return (long long)value; }
int RhView_GetClipZ(float *near, float *far) { *near = 1.0f; *far = 100.0f; return 0; }
int RhView_GetScreenCenter(float *x, float *y) { *x = 10.0f; *y = 10.0f; return 0; }
int RhView_GetScreenDist(float *dist) { *dist = 5.0f; return 0; }
float func_001a6c60(float x) { return x; }

int main(void)
{
    static union { struct DotrEx3dCalcPacket p; unsigned long long w[3]; } packet;
    float src[4] = { 0 }, out[4], average = 0.0f, ident[16] = { 0 };
    short xy[2];
    float *planes = (float *)data_00325cc0_ex3dcalc;

    /* Ex3d_RotTransPersAutoCalcQ: one type-4 vertex; after the divide by w 2 the entry holds
     * 16 * x = 256 and 16 * y = 512, the result is 1 and the average w is 2. */
    packet.p.count = 1;
    packet.p.types = 4;
    assert(Ex3d_RotTransPersAutoCalcQ(src, out, (DotrEx3dCalcWord)ident, (DotrEx3dCalcWord)&packet,
                                      (DotrEx3dCalcWord)&average) == 1);
    memcpy(xy, packet.p.data, sizeof(xy));
    assert(xy[0] == 256 && xy[1] == 512 && average == 2.0f && src[3] == 1.0f && push_calls == 1 && pop_calls == 1);

    /* Ex3d_InitClipPlane: half-width 10 + 10, distance 5: the first plane's normal is (0, 200, 800). */
    Ex3d_InitClipPlane();
    assert(planes[0] == 0.0f && planes[1] == 200.0f && planes[2] == 800.0f && planes[3] == 0.0f);
    return 0;
}
