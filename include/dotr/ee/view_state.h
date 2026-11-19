#ifndef DOTR_EE_VIEW_STATE_H
#define DOTR_EE_VIEW_STATE_H

/* Offset views for accessors of the 416-byte entries at EE 0x32a2a0 selected
 * by the word at 0x32a240, plus five four-float vector routines. Meanings are
 * unassigned. Evidence: docs/modules/render-files.md. */
#ifdef __MWERKS__
typedef __int128 DotrViewWord128;
#else
__extension__ typedef __int128 DotrViewWord128;
#endif

struct DotrViewVector {
    float x, y, z;
    float unknown_w;
};

struct DotrViewQuad {
    unsigned char unknown_0;
    unsigned char unknown_1;
    unsigned char unknown_2;
    unsigned char unknown_3;
};

/* Nine 128-bit words copied as a block. */
struct DotrViewBlock {
    DotrViewWord128 unknown_words[9];
};

/* Sixteen words copied as a block by 0x20eb40, 0x20eba0 and 0x20ec00. */
struct DotrViewMatrix {
    float unknown_values[16];
};

struct DotrViewEntry {
    struct DotrViewMatrix unknown_000;
    struct DotrViewMatrix unknown_040;
    struct DotrViewMatrix unknown_080;
    struct DotrViewBlock unknown_0c0;
    float unknown_150;
    float unknown_154;
    float unknown_158;
    float unknown_15c;
    float unknown_160;
    float unknown_164;
    float unknown_168;
    float unknown_16c;
    float unknown_170;
    float unknown_174;
    float unknown_178;
    float unknown_17c;
    float unknown_180;
    struct DotrViewQuad unknown_184;
    unsigned char unknown_188[0x18];
};

typedef char DotrViewEntryMustBe1a0[(sizeof(struct DotrViewEntry) == 0x1a0) ? 1 : -1];

extern struct DotrViewEntry gRhView_avl[];
extern int gRhView_nStackIndex[];

/* Prefix view of EE 0x32a250; only the word at +0x44 is observed. */
struct DotrViewControl {
    unsigned char unknown_00[0x44];
    int unknown_44;
};

extern struct DotrViewControl gRhView_FakeMirror[];

void RhGeom_NormalizeVector(struct DotrViewVector *v);
/* VU0 assembly (asm/ee/vu0); only a0 is read and its non-null path stores through it. */
extern void RhGeom_UnitMatrix(struct DotrViewMatrix *m);
/* Loads the matrix then copies the vector into elements 12..14. */
void RhGeom_GetTransMatrix(struct DotrViewMatrix *matrix, struct DotrViewVector *v);
/* Element view for 0x2061f0: a 48-byte stride whose first 16 bytes are the point. */
struct DotrViewVectorTriple {
    struct DotrViewVector unknown_00;
    struct DotrViewVector unknown_10[2];
};
/* Mean of (origin - point) over count points, normalized into out; 0 when count is 0. */
int Ex3d_GetViewVector(struct DotrViewVector *origin, int count, struct DotrViewVector *points, struct DotrViewVector *out);
int Ex3d_GetViewVectorForNode(struct DotrViewVector *origin, int count, struct DotrViewVectorTriple *points, struct DotrViewVector *out);
float RhGeom_GetVectorScalor(struct DotrViewVector *vector);
float RhGeom_GetPointsDistance(struct DotrViewVector *a, struct DotrViewVector *b);
void RhGeom_OuterProduct(struct DotrViewVector *out, struct DotrViewVector *a, struct DotrViewVector *b);
float RhGeom_InnerProduct(struct DotrViewVector *a, struct DotrViewVector *b);
int RhView_Term(void);
int RhView_GetCurView(struct DotrViewBlock *out);
int RhView_SetScreen(float a, float b, float c, float d, float e);
int RhView_SetAspect(float a, float b);
int RhView_SetClipZ(float a, float b);
int RhView_GetScreenCenter(float *a, float *b);
int RhView_PushStack(void);
int RhView_PopStack(void);
int RhView_PopFakeMirror(struct DotrViewBlock *out);
void RhView_GetWorldScreenMatrix(struct DotrViewMatrix *out);
void RhView_GetWorldViewMatrix(struct DotrViewMatrix *out);
void RhView_GetViewScreenMatrix(struct DotrViewMatrix *out);
int RhView_GetScreenDist(float *a);
int RhView_GetClipZ(float *a, float *b);
int RhView_SetFogParameter(float a, float b, float c, float d, struct DotrViewQuad *q);
int RhView_GetFogParameter(float *a, float *b, float *c, float *d, struct DotrViewQuad *q);

/* Frame at the start of an entry's 0xc0 block: a matrix loaded by the VU0 routine, then words from +0x40. */
struct DotrViewFrame {
    struct DotrViewMatrix unknown_00;
    float unknown_40[20];
};
int RhView_Reset(struct DotrViewFrame *frame);

/* Builds (x, y, SzView_GetScreenDistance(0), 1.0) and transforms it in place by the inverse
 * (RhGeom_InvertMatrix) of the world-view matrix at the selected entry's +0x40. Helper
 * argument meanings are assumed. SzView_GetScreenDistance is declared in block_table.h, which
 * cannot be included here (it declares RhGeom_NormalizeVector with another vector type). */
extern float SzView_GetScreenDistance(int index);
extern void RhGeom_InvertMatrix(struct DotrViewMatrix *out, struct DotrViewMatrix *m);
extern void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v);
/* Cos/sin-like helpers at 0x20b480/0x20b550, declared as in small_misc.h (which cannot be
 * included here); phase argument type assumed int. */
extern float RhMath_CosI(int phase);
extern float RhMath_SinI(int phase);
/* Rodrigues rotation: writes 16 floats to m from the normalized axis copy and the phase.
 * small_misc.h views the axis as float *; both are the same 16-byte vector. */
void RhGeom_GetRotVectorMatrix(float *m, struct DotrViewVector *axis, int phase);
void Ex3d_GetPointOnScreen(struct DotrViewVector *out, float x, float y);
/* Same address as the small_misc.h RhGeom_GetScaleRotXYZTransMatrix view (different TU); here
 * outMatrix/scale/rot/pos are plain arrays. */
void RhGeom_GetScaleRotXYZTransMatrix(float *outMatrix, float *scale, int *rot, float *pos);

/* Matrix multiply helper RhGeom_MulMatrix (0x20bc00, VU0 assembly); typed with DotrViewMatrix here
 * because small_misc.h declares it with DotrMiscMatrix and cannot be included. */
extern void RhGeom_MulMatrix(struct DotrViewMatrix *m, struct DotrViewMatrix *other);
/* Projection-style matrix builder: a is the scale of the temporary, b..e the x/y scales and
 * offsets, f..i depth terms. */
void RhGeom_GetScreenMatrix(struct DotrViewMatrix *m, float a, float b, float c, float d, float e, float f, float g, float h, float i);

/* Helpers of RhView_Set (two are VU0 assembly): arguments assumed from the call setup. The
 * copy helper is declared with void pointers as the draw_state.h entry does, because
 * memory_ops.h types 0x20b380 differently and cannot be included. */
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
int lRhView_ConstructMatrix(void);
extern int RhGeom_ResetMatrixStack(void);
extern void RhGeom_SetMatrix(struct DotrViewMatrix *m);
extern void RhGeom_GetMatrix(struct DotrViewBlock *block);
extern struct DotrViewMatrix InvertView;
/* Installs block into the selected entry, refreshes derived data and always returns 1. */
int RhView_Set(struct DotrViewBlock *block);
/* Variant of 0x20e260 that also copies the entry's +0x40 matrix back into block (four quadwords)
 * and does not call 0x20bba0/0x20bad0; block must therefore hold at least nine words. */
int RhView_Modify(struct DotrViewBlock *block);

/* Draw-chain helpers at 0x208e60..0x20a340 are defined or declared in draw_state.h, which
 * view_state.h does not include; declared here with opaque tag/packet types. The argument
 * types of these callees are assumed from the call setup. */
struct DotrDrawTag;
struct DotrDrawPacket;
extern struct DotrDrawTag *RhVu1Mac_SetViewScreenMatrix(struct DotrDrawTag *tag, void *source);
extern struct DotrDrawTag *RhVu1Mac_SetScreenWorldMatrix(struct DotrDrawTag *tag, void *source);
extern struct DotrDrawTag *RhVu1Mac_SetClipParameter(struct DotrDrawTag *tag, int mode, float x, float y, float z);
extern struct DotrDrawTag *RhVu1Mac_SetFogParameterEx(struct DotrDrawTag *tag, float a, float b, float c, float d);
extern struct DotrDrawPacket *RhBase_SetFogColor(struct DotrDrawPacket *packet, unsigned char *bytes);
/* Chains the entry's matrices, depth terms and bounds into the draw tag stream. */
struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag);
/* Transforms in - (0, 0, dz) by the selected entry's +0x80 matrix and returns 0x1a23b0 of z / w. */
int RhView_GetSampleZ(struct DotrViewVector *in, float dz);

/* Library-block helper at 0x1a23b0; takes a depth ratio, return type assumed int. */
extern int func_001a23b0(float ratio);
/* Transforms out (w set to 1.0) by the 0x20eba0 and 0x20ec00 matrices with z lowered by dz;
 * returns 0xfffff6 when the result w <= 0, 0 when z <= 0, else func_001a23b0(z / w). */
int Ex3d_GetSampleZ(struct DotrViewVector *out, float dz);
/* Same two-matrix transform as 0x205d10 with z lowered by dz; returns the transformed w. */
float Ex3d_GetSampleW(struct DotrViewVector *out, float dz);

/* Constant 16-byte vectors at 0x293d60/0x293d70; the edge vectors start as copies so only
 * their w component comes from here. Contents are unknown. */
extern struct DotrViewVector data_00293d60;
extern struct DotrViewVector data_00293d70;
/* Averages the cross products of consecutive edges of the n-point loop pts (n >= 3) into out
 * and normalizes it with 0x20d070; returns 1, or 0 with out zeroed when n < 3. */
int Ex3d_GetSurfaceNormal(int n, struct DotrViewVector *pts, struct DotrViewVector *out);

/* Plane through points a, b, c (x/y/z read at offsets 0..8): out[0..2] hold the cross-product
 * normal, out[3] the plane term, out[4] the normal length via 0x1a6c60. */
void Ex3d_GetSurfaceC(float *a, float *b, float *c, float *out);

/* Pointer arguments are passed to these as 32-bit ints on the target. On the host the casts
 * go through this pointer-sized integer and are then truncated to int, so host tests compare
 * the truncated values. */
#ifdef __MWERKS__
typedef int DotrViewWord;
#else
#include <stdint.h>
typedef intptr_t DotrViewWord;
#endif
/* Declared as in small_state.h, which view_state.h cannot include. */
extern void RhGeom_PushMatrix(int unknown);
extern void RhGeom_PopMatrix(void);
extern int Ex3d_Clipping(int a, int *b, int c, int d, int e, int f, int g);
extern int gEx3d_aPlane[];
/* Transforms count 48-byte entries (vector at the start, w set to 1.0) by m (or the 0x20eba0
 * matrix when null), passes the batch to 0x206d30 when c >= 0, and optionally stores the mean z
 * (sum / result) in *average. result is uninitialised when c < 0. */
int Ex3d_NodeL2V(struct DotrViewMatrix *m, int count, struct DotrViewVector *v, int a, int b, int c, float *average);

/* Library-block routine; used as a square root by the vector routines. */
extern float func_001a6c60(float value);
/* Same object as DotrMiscRect in small_misc.h, with the two float pairs
 * viewed as the vectors Ex3d_GetPointOnScreen fills. */
struct DotrViewRect {
    unsigned char unknown_00[0x10];
    struct DotrViewVector unknown_10;
    struct DotrViewVector unknown_20;
    unsigned char unknown_30[0x68];
    short unknown_98;
    short unknown_9a;
};
extern void func_001cdeb0_view(struct DotrViewRect *p);
/* Defined in the side_table group. */
int lSzDuel_SetCardCtl2(struct DotrViewRect *p, int a, int b, int c, int d, short e, short f);

/* 0x20ddb0. Clears the selected-entry word 0x32a240 and the control word at
 * 0x32a250 + 0x44, asks 0x209d60 for two ints and fills entry 0: +0x160 =
 * 400, +0x164 / +0x168 = the two ints as floats, +0x150 / +0x154 = half of
 * each (read back), +0x158 = 1, +0x15c = 0.95, +0x16c = 1, +0x170 = 10000,
 * +0x174 = that value - 1000 (the subtraction is done at run time), +0x178 =
 * +0x170, +0x17c = 0, +0x180 = 1000, the four quad bytes = 255; then
 * 0x20e1a0 on entry 0's frame. Answers 1.
 * RhBase_GetScreenSize (0x209d60, draw_state) is declared as there. */
extern int RhBase_GetScreenSize(int *a, int *b);
int RhView_Init(void);

/* 0x207230: 48-byte vertices. The eight bytes at +0x10 are written as one
 * 64-bit unit with 16/16/24/8-bit members (halfword stores, a masked
 * doubleword store and a byte store); the bytes from +0x18 are not touched.
 * The 0x1a23b0 result goes into the 24-bit member without sign extension.
 * scratch_quads.h declares this function for its caller with an int first
 * argument and its own vertex view. */
struct DotrViewPacked {
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long f : 8;
};
struct DotrViewVertex {
    struct DotrViewVector position;
    struct DotrViewPacked screen;
    unsigned char unknown_18[24];
};
void Ex3d_NodeV2S(struct DotrViewMatrix *matrix, int count, struct DotrViewVertex *vertex);

/* 0x206530 Ex3d_DivideN: the 0x28 bytes it reads of a vertex. Member names are this project's;
 * the 14-bit coordinate pair sharing storage with the two floats is what the code shows (the
 * upper two bits of each halfword are kept). Assumed to be the same vertex as DotrViewVertex. */
struct DotrViewDivideUv {
    unsigned short u : 14;
    unsigned short v : 14;
};
struct DotrViewDivideVertex {
    float pos[4];
    int unknown_10[2];
    unsigned char rgba[4];
    float q;
    union {
        float st[2];
        struct DotrViewDivideUv uv;
    } t;
};
/* Writes p0 + t * (p1 - p0) to out: position always; colour bytes (through func_001a23b0) and q
 * for flags & 1; the float pair for flags & 2; the 14-bit pair for flags & 0x10. */
void Ex3d_DivideN(struct DotrViewDivideVertex *p0, struct DotrViewDivideVertex *p1, int flags,
                  struct DotrViewDivideVertex *out, float t);

/* 0x20d5e0: builds a camera matrix from a position and a direction. Rows 0..2 are the negated
 * normalized (dir.z, 0, -dir.x), the negated normalized (-dir.y*dir.x, dir.x^2+dir.z^2,
 * -dir.y*dir.z) and the normalized direction; row 3 is the position with 1.0. The result is
 * multiplied by RhGeom_GetRotXYZMatrix of angles (0, 0, roll) and inverted in place.
 * The work vectors at 0x3269f0/0x326a00/0x326a10 are vecX/vecY/vecZ (static locals in the
 * Japanese build); dir is read with a 128-bit load, so it is 16-byte aligned. Parameter
 * types follow the caller's declaration in view_rebuild.h (assumed). */
extern struct DotrViewVector data_003269f0;
extern struct DotrViewVector data_00326a00;
extern struct DotrViewVector data_00326a10;
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
void RhGeom_CreateCameraMatrix2(struct DotrViewMatrix *out, float *pos, float *dir, int roll);

/* 0x20ede0. Sets centre->w to 1, transforms the centre with RhGeom_MulMatrixVector (null
 * matrix = current) into the static vector at 0x32a230 and tests the sphere against the
 * selected entry's clip distances (+0x16c, +0x170) and eight side planes built from +0x158
 * to +0x168 and the squared normals at +0x188/+0x18c/+0x190. Returns 4, or flags | 2, as soon
 * as the sphere is outside a plane, else flags | 1; bits 4 and 2 mark crossed planes.
 * Meanings of the bits are assumed from the callers (or-ed into a visibility word). */
extern struct DotrViewVector data_0032a230;
int RhView_CheckViewVolume(struct DotrViewVector *centre, float radius);

/* Float view of the 0x48 bytes at 0x32a250 (gRhView_FakeMirror) as 0x20e520 writes them.
 * Member meanings are assumed from the arithmetic. */
struct DotrViewMirror {
    struct DotrViewVector pos;    /* +0x00: copy of the first argument */
    struct DotrViewVector normal; /* +0x10: second argument, normalized */
    struct DotrViewVector dir;    /* +0x20: view direction of entry 0, mirrored */
    struct DotrViewVector eye;    /* +0x30: direction from pos to the view position, mirrored */
    float dist;                   /* +0x40: distance from pos to the view position */
    int active;                   /* +0x44: same word as DotrViewControl.unknown_44 */
};
/* 0x1a6c18 and 0x1a22c0 (library block): assumed atan2f(y, x) and float to int. */
extern float func_001a6c18(float a, float b);
extern int func_001a22c0(float value);
/* 0x20e520. Returns 0 when a mirror is already active. Else copies entry 0's view block to
 * out, replaces its position (+0x40) and direction (+0x50) with their mirror images about the
 * plane (pos, normal), stores a roll angle word at +0x80, pushes the view stack when it has
 * room (result unused), marks the mirror active and returns 1. */
int RhView_PushFakeMirror(struct DotrViewVector *pos, struct DotrViewVector *normal, struct DotrViewBlock *out);

#endif
