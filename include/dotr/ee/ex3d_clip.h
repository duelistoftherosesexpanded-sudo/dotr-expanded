#ifndef DOTR_EE_EX3D_CLIP_H
#define DOTR_EE_EX3D_CLIP_H

/* Ex3d polygon routines that take a vertex array and a plane or packet list (Ex3d_Clipping at
 * 0x206d30 and the neighbouring Ex3d_RotTransPers functions).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/math-prims.md. */

#ifdef __MWERKS__
typedef __int128 DotrEx3dWord128;
#else
__extension__ typedef __int128 DotrEx3dWord128;
#endif

/* The eight bytes at +0x18 of a vertex, copied as two floats by Ex3d_Clipping (view_state.h reads the
 * same bytes as four colour bytes and a float for 0x206530). */
struct DotrEx3dPair {
    float unknown_0;
    float unknown_4;
};

/* A 48-byte vertex as 0x206d30 uses it: copied whole with three 128-bit loads and stores, position read as
 * floats. view_state.h's DotrViewDivideVertex describes the same bytes for the callee. */
union DotrEx3dVertex {
    DotrEx3dWord128 quads[3];
    struct {
        float pos[4];
        int unknown_10[2];
        struct DotrEx3dPair pair;
        float st[2];
    } v;
};

/* A 20-byte plane record: distance = (x*px + y*py + z*pz + d) / e. */
struct DotrEx3dPlane {
    float x, y, z, d, e;
};

/* 0x206530, declared in view_state.h with its own vertex view; assumed signature as called here. */
extern void Ex3d_DivideN(union DotrEx3dVertex *p0, union DotrEx3dVertex *p1, int flags, union DotrEx3dVertex *out,
                         float t);

/* 0x206d30 Ex3d_Clipping (name from the Japanese build's symbol table). The fifth argument is not read.
 * view_state.h and small_state.h keep an all-int prototype for their callers. */
int Ex3d_Clipping(int plane_count, struct DotrEx3dPlane *planes, int count, union DotrEx3dVertex *vertices, int unused,
                  union DotrEx3dVertex *work, int flags);

/* 0x205440 Ex3d_RotTransPers (name from the Japanese build's symbol table). The vector is copied float by
 * float; the output is one 64-bit packet word with 16-bit x and y, a 24-bit z and eight bits that are kept.
 * Callee signatures are assumed from this call site (view_state.h and block_table.h declare the same
 * functions with their own pointer types; small_misc.h keeps another prototype of 0x205440). */
struct DotrEx3dVector {
    float x, y, z, w;
};
union DotrEx3dPacket {
    struct {
        unsigned long x : 16;
        unsigned long y : 16;
        unsigned long z : 24;
        unsigned long unknown_56 : 8;
    } xyz;
    float unknown_floats[2];
};
extern void RhView_GetWorldScreenMatrix(float *out);
extern void RhGeom_PushMatrix(float *matrix);
extern void RhGeom_MulMatrixVector(struct DotrEx3dVector *out, float *matrix, struct DotrEx3dVector *in);
extern void RhGeom_PopMatrix(void);
extern int func_001a23b0(float value);
int Ex3d_RotTransPers(struct DotrEx3dVector *in, float *matrix, union DotrEx3dPacket *packet, float *w);

/* 0x205630 Ex3d_RotTransPersAuto (name from the Japanese build's symbol table):
 * the array form of Ex3d_RotTransPers. Views assumed from the accesses: the input vectors'
 * fourth word is stored as the integer 0x3f800000, the packet has a 16-bit count, a 64-bit word
 * of 4-bit types and then one 64-bit word per entry, whose x and y are stored as halfwords and
 * whose 24-bit z is inserted above them. Only entries of type 4 are transformed. */
struct DotrEx3dAutoIn {
    float x, y, z;
    int w;
};
struct DotrEx3dAutoXyz {
    unsigned short x, y;
    int unknown_4;
};
struct DotrEx3dAutoBits {
    unsigned long xy : 32;
    unsigned long z : 24;
};
struct DotrEx3dAutoPacket {
    unsigned short count;
    unsigned long types;
    struct DotrEx3dAutoXyz xyz[1];
};
extern void RhGeom_MulMatrixVectorArray(float *out, float *matrix, struct DotrEx3dAutoIn *in, int count);
int Ex3d_RotTransPersAuto(struct DotrEx3dAutoIn *in, float *work, float *matrix, struct DotrEx3dAutoPacket *packet,
                          float *avg);

#endif
