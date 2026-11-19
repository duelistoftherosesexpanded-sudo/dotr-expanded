#ifndef DOTR_EE_EX3D_CALC_H
#define DOTR_EE_EX3D_CALC_H

/* Ex3d vertex transform-and-perspective and clip-plane setup routines that read the RhView
 * screen, clip and world-screen parameters.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/math-prims.md. */

/* Addresses that the code keeps in 32-bit integers are `int` on the target and pointer-sized on
 * the host. */
#ifdef __MWERKS__
typedef int DotrEx3dCalcWord;
/* A 16-byte aligned home for a spilled parameter; GCC rejects it on a parameter. */
#define DOTR_EX3D_CALC_ALIGN16 __attribute__((aligned(16)))
#else
#include <stdint.h>
typedef intptr_t DotrEx3dCalcWord;
#define DOTR_EX3D_CALC_ALIGN16
#endif

#ifndef NULL
#define NULL 0
#endif

/* ASSUMED from their use: a vertex packet header (count of four-bit entry types, flag bits
 * 0x800/0x20/0x10, the packed type word) followed by eight-byte entries; an entry of type 4
 * receives the screen coordinates as two shorts and a 24-bit depth. */
struct DotrEx3dCalcPacket {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long types;
    unsigned char data[1];
};
struct DotrEx3dCalcPacked {
    unsigned long long xy : 32;
    unsigned long long z : 24;
    unsigned long long fog : 8;
};

/* The clip-plane table at 0x325cc0 (gEx3d_aPlane): six planes of five floats (normal, distance,
 * normal length) written by Ex3d_InitClipPlane; a byte view. */
extern unsigned char data_00325cc0_ex3dcalc[];

/* Callees. RhGeom_MulMatrixVectorArray is declared without a prototype (its arguments are passed as
 * integers); it and the matrix stack routines are assembly routines in vu0, the RhView routines are
 * declared in view_state.h, func_001a6c60 (square root) lies in the library block. func_001a23b0
 * (float to integer) is declared inside Ex3d_RotTransPersAutoCalcQ. */
void RhView_GetWorldScreenMatrix(void *matrix);
void RhGeom_PushMatrix(DotrEx3dCalcWord matrix);
void RhGeom_MulMatrixVectorArray();
void RhGeom_PopMatrix(void);
int RhView_GetClipZ(float *near, float *far);
int RhView_GetScreenCenter(float *x, float *y);
int RhView_GetScreenDist(float *dist);
float func_001a6c60(float x);

/* 0x2058d0: transforms the packet's type-4 vertices by `matrix` (or the world-screen matrix),
 * divides by w, writes 16x screen x/y and depth into the entries; 0 when a vertex is off screen. */
int Ex3d_RotTransPersAutoCalcQ(void *src DOTR_EX3D_CALC_ALIGN16, float *vertices, DotrEx3dCalcWord matrix,
                               DotrEx3dCalcWord packet, DotrEx3dCalcWord average);
/* 0x206830: builds the six view-frustum planes at 0x325cc0 from the screen and clip parameters. */
void Ex3d_InitClipPlane(void);

#endif
