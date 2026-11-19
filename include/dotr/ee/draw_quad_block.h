#ifndef DOTR_EE_DRAW_QUAD_BLOCK_H
#define DOTR_EE_DRAW_QUAD_BLOCK_H

#include "dotr/ee/draw_state.h"

/* SzDuel_GetCardNodeOnScreen: the draw block as this routine uses it (owner_draws.h names the
 * same layout DotrOwnerDrawBlock and declares this routine with that type, and
 * small_state.h declares Ex3d_ViewClip with an int second argument, so this
 * header is included only by draw_state sources that need it and tests/draw_state.c).
 * The packet at +0x48, the vertex pointer at +0x78 and the bitfields at +0x80
 * are as accessed; the word at 0x317490+3156 is read as an int through this
 * group's view of that storage. Signature as declared by owner_draws.h. */
struct DotrDrawQuadAngles {
    int x, y, z, w;
};
struct DotrDrawQuadVector {
    float x, y, z, w;
};
struct DotrDrawQuadBlock {
    float pos[4];
    struct DotrDrawQuadAngles turn;
    int rot[4];
    struct DotrDrawQuadScale scale;
    unsigned char unknown_38[0x10];
    struct DotrDrawPacket packet;
    unsigned char unknown_58[0x20];
    struct DotrDrawQuadVertex *vertices;
    int unknown_7c;
    short unknown_80_bits_0_11 : 12;
    short flag_12 : 1;
    short flag_13 : 1;
    short flag_14 : 1;
    short flag_15 : 1;
    short count : 12;
    unsigned char unknown_84[12];
};
/* The 64-bit word at +0x10 of a vertex, whose bits 32-55 the routine replaces. */
struct DotrDrawQuadVertexWord {
    long unknown_00[2];
    unsigned long unknown_10_low : 32;
    unsigned long unknown_10_colour : 24;
    unsigned long unknown_10_high : 8;
    long unknown_18[3];
};
extern int data_00317490_drawwords[];
/* Defined in small_state and view_state; argument types as passed here. */
extern int Ex3d_ViewClip(int a, struct DotrDrawQuadVertex *b, int c, int d, int e);
extern void Ex3d_NodeV2S(int a, int count, struct DotrDrawQuadVertex *v);
/* MATCHING DEVICE: the same routine as SzDuel_GetCardNodeUnitOnScreen (same address). Its
 * definition reproduces only with the order (mode, cx, out, scale, cy) and this
 * caller only with (mode, out, cx, cy, scale); both use the same argument
 * registers, and a cast at the call turns it into an indirect call. */
extern int func_001da5e0_caller(int mode, struct DotrDrawQuadVertex *out, float cx, float cy,
                                struct DotrDrawQuadScale *scale);
int SzDuel_GetCardNodeOnScreen(struct DotrDrawQuadBlock *block, int a, float x, float y);

/* SzDuel_GetCardNodeUnit (0x1da4c0): the unscaled quad of SzDuel_GetCardNodeUnitOnScreen centred
 * on the origin at z 0 (120x180, or 256x368 for a non-zero mode); returns 4. */
int SzDuel_GetCardNodeUnit(int mode, struct DotrDrawQuadVertex *out);
/* SzDuel_GetCardNode (0x1d9f00): the world-space variant of SzDuel_GetCardNodeOnScreen. Builds
 * the block's local matrix from the position at +0, the three angle words at +0x10 and the angle
 * block at +0x20 (two compositions chosen by flag_13), multiplies it into the world-view matrix,
 * transforms the 16-byte vector template at 0x291540, fills the packet and vertices and returns
 * 1 when the node's view vector and the transformed template have a positive inner product.
 * Assumed from the code: the member types of +0..+0x2f, the four angle templates at
 * 0x291550-0x291580 (16-byte objects copied whole; their contents are not asserted here) and
 * the callee argument types, which mostly repeat view_state.h / block_table.h where those
 * headers cannot be included (matrices as float pointers or the incomplete DotrViewMatrix). */
struct DotrViewVector;
struct DotrViewMatrix;
extern struct DotrDrawQuadVector data_00291540;
extern struct DotrDrawQuadAngles data_00291550, data_00291560, data_00291570, data_00291580;
extern void RhGeom_PushMatrix(float *matrix);
extern void RhGeom_PopMatrix(void);
extern void RhGeom_GetScaleRotXYZTransMatrix(float *outMatrix, float *scale, int *rot, float *pos);
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
extern void RhGeom_MulMatrixRev(float *m, float *other);
extern void RhView_GetWorldViewMatrix(struct DotrViewMatrix *out);
extern void RhGeom_MulMatrix(float *m, float *other);
extern void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v);
extern int Ex3d_NodeL2V(struct DotrViewMatrix *m, int count, struct DotrDrawQuadVertex *v, int a, int b, int c,
                        float *average);
extern int Ex3d_GetViewVectorForNode(struct DotrDrawQuadVector *origin, int count, struct DotrDrawQuadVertex *points,
                                     struct DotrDrawQuadVector *out);
extern float RhGeom_InnerProduct(struct DotrDrawQuadVector *a, struct DotrDrawQuadVector *b);
int SzDuel_GetCardNode(struct DotrDrawQuadBlock *block, int a);

#endif
