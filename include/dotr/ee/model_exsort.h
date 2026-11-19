#ifndef DOTR_EE_MODEL_EXSORT_H
#define DOTR_EE_MODEL_EXSORT_H

/* SzModel_ExSort_Pipe (0x1ef8a0): the model draw wrapper that brackets SzModel_Draw_Pipe with
 * dither, Z-buffer and clamp packets and sorts the result by the model's sampled depth.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Cursors and packet addresses are ints in this group (the callers pass them through
 * registers; other groups view the same callees with pointer types). The four-float vector
 * is the 16-byte local SzModel_GetCenter fills. Callee prototypes are assumed
 * from the argument registers; func_001a23b0 is the float-to-int helper in the library block.
 * gSzModel_nBlurMode: 0 draws the motion blur first, 2 moves the depth sample by half the
 * model's radius. */
struct DotrExSortVec {
    float x, y, z, w;
};
extern int gSzModel_pfhCache;
extern int gSzModel_nBlurMode;
extern int ExSetAnchor_Pipe(int);
extern int ExDitherMatrixEz_Pipe(int, int);
extern int RhBase_SetZBufferMode(int, int);
extern int SzModel_DrawMotionBlur_Pipe(int, int);
extern int SzModel_Draw_Pipe(int cursor, int modelId, int c, int d, int e, int f, int g);
extern long ExGetRegVal(int, int);
extern int ExClampMode_Pipe(int, long);
extern int SzModel_GetCenter(int, struct DotrExSortVec *);
extern int SzModel_GetMaxOT(int);
extern float SzModel_GetRadius(int);
extern float Ex3d_GetSampleW(struct DotrExSortVec *, float);
extern int ExGetMyOT(void);
extern int func_001a23b0(float);
extern int ExSortAnchor_Pipe(int, int, int, int);
int SzModel_ExSort_Pipe(int cursor, int index, int c, int d, int e, int f, int g);

#endif
