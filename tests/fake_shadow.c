/* Host smoke test for lSzBattle_DrawFakeShadow (0x1e6f90), a battle model's fake shadow.
 * Fixtures are host data, not game data; stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* Built without -Werror: the C converts 32-bit EE words to pointers, which warns on a
 * 64-bit host. The path below executes none of those conversions. */
#include <assert.h>
#include "dotr/ee/fake_shadow.h"

/* Data the function reads, declared in its block scope with types the unit defines next to it:
 * same sizes as plain floats here (host storage; the target links them at their addresses). */
f32 data_00292380[5] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
f32 data_002923a0[48];
f32 data_00292460[2];

/* Call log. */
static const char *calls[64];
static int call_count;
static void note(const char *name)
{
    if (call_count < 64) {
        calls[call_count] = name;
    }
    call_count++;
}
static int same(const char *a, const char *b)
{
    while (*a != 0 && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}
static int called(const char *name)
{
    int i, n = 0;
    for (i = 0; i < call_count && i < 64; i++) {
        n += same(calls[i], name);
    }
    return n;
}

/* Callees on the tested path. */
static f32 plane_d = 0.0f;
s32 SzModel_GetModelType(s32 arg0, f32 *plane)
{
    note("SzModel_GetModelType");
    assert(arg0 == 3);
    plane_d = plane[3];
    return 4;
}
s32 SzModel_IsModelDisp(s32 arg0) { note("SzModel_IsModelDisp"); assert(arg0 == 3); return 1; }
s32 SzView_InqStraightLight(s32 arg0, s32 arg1) { note("SzView_InqStraightLight"); assert(arg0 == 0 && arg1 == 4); return 0; }
static s32 light_alpha = -1;
s32 SzView_GetStraightLightVector(s32 arg0, s32 arg1, s32 arg2, void *arg3)
{
    f32 *v = arg3;
    note("SzView_GetStraightLightVector");
    assert(arg0 == 0 && arg2 == 0);
    light_alpha = arg1;
    v[0] = 0.0f;
    v[1] = -0.5f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    return 0;
}
s32 RhView_GetCurView(void *arg0)
{
    f32 *f = arg0;
    int i;
    note("RhView_GetCurView");
    for (i = 0; i < 36; i++) {
        f[i] = 0.0f;
    }
    return 0;
}
f32 RhGeom_InnerProduct(f32 *arg0, f32 *arg1) { note("RhGeom_InnerProduct"); return arg0[1] * arg1[1]; }

/* Every other callee, declared as the unit sees it (the header, or the function's block-scope
 * declarations where they differ): records the call, returns 0. */
void Ex3d_IntersectP(void *arg0, void *arg1, void *arg2, void *arg3) { note("Ex3d_IntersectP"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; }
s32 Ex3d_NodeL2V(s32 arg0, s32 arg1, void *arg2, s32 arg3, void *arg4, s32 arg5, s32 arg6) { note("Ex3d_NodeL2V"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return 0; }
void Ex3d_NodeV2S(void *arg0, s32 arg1, void *arg2) { note("Ex3d_NodeV2S"); (void)arg0; (void)arg1; (void)arg2; }
s32 ExAssignAlpha(void *arg0, s64 arg1) { note("ExAssignAlpha"); (void)arg0; (void)arg1; return 0; }
void * ExDitherMode_Pipe(void *arg0, s32 arg1) { note("ExDitherMode_Pipe"); (void)arg0; (void)arg1; return 0; }
s32 ExSortNode_Pipe(s32 arg0, void *arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5) { note("ExSortNode_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; return 0; }
s32 func_001a23b0(f32 arg0) { note("func_001a23b0"); (void)arg0; return 0; }
void RhBase_AssignRenderFrame(s32 arg0, s32 arg1, s32 arg2) { note("RhBase_AssignRenderFrame"); (void)arg0; (void)arg1; (void)arg2; }
s32 RhBase_ClearRenderFrame(void *arg0, s32 arg1) { note("RhBase_ClearRenderFrame"); (void)arg0; (void)arg1; return 0; }
void * RhBase_DisableRenderFrame(void *arg0) { note("RhBase_DisableRenderFrame"); (void)arg0; return 0; }
void * RhBase_EnableRenderFrame(void *arg0) { note("RhBase_EnableRenderFrame"); (void)arg0; return 0; }
void RhBase_FreeRenderFrame(void) { note("RhBase_FreeRenderFrame");  }
s32 RhBase_GetTextureInfoForRender(void *arg0, void *arg1) { note("RhBase_GetTextureInfoForRender"); (void)arg0; (void)arg1; return 0; }
s32 RhBase_SetZBufferMode(s32 arg0, s32 arg1) { note("RhBase_SetZBufferMode"); (void)arg0; (void)arg1; return 0; }
f32 RhGeom_GetPointsDistance(f32 *arg0, f32 *arg1) { note("RhGeom_GetPointsDistance"); (void)arg0; (void)arg1; return 0; }
void RhGeom_InvertMatrix(void *arg0, void *arg1) { note("RhGeom_InvertMatrix"); (void)arg0; (void)arg1; }
void RhGeom_MulMatrixVectorArray(void *arg0, s32 arg1, void *arg2, s32 arg3) { note("RhGeom_MulMatrixVectorArray"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; }
void RhGeom_PopMatrix(void) { note("RhGeom_PopMatrix");  }
void RhGeom_PushMatrix(s32 arg0) { note("RhGeom_PushMatrix"); (void)arg0; }
s32 RhText_SetTextureFlush(s32 arg0) { note("RhText_SetTextureFlush"); (void)arg0; return 0; }
void RhView_GetWorldViewMatrix(void *arg0) { note("RhView_GetWorldViewMatrix"); (void)arg0; }
s32 RhView_Modify(void *arg0) { note("RhView_Modify"); (void)arg0; return 0; }
s32 RhView_PopStack(void) { note("RhView_PopStack");  return 0; }
s32 RhView_PushStack(void) { note("RhView_PushStack");  return 0; }
s32 RhView_SendViewParameters(void *arg0) { note("RhView_SendViewParameters"); (void)arg0; return 0; }
s32 RhView_SetScreen(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) { note("RhView_SetScreen"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; return 0; }
s32 SzModel_DrawWithOutCalc_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) { note("SzModel_DrawWithOutCalc_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return 0; }
s32 SzModel_GetAlpha(s32 arg0) { note("SzModel_GetAlpha"); (void)arg0; return 0; }
s32 SzModel_GetCenter(s32 arg0, void *arg1) { note("SzModel_GetCenter"); (void)arg0; (void)arg1; return 0; }

int main(void)
{
    /* Light from above (inner product with the floor plane negative): the plane's distance is set to
     * minus the height, the missing straight light falls back to light 9, and no shadow is drawn; the
     * packet pointer comes back unchanged. */
    assert(lSzBattle_DrawFakeShadow(0x5000, 3, 0, 12.5f, 800.0f) == 0x5000);
    assert(plane_d == -12.5f);
    assert(light_alpha == 9);
    assert(called("RhGeom_InnerProduct") == 1);
    assert(called("RhView_PushStack") == 0 && called("RhBase_FreeRenderFrame") == 0);
    return 0;
}
