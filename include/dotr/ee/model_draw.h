#ifndef DOTR_EE_MODEL_DRAW_H
#define DOTR_EE_MODEL_DRAW_H

/* Functions of the model drawing path that prepare the scratchpad work block at 0x70000000
 * (lRhModel_DrawCommonPart at 0x20faf0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

#include "dotr/ee/patch_lists.h"

/* An address held in a word: 32 bits on the target. */
#ifdef __MWERKS__
typedef int DotrModelAddress;
#else
#include <stdint.h>
typedef intptr_t DotrModelAddress;
#endif
/* A 16-byte word moved with lq/sq. */
#ifdef __MWERKS__
typedef __int128 DotrModelQuad;
#else
__extension__ typedef __int128 DotrModelQuad;
#endif

/* Words of the header the scratchpad points at, as 0x20faf0 reads them: offset and size of two
 * stretches of the loaded block, a float at +0x54 and a count at +0x58. Names describe use. */
struct DotrModelHead {
    int unknown_00[2];
    int first_offset;
    int first_size;
    int unknown_10[2];
    int second_offset;
    int second_size;
    int unknown_20[4];
    int bind_list;         /* +0x30: 0x20fed0's list for the first stretch */
    int unknown_34;
    int bind_list_second;  /* +0x38: 0x20fed0's list for the second stretch */
    int unknown_3c[3];
    int option_table;      /* +0x48: 0x212170's table, zero when absent */
    int option_enabled;    /* +0x4c: 0x212170 does nothing when zero */
    int unknown_50;
    float unknown_54;
    int unknown_58;
};
/* A four-float vector moved whole with lq/sq (0x20f4e0). */
struct DotrModelVector {
    float x, y, z, w;
};
/* What 0x20f4e0 reads of the loaded block: the head's first word is the offset of a record
 * whose vector at +0x30 holds a centre and, in w, a radius; the word after the first
 * stretch's start carries a 28-bit count. */
struct DotrModelNode {
    unsigned char unknown_00[0x30];
    struct DotrModelVector sphere;
};
struct DotrModelStretch {
    int unknown_00;
    unsigned int skip : 28;
};
/* The scratchpad at 0x70000000 as 0x20faf0 uses it (item_walks.h has another routine's view of
 * the word at +0xfc, which lies inside `buffer` here). Every access in the original is absolute
 * (lui at,0x7000), so the target build uses a literal address; the host build points the name at
 * an object the test defines. Sizes of the members not touched are unknown. */
struct DotrModelPad {
    struct DotrModelVector work;      /* +0x00: 0x20f4e0 has it filled by a copy or a product */
    unsigned char unknown_10[0x10];
    struct DotrModelVector centre;    /* +0x20: copied from the node, w set to 1 */
    float light[4];                   /* +0x30: three outputs of RhVu1Mac_GetPointLight;
                                         [1] is added to the radius */
    unsigned char unknown_40[0x10];
    struct DotrModelVector light_at;  /* +0x50 */
    unsigned char unknown_60[0x50];
    int unknown_b0;
    unsigned char *unknown_b4;
    int unknown_b8;
    float unknown_bc;
    int unknown_c0;
    unsigned char *first;
    DotrModelAddress *second;
    unsigned char *third;
    unsigned char *fourth;
    int unknown_d4;
    int skip;
    struct DotrModelHead *head;
    unsigned char unknown_e0[0x10];
    unsigned char buffer[16];
};
#ifdef __MWERKS__
#define DOTR_MODEL_PAD ((struct DotrModelPad *)0x70000000)
#else
extern struct DotrModelPad dotr_model_pad;
#define DOTR_MODEL_PAD (&dotr_model_pad)
#endif
/* Argument of 0x20faf0: a block pointer, two bytes of one-bit flags (bit numbers from the
 * shifts; names describe what each selects here), and words and floats forwarded to callees. */
struct DotrModelDraw {
    struct DotrPatchHead *block;
    unsigned char bit_0 : 1;
    unsigned char bit_1 : 1;
    unsigned char motion : 1;
    unsigned char multi : 1;
    unsigned char floating : 1;
    unsigned char option : 1;
    unsigned char texture : 1;
    unsigned char intensity : 1;
    unsigned char light_0 : 1;
    unsigned char light_1 : 1;
    unsigned char bit_10 : 1;
    unsigned char zbuffer : 1;
    unsigned char own_matrix : 1;  /* 0x20f4e0: copy `matrix` instead of multiplying */
    int unknown_08;
    float unknown_0c;
    int unknown_10;
    float unknown_14;
    float unknown_18;
    float unknown_1c;
    int unknown_20;
    int unknown_24;
    int unknown_28;
    int sample_z;                  /* +0x2c: written by 0x20f4e0 */
    unsigned char unknown_30[0x20];
    struct DotrModelVector matrix; /* +0x50: one 16-byte row copied by 0x20f4e0 */
};

/* 0x20a0b0, declared with int words in cursor_packets.h; here the word is an address. */
extern DotrModelAddress RhBase_SetZBufferMode(DotrModelAddress cursor, int a);
/* Assumed prototypes: argument lists from the call sites in 0x20faf0, meanings unknown. */
extern void lRhModel_DynamicAddressBind(struct DotrPatchHead *block, unsigned char *first, unsigned char *second, int flag);
extern void lRhModel_SetupOptionObject(struct DotrPatchHead *block, unsigned char *first, unsigned char *second, int flag, int e);
extern void lRhModel_SetMaterialIntensity(unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *d,
                                          struct DotrModelDraw *draw);
extern void lRhModel_SetupNoMotion(struct DotrPatchHead *block);
extern void lRhModel_SetupMultiMotion(struct DotrPatchHead *block, int a, int b, float f0, float f1, float f2, float f3);
extern void lRhModel_SetupFloatMotion(struct DotrPatchHead *block, int a, float f);
extern void lRhModel_SetupPulseMotion(struct DotrPatchHead *block, int a, float f);

void lRhModel_DrawCommonPart(struct DotrModelDraw *draw);

/* 0x20f4e0. The cursor is an address held in a word; the callees are declared with their
 * own groups' types (draw_state.h, view_state.h) and declared here over this group's views.
 * 0x20ede0, 0x20f780 and 0x20fa10 take argument lists from these call sites. */
extern int require_coordinate_index; /* require_coordinate_index in the Japanese build */
extern DotrModelAddress RhVu1Mac_SetFogOnOff(DotrModelAddress cursor, int a, int b);
extern void RhGeom_PushMatrix(int unknown);
extern void RhGeom_PopMatrix(void);
extern void RhGeom_MulMatrixVector(struct DotrModelVector *out, void *m, struct DotrModelVector *v);
extern int RhView_GetSampleZ(struct DotrModelVector *in, float dz);
extern int RhVu1Mac_GetPointLight(struct DotrModelVector *a, struct DotrModelVector *b, float *c, float *d, float *e);
extern float RhGeom_GetPointsDistance(struct DotrModelVector *a, struct DotrModelVector *b);
extern int RhView_CheckViewVolume(struct DotrModelVector *centre, float radius);
extern DotrModelAddress lRhModel_DrawNormalMethod(struct DotrModelDraw *draw, int lit);
extern DotrModelAddress lRhModel_DrawFastMethod(struct DotrModelDraw *draw);
DotrModelAddress RhModel_Draw(DotrModelAddress cursor, struct DotrModelDraw *draw);

#endif
