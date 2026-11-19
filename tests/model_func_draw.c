/* Synthetic host tests for the model_func_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/model_func_draw.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

DotrFuncDrawObject *gSzModel_ahModel_funcdraw[0x54];
DotrFuncDrawVector data_00292fd0, data_00292fe0;
int gSzModel_nBlurMode_word, data_0031bb10;

static int is_use_calls;

int func_001a23b0(float arg_a) { return 0; }
int SzModel_LoadTexture_Pipe(int current, int object, void *data, int *size) { return 0; }
int lSzModel_GetGNode(int arg0, int arg1, int *arg2) { return 0; }
int SzEffect_IsUse2(int arg0, int arg1)
{
    is_use_calls++;
    return 0;
}
void *SzView_SetFog_Pipe(void *arg0, int arg1, int arg2, float arg3) { return 0; }
int SzView_SetLight_Pipe(int arg0, int arg1, int arg2) { return 0; }
int SzView_InqPointLight(int arg0, int arg1) { return 0; }
int SzView_IsFogZ(int arg0, int arg1) { return 0; }
int SzView_IsFogY(int arg0, int arg1) { return 0; }
int Ex3d_GetSampleZ(void *arg0, float fparg0) { return 0; }
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3) { return 0; }
void *ExDitherMode_Pipe(void *arg0, int arg1) { return 0; }
int ExAssignAlpha(void *arg0, long long arg1) { return 0; }
long long ExGetAlphaVal(int arg0) { return 0; }
void *RhBase_MaskFBuffer(void *arg0, unsigned int arg1) { return 0; }
int RhBase_SetZBufferMode(int arg0, int arg1) { return 0; }
int RhBase_AssignRenderFrame(int arg0, int arg1, int arg2) { return 0; }
void *RhBase_EnableRenderFrame(void *arg0) { return 0; }
void *RhBase_DisableRenderFrame(void *arg0) { return 0; }
void *RhBase_ClearRenderFrame(unsigned char *packet, unsigned char *color) { return 0; }
void RhBase_FreeRenderFrame(void) { }
int RhBase_GetTextureInfoForRender(void *arg0, void *arg1) { return 0; }
void *RhMem_CopyMemory16(void *dest, void *source, int count) { return 0; }
void *RhMem_FillMemory4(void *dst, int val, int count) { return 0; }
void RhGeom_PushMatrix(int arg0) { }
void RhGeom_PopMatrix(void) { }
void RhGeom_MulMatrix(int arg0, void *arg1) { }
void RhGeom_MulMatrixRev(int arg0, int arg1) { }
void RhGeom_InvertMatrix(void *dst, void *src) { }
void RhGeom_GetTransMatrix(void *arg0, float *arg1) { }
void RhGeom_GetScaleRotXYZTransMatrix(void *arg0, void *p1, void *p2, void *p3) { }
void RhGeom_ScaleRotXYZTransMatrix(void *dst, float *scale, int *rot, float *trans) { }
void RhGeom_MulMatrixVector(void *arg0, void *arg1, void *arg2) { }
int RhView_GetCurView(void *arg0) { return 0; }
int RhView_SetClipZ(float fparg0, float fparg1) { return 0; }
int RhView_Modify(void *arg0) { return 0; }
int RhView_PushStack(void) { return 0; }
int RhView_PopStack(void) { return 0; }
void RhView_GetWorldViewMatrix(void *arg0) { }
int RhView_SendViewParameters(int arg0) { return 0; }
int RhText_SetTextureFlush(int arg0) { return 0; }
void *RhModel_Draw(void *arg0, void *arg1) { return 0; }
void GetAnyCoordinateInit(void) { }
int BookingGetAnyCoordinate(int id) { return 0; }
int GetAnyCoordinate(int arg0, void *arg1) { return 0; }

static void callback(void) { }

int main(void)
{
    /* No model in slot 5: after asking whether effects 0-2 are in use, the cursor comes back
     * unchanged. */
    assert(SzModel_DrawWithFunc_Pipe(0x1234, 5, 0, 0, 0, 0, 0, callback, 0) == 0x1234 && is_use_calls == 3);
    return 0;
}
