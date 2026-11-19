#ifndef DOTR_EE_MODEL_SETUP_H
#define DOTR_EE_MODEL_SETUP_H

/* lRhModel_Setup*Recursive walkers (0x210010-0x211530): each walks a model's node list, pushes
 * a matrix per node, records the matrix of a requested coordinate in require_coordinate and
 * recurses into the child node.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* Pointer values are 32-bit ints on the target; the host type only keeps the cast lossless. */
#ifdef __MWERKS__
typedef int DotrSetupWord;
#else
#include <stdint.h>
typedef intptr_t DotrSetupWord;
#endif
/* Sixteen bytes moved whole with lq/sq (the sphere copy of the motion walkers). */
typedef struct {
#ifdef __MWERKS__
    __int128 q;
#else
    __extension__ __int128 q;
#endif
} DotrSetupQuad;

/* Three floats copied whole (ld + lwc1) from the constant at 0x295080. */
struct DotrSetupVec3 {
    float x, y, z;
};
/* Four floats copied member by member (the root position). */
struct DotrSetupVec4 {
    float x, y, z, w;
};
/* A model node as the walkers read it (64 bytes). child and next are offsets into the node
 * block; only their low 28 bits are used. */
struct DotrSetupNode {
    float scale[3];
    unsigned short flags;  /* bits 0-1: 3 = no matrix pushed; bit 0: no translation; bit 1: no rotation;
                              bit 2: no scaling; bit 15: no view-volume check (motion walkers) */
    unsigned short matrix; /* index of a 16-byte unit in the matrix block, 0 = none */
    float rot[3];
    unsigned int child;
    float trans[3];
    unsigned int next;
    struct DotrSetupVec4 sphere; /* centre and, in w, radius */
};
/* The scratchpad at 0x70000000 as the walkers use it: the first 48 bytes receive a copy of
 * the node (the motion item callees then overwrite scale, rotation and position in place).
 * lRhModel_SetupNoMotionRecursive gets this address as its first argument and reads only
 * nodes and matrices. model_draw.h and motion_items.h hold other routines' views. */
struct DotrSetupPad {
    float scale[3];
    unsigned short flags;
    unsigned short matrix;
    int rot[4];
    struct DotrSetupVec4 position;
    unsigned char unknown_30[96];
    char *nodes;           /* +144 */
    char *item0;           /* +148: motion items, 24 bytes each with motion_flags bit 2, else 16 */
    float frame0;          /* +152 */
    int count;             /* +156: nodes that have a motion item */
    char *item1;           /* +160 */
    float frame1;          /* +164 */
    float rate0;           /* +168 */
    float rate1;           /* +172 */
    int visible;           /* +176: RhView_CheckViewVolume results or-ed together */
    char *matrices;        /* +180 */
    struct DotrSetupVec4 *root; /* +184: receives the first node's position when not null */
    float rate;            /* +188 */
    unsigned char unknown_c0[32];
    unsigned short motion_flags; /* +224 */
};
#ifdef __MWERKS__
#define DOTR_SETUP_PAD ((struct DotrSetupPad *)0x70000000)
#else
extern struct DotrSetupPad dotr_setup_pad;
#define DOTR_SETUP_PAD (&dotr_setup_pad)
#endif
/* An 80-byte entry of require_coordinate (the Japanese build's name):
 * requested index, a flag set to 1 when the node was reached, and the matrix taken then.
 * small_misc.h has the same storage as struct DotrMiscEntry80. */
struct DotrSetupCoordinate {
    int index;
    int flag;
    unsigned char unknown_08[8];
    float matrix[16];
};
extern struct DotrSetupCoordinate require_coordinate[];
extern int require_coordinate_no[];  /* number of entries in use (compared unsigned) */
extern int require_coordinate_index;    /* require_coordinate_index in the Japanese build */
extern struct DotrSetupVec3 data_00295080; /* assumed: the initial value of the local translation */

extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
extern int *RhMem_CopyMemory4(int *destination, int *source, int count);
extern void RhGeomEx_PushMatrix(void);
extern void RhGeomEx_PopMatrix(void);
extern void RhGeomEx_GetMatrix(void *matrix);
/* RhGeom_ScaleMatrix (VU0 code). Assumed: matrix (null = current), then three scale factors. */
extern void RhGeom_ScaleMatrix(void *matrix, float *scale);
/* 0x20dbe0. Assumed: the first argument is a pointer that is always null here. */
extern void RhGeomEx_ScaleRotXYZTransMatrix(void *unknown, void *rot, void *trans);
extern void RhGeom_GetMatrix(float *matrix);
extern int RhView_CheckViewVolume(struct DotrSetupVec4 *centre, float radius);
extern int func_001a22c0(float value);
unsigned int lRhModel_GetMultiMotionItem(void *item0, void *item1, float frame0, float rate0, float frame1, float rate1,
                                         unsigned int flags, float *position, int *rotation, float *scale, float rate);
/* LIMITATION: the two callers below match only when the flags parameter of these callees is
 * an unsigned short, and for 0x210cf0 when it precedes the frame. The definition of 0x210cf0
 * (motion_items.h) matches only with (data, float frame, unsigned int flags, ...). The
 * argument registers are the same either way, but the two declarations cannot be included
 * together; the true prototype is unresolved. 0x2118f0 has the same question
 * (motion_items.h). */
unsigned int lRhModel_GetInterpolateMotionItem(void *data, unsigned short flags, float frame, float *position,
                                               int *rotation, float *scale, float rate);
unsigned int lRhModel_GetPulseMotionItem(void *data, int frame, unsigned short flags, float *position, int *rotation,
                                         float *scale, float rate);

/* 0x210010. Assumed: no return value; parent points at 16 bytes (three scale factors). */
void lRhModel_SetupNoMotionRecursive(struct DotrSetupPad *pad, struct DotrSetupNode *node, float *parent);
/* 0x210310, 0x210940, 0x211530. Assumed: index counts the nodes visited so far and the
 * result is the count after this chain; parent as above. */
int lRhModel_SetupMultiMotionRecursive(struct DotrSetupNode *node, int index, float *parent);
int lRhModel_SetupFloatMotionRecursive(struct DotrSetupNode *node, int index, float *parent);
int lRhModel_SetupPulseMotionRecursive(struct DotrSetupNode *node, int index, float *parent);

#endif
