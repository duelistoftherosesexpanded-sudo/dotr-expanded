#ifndef DOTR_EE_MODEL_METHODS_H
#define DOTR_EE_MODEL_METHODS_H

/* The two drawing methods RhModel_Draw chooses between (lRhModel_DrawFastMethod at 0x20fa10,
 * lRhModel_DrawNormalMethod at 0x20f780): both change the VU1 draw method, call
 * lRhModel_DrawCommonPart and work on the scratchpad block at 0x70000000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The code addresses the scratchpad by literal addresses (0x700000b0 ...). The target build keeps
 * the literal; the host build points it at an object the test defines. Pointer-sized words overlap
 * on a 64-bit host, so the host layout is not the target's; the smoke test only observes calls and
 * flags. */
#ifdef __MWERKS__
#define DOTR_METHODS_SPR(off) (0x70000000 + (off))
#else
#include <stdint.h>
extern unsigned char dotr_model_methods_spr[0x200];
#define DOTR_METHODS_SPR(off) ((intptr_t)dotr_model_methods_spr + (off))
#endif

/* A packet cursor passed as an int (RhBase_SetZBufferMode): `int` on the target,
 * pointer-sized on the host. */
#ifdef __MWERKS__
typedef int DotrMethodsWord;
#else
typedef intptr_t DotrMethodsWord;
#endif

/* Flag bytes at +4/+5 of the model draw argument, as the two methods read them (struct
 * DotrModelDraw in model_draw.h is another view of the same object). Bit names are positions. */
struct DotrMethodsNormalFlags {
    unsigned char pad[4];
    unsigned char reserved4_0 : 1;
    unsigned char flag4_1 : 1;
    unsigned char flag4_2 : 1;
    unsigned char reserved4_3 : 5;
    unsigned char reserved5_0 : 2;
    unsigned char flag5_2 : 1;
    unsigned char flag5_3 : 1;
    unsigned char reserved5_4 : 4;
};
struct DotrMethodsFastFlags {
    unsigned char pad[4];
    unsigned char unused4_0 : 1;
    unsigned char flag4 : 1;
    unsigned char unused4_2 : 6;
    unsigned char unused5_0 : 2;
    unsigned char flag5 : 1;
    unsigned char unused5_3 : 5;
};

/* gRhModel_mlsmSwapArea (0x32a610 in this executable): a save area for the first 0x110 bytes of
 * the scratchpad. Size unknown. */
extern unsigned char gRhModel_mlsmSwapArea[];

/* Callees; other groups declare them with their own types. */
void RhGeomEx_SetMatrixForPointLight(void);
void RhGeomEx_SetMatrixForStraightLight(void *arg0);
void *RhVu1Mac_ChangeDrawMethod(void *arg0, int arg1);
void *RhVu1Mac_ReserveChangeDrawMethod(void *arg0);
void lRhModel_DrawCommonPart(void *arg0);
void *RhMem_CopyMemory16(void *dest, void *source, int count);
DotrMethodsWord RhBase_SetZBufferMode(DotrMethodsWord arg0, int arg1);
unsigned char *RhModelCpu_DrawAlpha(unsigned char *arg0, int arg1, int arg2, int arg3);
void RhView_GetViewScreenMatrix(void *dst);
void lRhModel_ChangeL2VtoL2S(int arg0, int arg1, int arg2);

/* 0x20f780: lit drawing method; returns the packet cursor after the model. */
void *lRhModel_DrawNormalMethod(void *modelBaseAddr, int arg1);
/* 0x20fa10: unlit drawing method with the view-screen matrix as the light matrix. */
void *lRhModel_DrawFastMethod(void *baseFileHeader);

#endif
