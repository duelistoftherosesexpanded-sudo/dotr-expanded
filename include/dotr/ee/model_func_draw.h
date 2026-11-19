#ifndef DOTR_EE_MODEL_FUNC_DRAW_H
#define DOTR_EE_MODEL_FUNC_DRAW_H

/* SzModel_DrawWithFunc_Pipe, the model draw path with a per-model callback, render-frame
 * passes, fog and light set-up.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Field meanings and parameter types in the types, data and callee prototypes below are
 * assumed; the function reads many further model fields at raw byte offsets. */

#ifndef NULL
#define NULL 0
#endif

struct DotrFuncDrawVector4f {
    float x, y, z, w;
};
struct DotrFuncDrawSlotInner {
    unsigned char pad00[4];
    int f4;
};
typedef struct {
    unsigned char pad00[0x14];
    struct DotrFuncDrawSlotInner *f14;
} DotrFuncDrawSlot;
/* The model table at 0x31abb0 holds 0x54 pointers to these. */
typedef struct {
    unsigned char pad000[0x30];
    struct DotrFuncDrawVector4f v;      /* 0x030 */
    unsigned char pad040[0x138];
    DotrFuncDrawSlot *slots[18];        /* 0x178 */
    int vals[8];                        /* 0x1C0 */
    signed char lv[4];                  /* 0x1E0 */
} DotrFuncDrawObject;
extern DotrFuncDrawObject *gSzModel_ahModel_funcdraw[];

/* The 128-bit integer type: the host accepts it as a GNU extension. */
#ifdef __MWERKS__
typedef __int128 DotrFuncDrawQ128;
#else
__extension__ typedef __int128 DotrFuncDrawQ128;
#endif
typedef union { DotrFuncDrawQ128 q; float f[4]; } DotrFuncDrawVector;
typedef union { DotrFuncDrawQ128 q[4]; float f[16]; } DotrFuncDrawMatrix;
typedef union { DotrFuncDrawQ128 q[9]; unsigned char bytes[0x90]; } DotrFuncDrawEffect;
typedef struct {
    signed char b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
} DotrFuncDrawObjectFlags;
typedef struct {
    unsigned char b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
} DotrFuncDrawFlags;
typedef struct {
    unsigned char base:2, overlay:2, other:4;
} DotrFuncDrawTextureFlags;
typedef struct {
    DotrFuncDrawVector position;
    DotrFuncDrawVector extra;
    int rotation[4];
    DotrFuncDrawVector scale;
} DotrFuncDrawTransform;
typedef struct {
    int model;
    DotrFuncDrawFlags flags0, flags1;
    unsigned char pad06[2];
    DotrFuncDrawSlot *slot0;
    float value0;
    DotrFuncDrawSlot *slot1;
    float value1, value2, value3;
    unsigned char pad20[0xC];
    int depth;
    float values[4];
    unsigned char pad40[0x10];
} DotrFuncDrawModel;
typedef struct {
    unsigned short count, type;
    int pad04;
    unsigned long long tag;
} DotrFuncDrawHeader;
typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrFuncDrawXyzf;
typedef struct {
    unsigned short low : 5;
    unsigned short flag : 1;
    unsigned short type : 3;
    unsigned short high : 7;
} DotrFuncDrawPrimitiveFlags;
typedef struct {
    DotrFuncDrawHeader header;
    unsigned char r, g, b, a;
    float q;
    float s0, t0;
    DotrFuncDrawXyzf v0;
    float s1, t1;
    DotrFuncDrawXyzf v1;
    unsigned long long extra[4];
} DotrFuncDrawPrimitive;
typedef struct {
    DotrFuncDrawHeader header;
    unsigned char r, g, b, a;
    float q;
    DotrFuncDrawXyzf v0;
    DotrFuncDrawXyzf v1;
    unsigned char tail[4];
} DotrFuncDrawSprite;

extern DotrFuncDrawVector data_00292fd0;
extern DotrFuncDrawVector data_00292fe0;
extern int gSzModel_nBlurMode_word;
extern int data_0031bb10;

/* Callees (func_001a23b0 is the float-to-integer helper in the library block). */
int func_001a23b0(float arg_a);
int SzModel_LoadTexture_Pipe(int current, int modelId, void *modelBaseAddr, int *size);
int lSzModel_GetGNode(int modelId, int gNodeId, int *maxDepth);
int SzEffect_IsUse2(int arg0, int arg1);
void *SzView_SetFog_Pipe(void *arg0, int arg1, int arg2, float arg3);
int SzView_SetLight_Pipe(int arg0, int arg1, int arg2);
int SzView_InqPointLight(int arg0, int arg1);
int SzView_IsFogZ(int arg0, int arg1);
int SzView_IsFogY(int arg0, int arg1);
int Ex3d_GetSampleZ(void *arg0, float fparg0);
int ExSortPrim_Pipe(int arg0, int arg1, int arg2, int arg3);
void *ExDitherMode_Pipe(void *arg0, int arg1);
int ExAssignAlpha(void *arg0, long long arg1);
long long ExGetAlphaVal(int arg0);
void *RhBase_MaskFBuffer(void *arg0, unsigned int arg1);
int RhBase_SetZBufferMode(int arg0, int arg1);
int RhBase_AssignRenderFrame(int arg0, int arg1, int arg2);
void *RhBase_EnableRenderFrame(void *arg0);
void *RhBase_DisableRenderFrame(void *arg0);
void *RhBase_ClearRenderFrame(unsigned char *packet, unsigned char *color);
void RhBase_FreeRenderFrame(void);
int RhBase_GetTextureInfoForRender(void *arg0, void *arg1);
void *RhMem_CopyMemory16(void *dest, void *source, int count);
void *RhMem_FillMemory4(void *dst, int val, int count);
void RhGeom_PushMatrix(int arg0);
void RhGeom_PopMatrix(void);
void RhGeom_MulMatrix(int arg0, void *arg1);
void RhGeom_MulMatrixRev(int arg0, int arg1);
void RhGeom_InvertMatrix(void *dst, void *src);
void RhGeom_GetTransMatrix(void *matrix, float *arg1);
void RhGeom_GetScaleRotXYZTransMatrix(void *outMatrix, void *scale, void *rot, void *pos);
void RhGeom_ScaleRotXYZTransMatrix(void *dst, register float *scale, int *rot, register float *trans);
void RhGeom_MulMatrixVector(void *arg0, void *arg1, void *arg2);
int RhView_GetCurView(void *arg0);
int RhView_SetClipZ(float fparg0, float fparg1);
int RhView_Modify(void *arg0);
int RhView_PushStack(void);
int RhView_PopStack(void);
void RhView_GetWorldViewMatrix(void *arg0);
int RhView_SendViewParameters(int arg0);
int RhText_SetTextureFlush(int arg0);
void *RhModel_Draw(void *arg0, void *arg1);
void GetAnyCoordinateInit(void);
int BookingGetAnyCoordinate(int id);
int GetAnyCoordinate(int arg0, void *arg1);

/* 0x1ee6c0: draws model modelId (0..0x53) at cursor arg0 with optional position
 * (translationRelated), rotation (rotationRelated) and scale (scaleRelated) offsets, an optional
 * packet header (arg5), a depth word or out pointer (zbufferPtr) and the callbacks
 * preRenderCallback and postRenderCallback; returns the advanced cursor, or arg0 when nothing is
 * drawn. Compiled with `#pragma global_optimizer off`. */
int SzModel_DrawWithFunc_Pipe(int arg0, int modelId, int translationRelated, int rotationRelated, int scaleRelated,
                              int arg5, int zbufferPtr, void (*preRenderCallback)(void), int postRenderCallback);

#endif
