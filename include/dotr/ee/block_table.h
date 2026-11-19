#ifndef DOTR_EE_BLOCK_TABLE_H
#define DOTR_EE_BLOCK_TABLE_H

/* The 2,240-byte block addressed by the single pointer at 0x31beb0, the eight
 * 48-byte slots at 0x31bec0 and the two 12-byte entries at 0x31c050.
 * Structure layouts and prototypes are inferred, not the original declarations;
 * only some field meanings are known (camera point, screen distance, model-view records, fog
 * colours, lighting flags, view mode).
 * pool_table.h declares the pointer word as a plain word pointer under
 * another symbol. Evidence: docs/modules/camera.md. */
#ifdef __MWERKS__
typedef __int128 DotrBlockWord128;
#else
__extension__ typedef __int128 DotrBlockWord128;
#endif

struct DotrBlockQuad {
    unsigned char unknown_bytes[4];
};

struct DotrBlockPair {
    float x, y;
};

struct DotrBlockVector4 {
    float x, y, z;
    float unknown_w;
};

struct DotrBlockTriple {
    struct DotrBlockVector4 unknown_00[3];
};

struct DotrBlockElement {
    struct DotrBlockTriple unknown_00[3];
};

/* Forty-eight bytes copied with three 128-bit loads and stores. */
union DotrBlockMatrix48 {
    DotrBlockWord128 unknown_quads[3];
    struct DotrBlockVector4 unknown_vectors[3];
};

struct DotrBlock {
    unsigned char unknown_000[0x30];
    unsigned char cameraPoint[0x10];
    int unknown_040;
    float screenDistance;
    unsigned char unknown_048[0x18];
    struct DotrBlockElement modelViewData[10];
    union DotrBlockMatrix48 unknown_600[10];
    struct DotrBlockPair unknown_7e0[10];
    struct DotrBlockPair unknown_830[10];
    struct DotrBlockQuad fogColour[10];
    unsigned short unknown_8a8;
    unsigned short lightingFlags;
    unsigned short unknown_8ac;
    unsigned short unknown_8ae;
    int viewMode;
    int unknown_8b4;
    unsigned char unknown_8b8[8];
};

typedef char DotrBlockMustBe8c0[(sizeof(struct DotrBlock) == 0x8c0) ? 1 : -1];

struct DotrBlockBit {
    signed char unknown_bit_0 : 1;
    signed char unknown_bits_1_7 : 7;
};

struct DotrBlockSlot {
    struct DotrBlockBit unknown_00;
    struct DotrBlockBit unknown_01;
    signed char unknown_02;
    signed char unknown_03;
    unsigned char unknown_04[44];
};

struct DotrBlockEntryBits {
    unsigned char unknown_bits_0_5 : 6;
    signed char unknown_bit_6 : 1;
    signed char unknown_bit_7 : 1;
};

struct DotrBlockEntry {
    struct DotrBlockQuad unknown_00;
    unsigned char unknown_04;
    unsigned char unknown_05;
    unsigned char unknown_06;
    struct DotrBlockEntryBits unknown_07;
    struct DotrBlockBit unknown_08;
    unsigned char unknown_09[3];
};

typedef char DotrBlockSlotMustBe30[(sizeof(struct DotrBlockSlot) == 0x30) ? 1 : -1];
typedef char DotrBlockEntryMustBe0c[(sizeof(struct DotrBlockEntry) == 0x0c) ? 1 : -1];

extern struct DotrBlock *gSzView_hView_blocks[];
extern struct DotrBlockSlot gSzView_CameraReq[];
extern struct DotrBlockEntry gSzFade_fl[];

int SzView_SetFogColor(int index, int position, struct DotrBlockQuad *in);
int SzView_SetFogParamZ(int index, int position, struct DotrBlockPair *in);
int SzView_SetFogParamY(int index, int position, struct DotrBlockPair *in);
int SzView_SetStraightLightColor(int index, int position, int part, struct DotrBlockVector4 *in);
int SzView_GetStraightLightVector(int index, int position, int part, struct DotrBlockVector4 *out);

/* Stores *in (when given) as vector 1 of part `part` of element `position`,
 * then builds a local 64-byte matrix from that vector with 0x20c630 and hands
 * 0x20cdd0 vector 2 of the part, the matrix and the object at 0x293040.
 * Assumed: the local is one 64-byte structure (a bare float array orders the
 * 0x20c630 arguments differently); the callee prototypes are those declared
 * in small_misc.h and view_state.h; 0x293040 is only seen by address and is
 * declared as four floats. */
struct DotrBlockMatrix64 {
    float unknown_00[16];
};
struct DotrViewVector;
struct DotrViewMatrix;
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
extern void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v);
extern float gSzView_vecUnit[4];
int SzView_SetStraightLightDirection(int index, int position, int part, struct DotrBlockVector4 *in);
int SzView_InqStraightLight(int index, int position);
int SzView_InqPointLight(int index, int position);
int SzView_SetPointLight(int index, int position, union DotrBlockMatrix48 *pointLightParameters);
float SzView_GetScreenDistance(int index);
int SzView_SetScreenDistance(int index, float value);
void *SzView_GetCP(int index);
/* Same guards; returns the address of the block's +0x10 member (type unknown). */
void *SzView_GetRel(int index);
int SzView_SetRoll(int index, int rollAmount);
struct DotrBlockSlot *SzView_InqRequest(int a, int b);
int SzView_PauseRequest(int a, int b, int flag);
int SzView_FreeRequest(int a, int b);
struct DotrBlock *SzView_GetLook(int index);
int SzView_IsFogZ(int index, int position);
int SzView_IsFogY(int index, int position);
int SzView_GetMode(int index);
int SzView_SetMode(int index, int value);
void SzView_IV2V(int *in, float *out);
void SzFade_Init(void);
int SzFade_Entry(int index, int a, int b, int c, int d, struct DotrBlockQuad *in);
int SzFade_SetKillFlag(int index);
int SzFade_SetPauseFlag(int index, int flag);
int SzFade_GetRestCount(int index);
int SzFade_IsDoing(int flag);

/* 0x1f90e0 views a block as three vectors at +0x10,
 * +0x20 and +0x30; the table at 0x31beb0 is typed as pointers to it. */
struct DotrBlockDelta {
    unsigned char unknown_00[0x10];
    struct DotrBlockVector4 relativeDirection;
    struct DotrBlockVector4 sourcePoint;
    struct DotrBlockVector4 cameraPoint;
};
extern struct DotrBlockDelta *gSzView_hView_delta[];
extern void RhGeom_NormalizeVector(struct DotrBlockVector4 *v);
int lSzView_CalcRel(int index);
/* 0x1fb6a0: assumes the view object's first three floats are distance, yaw and pitch, and a
 * word at +0x8b0 is a mode (0 and 2 limit distance and pitch). Parameter order is the one its
 * caller SzGame_ComInit (0x1a8bb0) passes. */
float RhMath_Sin(float rad);
float RhMath_Cos(float rad);
int SzView_RollCamera(int index, int nested, int dir, float yaw, float pitch, float dist);

/* Assumed view of a slot-set object (size at least 464 bytes): 22 16-byte entries, 32 ids at
 * +0x160, a 5-bit mode at +0x1a0, two halfword counters and two vectors at +0x1b0 / +0x1c0. The
 * real object layout is unknown; bits of the entry byte at +12 above the low two and of the
 * byte at +0x1a0 above the low five are only read-modify-written. */
typedef struct DotrSlotVec {
    float x, y, z, w;
} DotrSlotVec;
typedef struct DotrSlotEntry {
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned char pad6[2];
    int d;
    unsigned char kind : 2;
    unsigned char pad_c[3];
} DotrSlotEntry;
typedef struct DotrSlotSet {
    DotrSlotEntry entries[22];
    unsigned short ids[32];
    unsigned char mode : 5;
    unsigned char low_1a1 : 4; /* 0x275d00 clears the low four bits of the byte at +0x1a1 */
    unsigned short h418;
    unsigned short h420;
    unsigned char pad_a2[10];
    DotrSlotVec v0;
    DotrSlotVec v1;
} DotrSlotSet;
/* Resets the set to empty entries and ids and copies the vectors of block 0 (+0x30) and the
 * block itself into it. */
void TaTuto_InitTutoData(DotrSlotSet *set);
/* Defined in small_misc and state_records; declared here for TaTuto_InitTutorial. */
extern void TaTuto_Init032(void);
extern void TaTuto_Init036(void);
extern int SzDuel_CalcSummonPower(int side, int operation, int amount);
/* kind is masked to a byte after the reset call. */
void TaTuto_InitTutorial(unsigned char kind, DotrSlotSet *set);
/* 0x275d00: TaTuto_IsTuto and SzDuel_GetPlayer as declared in state_queries.h and
 * state_records.h; SzDuel_GetUnitSummonOrder as in state_grid.h. */
extern int TaTuto_IsTuto(int side);
extern int SzDuel_GetPlayer(int side);
extern int SzDuel_GetUnitSummonOrder(int side, int pos);
int TaLogic_InitLogicData(int side, DotrSlotSet *set);

/* SzView_Set_Pipe: views the start of a block as two vectors at +0x10/+0x20, a
 * word at +0x40 and five floats from +0x44, and fills a 144-byte local that
 * is passed to RhView_Set (vectors at +0x40/+0x50, word at +0x80). The
 * callee declarations repeat view_state.h; Ex3d_InitClipPlane (ex3d_calc) is declared
 * as taking and returning nothing. */
struct DotrViewBlock;
struct DotrViewQuad;
struct DotrDrawTag;
struct DotrBlockViewArgs {
    unsigned char unknown_00[0x40];
    struct DotrBlockVector4 unknown_40;
    struct DotrBlockVector4 unknown_50;
    unsigned char unknown_60[0x20];
    int unknown_80;
    unsigned char unknown_84[0xc];
};
struct DotrBlockHead {
    unsigned char unknown_00[0x10];
    struct DotrBlockVector4 relativeDirection;
    struct DotrBlockVector4 sourcePoint;
    unsigned char cameraPoint[0x10];
    int unknown_40;
    float screenDistance;
    float xAspectRatio;
    float yAspectRatio;
    float minZClip;
    float maxZClip;
};
extern int RhView_SetScreen(float a, float b, float c, float d, float e);
extern int RhView_SetAspect(float a, float b);
extern int RhView_SetClipZ(float a, float b);
extern void Ex3d_InitClipPlane(void);
extern int RhView_SetFogParameter(float a, float b, float c, float d, struct DotrViewQuad *q);
extern int RhView_Set(struct DotrViewBlock *block);
extern struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag);

struct DotrDrawTag *SzView_Set_Pipe(struct DotrDrawTag *tag, int index);

/* 0x1f8f80 stores four floats at the start of the 0x1f90e0 view from the vectors at +0x20 and
 * +0x30; the fourth is copied from an unwritten stack word. The three float helpers are named by
 * address only (assumed: square root, two-argument angle, two-argument remainder). */
extern float func_001a6c60(float value);
extern float func_001a6c18(float a, float b);
extern float func_001a6c30(float a, float b);
int lSzView_CalcLook(int index);

/* 0x1f9770 copies a whole 144-byte element with nine 128-bit loads and stores; its third
 * argument is also compared with 0 and 1 as plain values (clear the element's bit / set it
 * without copying). */
union DotrBlockElement144 {
    DotrBlockWord128 unknown_quads[9];
    struct DotrBlockElement unknown_element;
};
struct DotrBlockElements {
    unsigned char unknown_000[0x60];
    union DotrBlockElement144 modelViewData[10];
};
int SzView_SetStraightLight(int index, int k, union DotrBlockElement144 *in);

/* 0x1f9d80: copies a vector (the 16 bytes at 0x293030 when the pointer is null) into
 * the slot-0 block's +0x30 vector, then runs the inlined bodies of 0x1f90e0 and
 * 0x1f8f80. Returns 0 for an invalid index or empty slot. The default's type is
 * assumed from the four float copies. */
extern struct DotrBlockVector4 gSzView_pntCPDef;
int SzView_SetCP(int index, struct DotrBlockVector4 *in);
/* 0x1fad60: assumed to take the block index and a direction vector; the first float of the
 * block is the distance. */
int SzView_SetRelSP(int index, struct DotrBlockVector4 *in);
/* 0x1f9b20 and 0x1f9fe0: the same routine for the vector at +0x20 (default: the 16
 * bytes at 0x293020), and for both vectors at once. 0x1f9ad0 returns the address
 * of the +0x20 vector. Argument types are assumed from the copies. */
extern struct DotrBlockVector4 gSzView_pntSPDef;
void *SzView_GetSP(int index);
int SzView_SetSP(int index, struct DotrBlockVector4 *in);
int SzView_SetCamera(int index, struct DotrBlockVector4 *sourcePoint, struct DotrBlockVector4 *cameraPoint);
/* 0x1fb360 and 0x1fb500: store *in as the block's first 16 bytes (the polar
 * vector 0x1f9fe0 writes), wrap its y with 0x1a6c30, turn the object at 0x293040
 * into the +0x10 vector through a local matrix (built only when 0x1fafb0 returns
 * non-zero), then set the +0x20 vector to +0x30 plus +0x10 scaled by polar x
 * (0x1fb360) or the +0x30 vector to +0x20 minus it (0x1fb500), and redo the
 * 0x1f90e0 delta. Assumed: 0x1fafb0 returns int and fills four ints (view of
 * the address small_misc.h declares as void(int, int)). */
extern int func_001fafb0_angles(int index, int *out);
int SzView_SetLookSP(int index, struct DotrBlockVector4 *in);
int SzView_SetLookCP(int index, struct DotrBlockVector4 *in);

/* 0x1f8d30 SzView_SetLight_Pipe (name from the Japanese build's symbol table): submits the
 * straight lights and the point light of slot k, or of slot 9 when the slot's bit is clear in
 * +0x8a8 / +0x8aa (or set in +0x8ae). Assumed: 0x208f00 takes (tag, 48-byte light, direction,
 * slot) and returns the tag; a first call with all-zero arguments and slot -1 has its result
 * ignored. 0x209110 is declared as in draw_state.h. */
extern struct DotrDrawTag *RhVu1Mac_SetStraightLightToVu1_Ex(struct DotrDrawTag *tag, struct DotrBlockTriple *light,
                                                             struct DotrBlockVector4 *dir, int slot);
extern struct DotrDrawTag *RhVu1Mac_SetPointLightToVu1(struct DotrDrawTag *tag, float *a, float *b, float x, float y,
                                                       float z);
struct DotrDrawTag *SzView_SetLight_Pipe(struct DotrDrawTag *tag, int index, int k);
/* 0x1fc210: moves the view's +0x30 point to ratio * model centre + (1 - ratio) * base (base 0 = the
 * current +0x30 point), w 1.0, then recomputes the direction (0x1f90e0 inlined) and the polar triple
 * (0x1f8f80 inlined). mode 1 stops there; mode 2 also calls SzView_RollCamera(index, 0, 1, ...) with
 * the old minus the new polar yaw, pitch and distance; any other mode changes nothing. Returns 0
 * for a bad index or an empty slot, else 1. Parameter meanings are inferred; SzModel_GetCenter
 * (declared in object_table.h with its own vector type) is declared here with this group's. */
extern int SzModel_GetCenter(int modelId, struct DotrBlockVector4 *xyzCoords);
int SzView_SetViewCamera(int index, int model, struct DotrBlockVector4 *base, int mode, float ratio);

/* 0x1fb940: assumes a camera motion block tagged 'CMOT' whose word at +0x0c is a frame count and
 * whose four {offset, count} pairs at +0x10 select eight-byte key records from +0x20 (eye, look-at,
 * roll, field of view), and view-object fields +0x40 (roll angle), +0x44 (screen distance) and
 * +0x50 (its lower limit). Member and parameter names are assumed. The key-track and model
 * callees are defined in the key_tracks and object_table groups and declared here with
 * incomplete types. */
struct DotrCameraMotion {
    int magic;
    int unknown_04;
    int unknown_08;
    unsigned int frames;
    struct { unsigned short offset; unsigned short count; } track[4];
    struct { int w[2]; } data[1];
};
struct DotrBlockCamera {
    struct DotrBlockDelta delta;
    int roll;
    float screen;
    int unknown_48;
    int unknown_4c;
    float screen_min;
};
struct DotrKeyOut;
struct DotrKeyTriple;
struct DotrKeySingle;
struct DotrObjectVector4;
void lSzView_CalcInterpCameraMotion(struct DotrKeyOut *outputPosition, struct DotrKeyTriple *keys, int keyframeCount,
                                    float currentTime);
float lSzView_CalcInterpCameraMotion1(struct DotrKeySingle *keys, int n, float t);
extern int func_001a22c0(float value);
int SzModel_GetRot(int modelId, struct DotrObjectVector4 *rotationArray);
int SzModel_GetPos(int modelId, struct DotrObjectVector4 *xyzCoords);
extern void RhGeom_PushMatrix(float *matrix);
extern void RhGeom_PopMatrix(void);
int SzView_SetInterpCameraMotion(int index, struct DotrCameraMotion *motion, int frames, int frame, int model, int mirror);

/* SzView_SetFog_Pipe (0x1f8b50). Fog pairs at +0x7e0 / +0x830 and colour quads at +0x880 of the
 * view block, enable masks at +0x8ac / +0x8ae (same offsets as DotrBlock above). The two callees
 * are called through views with void-pointer parameters; the host test forwards them to the stubs
 * of RhBase_SetFogColor and RhVu1Mac_SetFogParameterEx. */
struct DotrDrawPacket;
struct DotrBlockFogColor {
    unsigned char c[4];
};
struct DotrBlockFogView {
    unsigned char pad0[0x7E0];
    float posA[10][2];                  /* 0x7E0 */
    float posB[10][2];                  /* 0x830 */
    struct DotrBlockFogColor color[10]; /* 0x880 */
    unsigned char pad8A8[4];
    unsigned short maskA;               /* 0x8AC */
    unsigned short maskB;               /* 0x8AE */
};
extern int func_001a23b0(float value);
extern struct DotrDrawPacket *RhBase_SetFogColor(struct DotrDrawPacket *packet, unsigned char *bytes);
extern struct DotrDrawTag *RhVu1Mac_SetFogParameterEx(struct DotrDrawTag *tag, float a, float b, float c, float d);
extern void *func_0020a340_fog(void *arg0, void *arg1);                                   /* RhBase_SetFogColor */
extern void *func_002093e0_fog(void *arg0, float fparg0, float fparg1, float fparg2, float fparg3); /* RhVu1Mac_SetFogParameterEx */
void *SzView_SetFog_Pipe(void *arg0, int arg1, int arg2, float arg3);

#endif
