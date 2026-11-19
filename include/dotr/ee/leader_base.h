#ifndef DOTR_EE_LEADER_BASE_H
#define DOTR_EE_LEADER_BASE_H

/* SzDuel_DrawLeaderBase_Pipe at 0x1dc790: rotates four corner vectors by the leader model's
 * rotation and builds its quad in the scratchpad at 0x70002000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The unit SzDuel_GetUnit returns, as far as this function reads it. */
struct DotrLeaderBaseUnit {
    int unknown_00;
    int unknown_04; /* non-zero: nothing is drawn */
    unsigned char unknown_08[0x38];
    unsigned char unknown_40; /* passed to SzDuel_GetMapInfo with the next byte */
    unsigned char unknown_41;
    unsigned char unknown_42[4];
    unsigned char unknown_46; /* passed to SzModel_GetRot */
    unsigned char unknown_47[6];
    unsigned char unknown_4d; /* brightness, scaled by level / 128 */
    unsigned char unknown_4e[9];
    char unknown_57_0 : 7;
    char unknown_57_7 : 1;
    char unknown_58_0 : 1;
    char unknown_58_1 : 1;
};

/* The map cell SzDuel_GetMapInfo returns: three colour bytes and the level byte. */
struct DotrLeaderBaseCell {
    unsigned char unknown_00[0x10];
    unsigned char unknown_10;
    unsigned char unknown_11;
    unsigned char unknown_12;
    unsigned char unknown_13[7];
    unsigned char unknown_1a;
};

/* One 48-byte scratchpad vertex as this function fills it (scratch_quads.h has the same
 * record without the two floats at +0x20). */
struct DotrLeaderBaseVertex {
    float position[3];
    unsigned char unknown_0c[12];
    unsigned char colour[4];
    float unknown_1c;
    float uv[2];
    unsigned char unknown_28[8];
};

/* 16-byte header at the start of the 48-byte packet local. */
struct DotrLeaderBasePacket {
    short unknown_00;
    short unknown_02;
    int unknown_04;
    long unknown_08;
};

/* gSzDuel_Base is read as 10-byte records, one per side; only the first byte is used. */
struct DotrLeaderBaseSide {
    unsigned char texture;
    unsigned char unknown_01[9];
};

/* The storage at 0x317490 as this function reads it: texture pointers at +1696 (indexed by
 * the side record's byte; extent not known) and the byte at +0xd34 that scales the base. */
struct DotrLeaderBaseControl {
    unsigned char unknown_000[1696];
    int *texture[2];
#ifdef __MWERKS__
    unsigned char unknown_6a8[0xd34 - 0x6a8];
#endif
    unsigned char unknown_d34;
};

/* Four corner vectors (x, y, z, w) copied whole from 0x291590 before x and z are set. */
struct DotrLeaderBaseCorners {
    float v[4][4];
};

extern struct DotrLeaderBaseSide gSzDuel_Base[];
extern struct DotrLeaderBaseControl data_00317490;
extern struct DotrLeaderBaseCorners data_00291590;

#ifdef __MWERKS__
#define DOTR_LEADER_BASE_VERTICES ((struct DotrLeaderBaseVertex *)0x70002000)
#else
extern struct DotrLeaderBaseVertex dotr_leader_base_vertices_70002000[5];
#define DOTR_LEADER_BASE_VERTICES (dotr_leader_base_vertices_70002000)
#endif

/* Callees: SzDuel_GetUnit (state_records), SzDuel_GetMapInfo (state_grid), SzModel_GetRot (object_table),
 * RhGeom_GetRotXYZMatrix (small_misc), RhGeom_PushMatrix / RhGeom_PopMatrix (vu0), ExAssignAlpha
 * (draw_state), 0x2070f0 / Ex3d_NodeV2S (view_state), RhGeom_MulMatrixVectorArray (vu0), ExAssignTexture
 * (ex_texture) and ExSortNode_Pipe (draw_state). Pointer parameters are narrowed to this group's views. */
extern struct DotrLeaderBaseUnit *SzDuel_GetUnit(int side, int pos);
extern struct DotrLeaderBaseCell *SzDuel_GetMapInfo(int x, int y);
extern int SzModel_GetRot(int modelId, int *rotationArray);
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
extern void RhGeom_PushMatrix(float *matrix);
extern void RhGeom_PopMatrix(void);
extern void RhGeom_MulMatrixVectorArray(float *out, float *m, float *in, int count);
extern int ExAssignAlpha(struct DotrLeaderBasePacket *packet, long value);
extern int ExAssignTexture(struct DotrLeaderBasePacket *packet, int *texture, int a);
extern int Ex3d_NodeL2V(void *m, int count, struct DotrLeaderBaseVertex *v, int a, int b, int c, float *average);
extern void Ex3d_NodeV2S(int a, int count, struct DotrLeaderBaseVertex *v);
extern int ExSortNode_Pipe(int a, struct DotrLeaderBasePacket *packet, int count, struct DotrLeaderBaseVertex *v,
                           int e, int f);

/* `cursor` is the draw cursor passed on and returned; `centre` three floats; assumed from the uses. */
extern int SzDuel_DrawLeaderBase_Pipe(int cursor, int side, float *centre, int alpha, int outline);

#endif
