#ifndef DOTR_EE_LK_HELPERS_H
#define DOTR_EE_LK_HELPERS_H

/* Lk* helpers at 0x103260-0x104520: saturating counters, three-float vector arithmetic,
 * coordinate reset, flag test and text-load requests (names from the Japanese build).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Three consecutive floats; the functions touch nothing past +8. */
struct DotrLkVector {
    float x;
    float y;
    float z;
};

/* Three rows of four words at +0x00, +0x10 and +0x20. LkInitCoordinate writes the
 * first row as the bit pattern of 1.0f and clears the others; the row meanings
 * are not established. */
struct DotrLkCoordinate {
    int rows[3][4];
};

/* Declared in small_misc.h. */
extern int SzText_LoadRequest(int group, int id);

/* The second argument is not touched and reaches SzText_LoadRequest unchanged
 * (assumed to be a parameter; the Japanese prototype is not available). */
int LkInitTex2(int *handle, int id);
int LkInitTexBtl(int *handle, int id);
int LkIncrFLOAT(float *value, float limit, float step);
int LkDecrBYTE(unsigned char *value, unsigned char limit, unsigned char step);
int LkDecrINT(int *value, int limit, int step);
/* Counterparts of LkDecrBYTE and LkDecrINT that step up to the limit, on a byte and on an int. */
int LkIncrBYTE(unsigned char *value, unsigned char limit, unsigned char step);
int LkIncrINT(int *value, int limit, int step);
int LkDecrFLOAT(float *value, float limit, float step);
void LkAddVector(struct DotrLkVector *target, struct DotrLkVector *other);
void LkSubVector(struct DotrLkVector *target, struct DotrLkVector *other);
void LkMulVector(struct DotrLkVector *target, struct DotrLkVector *other);
/* Per-component versions of LkIncrFLOAT/LkDecrFLOAT: bit n of the result is set while
 * component n is still stepping. Parameter and result types are assumed from the code. */
int LkIncrVec(struct DotrLkVector *value, float limit, float step);
int LkDecrVec(struct DotrLkVector *value, float limit, float step);
/* out = from + (to - from) / 2; the three-float layout is assumed from the offsets used. */
void LkGetCenter(struct DotrLkVector *from, struct DotrLkVector *to, struct DotrLkVector *out);
/* Moves value one of `steps` equal parts towards target, or copies target when steps is 0. */
void LkOBJ_TransitVector(struct DotrLkVector *value, struct DotrLkVector *target, int steps);
void LkInitCoordinate(struct DotrLkCoordinate *coordinate);
int LkCheckFlag(int flags, int bit);

/* 0x105830 LkAssignUV2. Layouts from the code, member names descriptive (assumed):
 * four 24-byte vertex records from +24 whose first two halfwords hold 14-bit fields (top two
 * bits kept), and an animation block of 16-bit values with a flag word at +56. */
struct DotrLkUvVertex {
    unsigned short u : 14;
    unsigned short u_high : 2;
    unsigned short v : 14;
    unsigned short v_high : 2;
    unsigned char unknown_04[20];
};

struct DotrLkUvPacket {
    unsigned short unknown_00;
    unsigned short flags;
    unsigned char unknown_04[20];
    struct DotrLkUvVertex vertex[4];
};

struct DotrLkUvAnim {
    short u;
    short v;
    short w;
    short h;
    short u0;
    short v0;
    short sheet_w;
    short sheet_h;
    unsigned char unknown_10[40];
    int flags;
    unsigned char unknown_3c[12];
    short wait;
    short wait_max;
    short frame_max;
    short loop_frames;
    short frame;
    short state;
};
/* Computes the current cell (u, v) of a sprite sheet from the frame number (row-major when
 * flags bit 1 is set, column-major otherwise), sets bit 11 of the packet halfword at +2 and
 * writes the four corners as 12.4 texel positions: the upper half of the cell for mode 0,
 * the lower half otherwise. Mode 1 then advances the wait and frame counters. */
void LkAssignUV2(struct DotrLkUvPacket *packet, struct DotrLkUvAnim *anim, int mode);

/* 0x108500 POLPAN_UVIN_ (name from the Japanese build's symbols). `rect` is four halfwords
 * {u, v, w, h}; the corners (u, v), (u + w, v), (u, v + h), (u + w, v + h) are written as 12.4
 * texel positions into the 14-bit fields of the four vertex records, top two bits kept.
 * Vertex order by mode: 0 -> 0,1,2,3; 1 -> 1,3,0,2; 10 -> 2,0,3,1; any other -> 1,0,3,2.
 * Assumed: the packet layout shared with LkAssignUV2, `rect` as short[4], void result. */
void POLPAN_UVIN_(struct DotrLkUvPacket *packet, short *rect, int mode);

#endif
