#ifndef DOTR_EE_BATTLE_FAKES_H
#define DOTR_EE_BATTLE_FAKES_H

/* lSzBattle_DrawFake* functions at 0x1e6650-0x1e7798: battle-stage passes that render models into
 * render frame (0, 10, 49) and sort the result as textured primitives (mirror, shadow).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

#include "dotr/ee/title_draws.h"

#ifndef NULL
#define NULL 0
#endif

/* The types of the render-to-texture sprite packet, the view block and the cursor word come
 * from title_draws.h (lSzDuel_DrawWindow_FieldPiece_Pipe builds the same packet). */

/* The mirror's four corners at 0x292310, copied with four lq/sq pairs. */
struct DotrFakeQuad {
    struct DotrViewVector p[4];
};
/* RhBase_GetTextureInfoForRender's first word: only the bit cleared afterwards (bit 34) is
 * known. */
struct DotrFakeTex {
    unsigned long low : 34;
    unsigned long flag : 1;
};
/* The 80-byte sprite packet at sp+416: DotrCursorSlotRectPacket without the eight bytes in
 * front of the header, which this frame does not have. */
struct DotrFakePacket {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorSlotRectVertex vertex[2];
    long unknown_38;
    struct DotrFakeTex tex0;
    struct DotrCursorTex tex1;
};
/* Only the byte read at 0x46 of SzDuel_GetUnit's result (unsigned load), as battle_draws.h. */
struct DotrFakeUnit {
    char unknown_00[0x46];
    unsigned char unknown_46;
};
/* The two intensity locals take 16 bytes each in the frame although 12 are copied; the
 * fourth word is never accessed. */
struct DotrFakeIntensity {
    struct DotrTitleIntensity v;
    float unknown_0c;
};

/* sTrimX$357, sTrimY$358, sFlipX$359 and sFlipY$360 in the Japanese build's symbols (function
 * statics; the names are not C identifiers). Element type from the lh/sh accesses. */
extern short data_00292360[4];
extern short data_00292368[4];
extern short data_00292370;
extern short data_00292378;
/* Words at 0x31ab88 (state_grid.h and battle_draws.h have the same declaration). */
extern int gSzBattle_bl[];

/* Assumed prototypes: pointer types chosen for this caller. func_001a22c0 (library block) is
 * assumed to convert a float to an int and func_0019a5c8 (library block, no original name) to
 * return the next pseudo-random number, as in fx_calc.h. */
extern int Ex3d_GetPointsAverage(int count, struct DotrViewVector *items, struct DotrViewVector *out);
extern void *SzView_GetSP(int index);
extern int SzGame_GetFrameCount(void);
extern int func_001a22c0(float value);
extern int func_0019a5c8(void);
extern int SzModel_GetPauseFlag(int modelId);
extern DotrDrawWord ExDitherMode_Pipe(DotrDrawWord cursor, int value);
extern struct DotrFakeUnit *SzDuel_GetUnit(int side, int pos);
extern int SzModel_GetRgba(int modelId, unsigned char *out);
extern int SzModel_SetRgba(int index, int value, unsigned char *in);
extern int SzModel_GetIntensityOnly(int index, float *out);
extern int SzModel_SetIntensityOnly(int index, float value);

/* Types of lSzBattle_DrawFakeMirror: the mirror quad at 0x292310 (four float[4] corners), the
 * light intensity at 0x292350 (three floats), and the 88-byte sprite packet with its GIF tag
 * view, UV view and bit fields. Field meanings are assumed.
 * DotrFakePacket above views the same packet for the host test. */
struct DotrFakeMirrorQuad {
    float points[4][4];
};
struct DotrFakeMirrorVec3 {
    float x;
    float y;
    float z;
};
struct DotrFakeMirrorTag {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
};
struct DotrFakeMirrorUv {
    unsigned long long u : 14;
    unsigned long long pad0 : 2;
    unsigned long long v : 14;
    unsigned long long pad1 : 34;
};
struct DotrFakeMirrorVertex {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
};
struct DotrFakeMirrorTex0 {
    unsigned long long tbp : 14;
    unsigned long long tbw : 6;
    unsigned long long psm : 6;
    unsigned long long tw : 4;
    unsigned long long th : 4;
    unsigned long long tcc : 1;
    unsigned long long rest : 29;
};
struct DotrFakeMirrorPacket {
    unsigned long long tag[2];
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    float q;
    unsigned long long uv0;
    struct DotrFakeMirrorVertex xyz0;
    unsigned long long uv1;
    struct DotrFakeMirrorVertex xyz1;
    unsigned long long test;
    struct DotrFakeMirrorTex0 tex0;
    unsigned long long tex1;
    unsigned long long clamp;
};

extern struct DotrFakeMirrorQuad data_00292310;
extern struct DotrFakeMirrorVec3 data_00292350;

/* Views of the callees, linked by address; the host test forwards each to the stub of the real
 * name. An int that carries an address (SzView_GetSP's and SzDuel_GetUnit's results) is
 * DotrDrawWord, int for the target. */
extern int func_00206030_fakes(int count, void *arg1, void *arg2);          /* Ex3d_GetPointsAverage */
extern int func_00205e70_fakes(int count, void *arg1, float *normal);       /* Ex3d_GetSurfaceNormal */
extern int func_002060f0_fakes(float *origin, int count, float *points, float *out); /* Ex3d_GetViewVector */
extern DotrDrawWord func_001f9ad0_fakes(int arg0);                          /* SzView_GetSP */
extern float func_0020d1a0_fakes(float *arg0, float *arg1);                 /* RhGeom_InnerProduct */
extern int func_0020e520_fakes(float *point, float *normal, void *output);  /* RhView_PushFakeMirror */
extern int func_001a8720_fakes(void);                                       /* SzGame_GetFrameCount */
extern float func_0020d0e0_fakes(float *arg0);                              /* RhGeom_GetVectorScalor */
extern int func_001f3360_fakes(int arg0);                                   /* SzModel_GetPauseFlag */
extern void *func_00208530_fakes(void *arg0, int arg1);                     /* ExDitherMode_Pipe */
extern void *func_0020a610_fakes(void *arg0);                               /* RhBase_EnableRenderFrame */
extern void *func_0020a870_fakes(unsigned char *packet, unsigned char *color); /* RhBase_ClearRenderFrame */
extern int func_0020e360_fakes(void *arg0);                                 /* RhView_Modify */
extern int func_0020ec60_fakes(int arg0);                                   /* RhView_SendViewParameters */
extern void *func_00209f70_fakes(void *arg0, unsigned int arg1);            /* RhBase_MaskFBuffer */
extern int func_0020a0b0_fakes(int arg0, int arg1);                         /* RhBase_SetZBufferMode */
extern int func_001efa80_fakes(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6); /* SzModel_DrawWithOutCalc_Pipe */
extern DotrDrawWord func_001df540_fakes(int arg0, int arg1);                /* SzDuel_GetUnit */
extern int func_001f1370_fakes(int arg0);                                   /* SzModel_IsModelDisp */
extern int func_001f2660_fakes(int arg0, void *arg1);                       /* SzModel_GetRgba */
extern int func_001f26e0_fakes(int arg0, int arg1, void *arg2);             /* SzModel_SetRgba */
extern int func_001f1e80_fakes(int arg0, float *arg1);                      /* SzModel_GetIntensityOnly */
extern int func_001f1f70_fakes(int arg0, float fparg0);                     /* SzModel_SetIntensityOnly */
extern int func_001f1e10_fakes(int arg0, float *arg1);                      /* SzModel_GetIntensity */
extern int func_001f1ee0_fakes(int arg0, float *arg1);                      /* SzModel_SetIntensity */
extern int func_002086d0_fakes(void *arg0, long long arg1);                 /* ExAssignAlpha */
extern int func_0020aa30_fakes(void *arg0, void *arg1);                     /* RhBase_GetTextureInfoForRender */
extern void *func_0020a760_fakes(void *arg0);                               /* RhBase_DisableRenderFrame */
extern int func_0020f290_fakes(int arg0);                                   /* RhText_SetTextureFlush */
extern int func_0020e970_fakes(void *arg0);                                 /* RhView_PopFakeMirror */
extern void func_0020a9e0_fakes(void);                                      /* RhBase_FreeRenderFrame */

/* 0x1e6650: the first argument is the display-list cursor, the second a model index (both
 * from their uses). */
int lSzBattle_DrawFakeMirror(int cursor, int index);

#endif
