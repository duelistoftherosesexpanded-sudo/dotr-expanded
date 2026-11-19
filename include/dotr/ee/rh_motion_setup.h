#ifndef DOTR_EE_RH_MOTION_SETUP_H
#define DOTR_EE_RH_MOTION_SETUP_H

/* lRhModel motion setup entry points that fill the scratchpad header and run their recursive
 * node walk with the stack switched to the scratchpad at 0x70004000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The code addresses the scratchpad by literal addresses. The target build keeps the literal; the host build
 * points it at an object the test defines (pointer members are wider there, so the host layout
 * differs). */
#ifdef __MWERKS__
#define DOTR_MOTION_SPR(off) (0x70000000 + (off))
#else
#include <stdint.h>
extern unsigned char dotr_rh_motion_setup_spr[0x200];
#define DOTR_MOTION_SPR(off) ((intptr_t)dotr_rh_motion_setup_spr + (off))
#endif

#ifndef NULL
#define NULL 0
#endif

/* Scratchpad header views as the entry points write them: the model data pointer at +0x90,
 * motion words, frames and values after it, a halfword count at +0xe0; lRhModel_SetupNoMotion
 * also copies a vector through the pointer at +0xb8 when it is set. Names describe use. */
struct DotrMotionVec4 {
    float x, y, z, w;
};
struct DotrMotionNoSpr {
    unsigned char pad00[0x90];
    unsigned char *data;
    unsigned char pad94[0xB8 - 0x94];
    struct DotrMotionVec4 *dst;
};
struct DotrMotionMultiSpr {
    unsigned char pad00[0x90];
    unsigned char *data;
    int motion94;
    float frame98;
    int count9C;
    int motionA0;
    float frameA4;
    float valueA8;
    float valueAC;
    unsigned char padB0[0xE0 - 0xB0];
    unsigned short countE0;
};
/* A motion record: a halfword count at +4 and a pointer to its key data at +8. */
struct DotrMotionMulti {
    unsigned char pad0[4];
    unsigned short count;
    unsigned char pad6[2];
    int *motion;
};

/* gRhModel_dwStack (0x32a720): the stack pointer saved while a walk runs on the scratchpad;
 * data_70004000_rhmotion: the scratchpad stack top (only named in the inline blocks);
 * gRhModel_vecScale (0x295070): the initial scale vector handed to each walk. */
extern void *gRhModel_dwStack;
extern unsigned char data_70004000_rhmotion[];
extern unsigned char gRhModel_vecScale[];

/* The recursive walks, defined in model_setup. */
void lRhModel_SetupNoMotionRecursive(void *arg0, void *arg1, void *arg2);
int lRhModel_SetupMultiMotionRecursive(void *arg0, int depth, void *arg2);
int lRhModel_SetupFloatMotionRecursive(void *arg0, int depth, void *arg2);
int lRhModel_SetupPulseMotionRecursive(void *arg0, int depth, void *arg2);

/* 0x20ff80, 0x210260, 0x2108c0, 0x2114b0: fill the scratchpad header from the model (data at
 * model + word +0x10) and motion, run the walk on the scratchpad stack, return 1. */
int lRhModel_SetupNoMotion(void *arg0);
int lRhModel_SetupMultiMotion(void *arg0, void *arg1, void *arg2, float frame, float frame2, float value, float value2);
int lRhModel_SetupFloatMotion(unsigned char *model, unsigned char *motion, float time);
int lRhModel_SetupPulseMotion(unsigned char *model, unsigned char *motion, float time);

#endif
