/* lSzBattle_DrawFakeShadow (0x1e6f90, 2,056 bytes), draws a battle model's shadow projected onto the floor
 * plane along the straight light and returns the next packet address. Yu-Gi-Oh! The Duelists of the
 * Roses, NTSC-U SLUS_205.15. Evidence: docs/modules/engine.md.
 *
 * ORIGINAL-FILE UNIT: the 17 functions that precede lSzBattle_DrawFakeShadow in the original source file
 * (lSzDuel_OpenHandCursor_Pipe 0x1e4570 ... lSzBattle_DrawFakeMirror 0x1e6650) are compiled
 * first, in this one translation unit, as compiler context (docs/DECOMPILATION.md,
 * "Original-file units"). lSzBattle_DrawFakeShadow compiles to the original only after all of
 * them, in this order; tools/match.py places them at their own addresses and compares
 * lSzBattle_DrawFakeShadow. Each of them also has its own source elsewhere in the tree. The host
 * build sees only lSzBattle_DrawFakeShadow (the block is `#ifdef __MWERKS__`).
 */
#include "dotr/ee/fake_shadow.h"

#ifdef __MWERKS__
/* ---- Compiler context: the original file's functions before lSzBattle_DrawFakeShadow. ---- */

typedef struct {
    u16 count;
    u16 flags;
    u32 padding;
    u64 registers;
} Tag_001E4570;

typedef struct {
    u8 texture;
    u8 padding;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
} Texture_001E4570;

typedef struct {
    u64 u : 14;
    u64 pad0 : 2;
    u64 v : 14;
    u64 pad1 : 34;
} UV_001E4570;

typedef struct {
    u64 x : 16;
    u64 y : 16;
    u64 z : 24;
    u64 fog : 8;
} Vertex_001E4570;

typedef struct {
    u64 uv;
    Vertex_001E4570 xyz;
} Corner_001E4570;

typedef struct {
    u64 tag[2];
    u8 r, g, b, a;
    f32 q;
    Corner_001E4570 corners[4];
    u64 extra[4];
} Packet_001E4570;

typedef struct {
    f32 v[8][4];
} Quad_001E4570;

typedef struct {
    s32 v[4];
} Rotation_001E4570;

typedef struct {
    f32 m[4][4];
} Matrix_001E4570;

typedef struct {
    u8 pad000[0x6A0];
    void *textures[16];
    u8 pad6E0[0x578];
    s32 depth;
} State_001E4570;

static inline s32 abs_001E4570(s32 value) {
    return value >= 0 ? value : -value;
}

void lSzDuel_OpenHandCursor_Pipe(s32 packet, s32 angle, s32 x, s32 y) {
    extern Quad_001E4570 data_00291600;
    extern Rotation_001E4570 data_00291680;
    extern u8 data_00290814[];
    extern u8 data_0029081e[];
    extern void RhGeom_MulMatrixVectorArray(void *, s32, void *, s32);
    State_001E4570 *state;
    Texture_001E4570 *first;
    Texture_001E4570 *second;
    s32 originX;
    s32 originY;
    s32 halfWidth;
    s32 halfHeight;
    s32 width;
    s32 height;
    s32 du;
    s32 dv;
    s32 turn;
    Quad_001E4570 corners;
    Rotation_001E4570 rotation;
    Matrix_001E4570 matrix;
    Packet_001E4570 sprite __attribute__((aligned(8)));

    state = (State_001E4570 *)data_00317490.b;
    first = (Texture_001E4570 *)data_00290814;
    second = (Texture_001E4570 *)data_0029081e;
    halfWidth = abs_001E4570(first->right - first->left) >> 1;
    halfHeight = abs_001E4570(first->bottom - first->top) >> 1;
    width = abs_001E4570(second->right - second->left);
    height = abs_001E4570(second->bottom - second->top);
    corners = data_00291600;
    corners.v[0][0] = -(f32)halfWidth;
    corners.v[0][1] = -(f32)halfHeight;
    corners.v[1][0] = (f32)halfWidth;
    corners.v[1][1] = -(f32)halfHeight;
    corners.v[2][0] = -(f32)halfWidth;
    corners.v[2][1] = (f32)halfHeight;
    corners.v[3][0] = (f32)halfWidth;
    corners.v[3][1] = (f32)halfHeight;
    corners.v[4][0] = (f32)-halfWidth;
    corners.v[4][1] = (f32)-halfHeight;
    corners.v[5][0] = (f32)(width - halfWidth);
    corners.v[5][1] = (f32)-halfHeight;
    corners.v[6][0] = (f32)-halfWidth;
    corners.v[6][1] = (f32)(height - halfHeight);
    corners.v[7][0] = (f32)(width - halfWidth);
    corners.v[7][1] = (f32)(height - halfHeight);
    turn = (angle + 0x10000) % 0x10000;
    originY = y + 0x800;
    rotation = data_00291680;
    originX = x + 0x800;
    rotation.v[2] = (turn << 16) >> 8;
    dv = du = 8;
    switch (turn >> 14) {
    case 1:
        dv = -8;
        break;
    case 2:
        dv = du = -8;
        break;
    case 3:
        du = -8;
        break;
    }
    RhGeom_GetRotXYZMatrix(&matrix, &rotation);
    RhGeom_PushMatrix((s32)&matrix);
    RhGeom_MulMatrixVectorArray((void *)0x70000000, 0, &corners, 8);
    RhGeom_PopMatrix();
    ((Tag_001E4570 *)sprite.tag)->flags = 0x24;
    ((Tag_001E4570 *)sprite.tag)->count = 9;
    ((Tag_001E4570 *)sprite.tag)->registers = 0x424242421ULL;
    ((Tag_001E4570 *)sprite.tag)->padding = 0;
    ((Tag_001E4570 *)sprite.tag)->flags |= 0x800;
    ExAssignAlpha(&sprite, 0x8000000044ULL);
    sprite.r = 0x80;
    sprite.g = 0x80;
    sprite.b = 0x80;
    sprite.a = 0x80;
    sprite.q = 1.0f;
    sprite.corners[0].xyz.z = state->depth;
    sprite.corners[0].xyz.fog = 0;
    sprite.corners[1].xyz.z = state->depth;
    sprite.corners[1].xyz.fog = 0;
    sprite.corners[2].xyz.z = state->depth;
    sprite.corners[2].xyz.fog = 0;
    sprite.corners[3].xyz.z = state->depth;
    sprite.corners[3].xyz.fog = 0;
    ExAssignTexture(&sprite, state->textures[first->texture], 0);
    ((UV_001E4570 *)&sprite.corners[0].uv)->u = (first->left << 4) + du;
    ((UV_001E4570 *)&sprite.corners[0].uv)->v = (first->top << 4) + dv;
    ((UV_001E4570 *)&sprite.corners[1].uv)->u = (first->right << 4) - du;
    ((UV_001E4570 *)&sprite.corners[1].uv)->v = (first->top << 4) + dv;
    ((UV_001E4570 *)&sprite.corners[2].uv)->u = (first->left << 4) + du;
    ((UV_001E4570 *)&sprite.corners[2].uv)->v = (first->bottom << 4) - dv;
    ((UV_001E4570 *)&sprite.corners[3].uv)->u = (first->right << 4) - du;
    ((UV_001E4570 *)&sprite.corners[3].uv)->v = (first->bottom << 4) - dv;
    sprite.corners[0].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[0]);
    sprite.corners[0].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[1]);
    sprite.corners[1].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[4]);
    sprite.corners[1].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[5]);
    sprite.corners[2].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[8]);
    sprite.corners[2].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[9]);
    sprite.corners[3].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[12]);
    sprite.corners[3].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[13]);
    packet = ExSortPrim_Pipe(packet, (s32)&sprite, 0, 0);
    ExAssignTexture(&sprite, state->textures[second->texture], 0);
    ((UV_001E4570 *)&sprite.corners[0].uv)->u = (second->left << 4) + du;
    ((UV_001E4570 *)&sprite.corners[0].uv)->v = (second->top << 4) + dv;
    ((UV_001E4570 *)&sprite.corners[1].uv)->u = (second->right << 4) - du;
    ((UV_001E4570 *)&sprite.corners[1].uv)->v = (second->top << 4) + dv;
    ((UV_001E4570 *)&sprite.corners[2].uv)->u = (second->left << 4) + du;
    ((UV_001E4570 *)&sprite.corners[2].uv)->v = (second->bottom << 4) - dv;
    ((UV_001E4570 *)&sprite.corners[3].uv)->u = (second->right << 4) - du;
    ((UV_001E4570 *)&sprite.corners[3].uv)->v = (second->bottom << 4) - dv;
    sprite.corners[0].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[16]);
    sprite.corners[0].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[17]);
    sprite.corners[1].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[20]);
    sprite.corners[1].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[21]);
    sprite.corners[2].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[24]);
    sprite.corners[2].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[25]);
    sprite.corners[3].xyz.x = (originX << 4) + (s32)(16.0f * ((f32 *)0x70000000)[28]);
    sprite.corners[3].xyz.y = (originY << 4) + (s32)(16.0f * ((f32 *)0x70000000)[29]);
    ExSortPrim_Pipe(packet, (s32)&sprite, 0, 0);
}

void SzDuel_CloseHandCursor(void) {
    HandCursorState = 0x18;
}

typedef struct {
    s8 pad0 : 5;
    s8 flag : 1;
    s8 pad1 : 2;
} Flags;

extern Flags DisplayUIBitfield;

void SzDuel_SetIgnoreJudge(s64 arg0) {
    DisplayUIBitfield.flag = arg0;
}

typedef struct {
    s32 entries[2];
    s32 count;
} State_001E4E90;

void SzBattle_Initialize(void) {
    extern State_001E4E90 gSzBattle_bl_raw;
    State_001E4E90 *state = &gSzBattle_bl_raw;
    s32 i;

    RhMem_FillMemory((s8 *)&gSzBattle_bl_raw, 0, 0xC);
    i = 0;
    do {
        state->entries[i] = -1;
        i++;
    } while (i < 2);
    state->count = 0;
}

typedef struct Vec3_001E4F00 {
    f32 x, y, z;
} Vec3_001E4F00;

void SzBattle_Init(s32 arg0) {
    extern u8 gSzBattle_aStageParam_bytes[];
    extern s32 gSzBattle_nSave_word;
    extern s32 anOfs_STAGECAM_words[];
    extern s32 data_00292174[];
    extern Vec3_001E4F00 data_002921a0;
    extern Vec3_001E4F00 data_002921b0;
    extern char data_002cf730[];
    extern char data_002cf746[];
    extern s32 gSzBattle_bl_raw[];
    extern s32 SzFile_ReadRequestParam(s32, s32, char *, s32, s32, s32, s32, void *, s64);
    s32 * state;
    s32 begin;
    f32 * vec20;
    s32 j;
    f32 * range;
    f32 * vecC;
    s32 size;
    f32 * vecB8;
    char filename[128];
    s32 i;
    s64 enabled;
    u8 * base;

    state = gSzBattle_bl_raw;
    base = gSzBattle_aStageParam_bytes + arg0 * 0xC0;
    vecC = (f32 *)(base + 0xC);
    vec20 = (f32 *)(base + 0x20);
    range = (f32 *)(base + 0xB0);
    vecB8 = (f32 *)(base + 0xB8);
    if (arg0 == 4) {
        enabled = 0;
    } else {
        enabled = 1;
    }
    SzView_SetStraightLight(0, 9, 1);
    i = 0;
    do {
        SzModel_SetDrawFlag(i, 0);
        i += 1;
    } while (i < 0x54);
    i = 0;
    do {
        SzModel_SetDrawFlag(i + 8, 1);
        i += 1;
    } while (i < 0xC);
    i = 0;
    do {
        SzModel_SetDrawFlag(i + 7, 1);
        i += 1;
    } while (i <= 0);
    SzEffect_SetFogColorForMonster((s32) vecB8);
    SzEffect_SetFogColorForBattle((s32) vecB8);
    SzEffect_SetFogParamZForMonster(0);
    SzEffect_SetFogParamZForBattle(0);
    if (range[0] == range[1]) {
        SzEffect_SetFogParamYForMonster(0);
        SzEffect_SetFogParamYForBattle(0);
    } else {
        SzEffect_SetFogParamYForMonster((s32) range);
        SzEffect_SetFogParamYForBattle((s32) range);
    }
    SzEffect_SetStraightLightForMonster((s32) vec20);
    SzEffect_SetStraightLightForBattle((s32) vec20);
    SzEffect_SetPointLightForMonster(0);
    SzEffect_SetPointLightForBattle(0);
    SzEffect_SetIntensityForMonster((s32) vecC);
    SzEffect_SetIntensityForBattle((s32) base);
    SzEffect_SetBlackOutForMonster(0x80);
    SzEffect_SetBlackOutForBattle(0x80);
    j = 0;
    do {
        SzModel_SetFogFlag(j + 8, enabled);
        j += 1;
    } while (j < 0xC);
    if (arg0 == 4) {
        Vec3_001E4F00 position = data_002921a0;
        Vec3_001E4F00 other;
        position.x = ((f32 *)base)[0];
        position.y = ((f32 *)base)[1];
        other = data_002921b0;
        SzModel_SetIntensity(8, &position.x);
        SzModel_SetFogFlag(9, 1);
        SzModel_SetIntensity(0xC, &other.x);
    }
    if (arg0 == 2) {
        SzModel_SetFogFlag(9, 0);
    }
    if (arg0 != gSzBattle_nSave_word) {
        gSzBattle_nSave_word = arg0;
    }
    if (state[2] != 0) {
        SzMem_Free(state[2]);
        state[2] = 0;
    }
    begin = anOfs_STAGECAM_words[arg0];
    size = data_00292174[arg0] - begin;
    func_0019b648(filename, data_002cf730, (s32) SzFile_GetEnv((s32) data_002cf746));
    SzFile_ReadRequestParam(0x400, 1, filename, 0, size, begin, size, lSzBattle_CameraCB, 0);
}

typedef struct {
    u8 padding[8];
    s32 field8;
} State_001E5210;

extern State_001E5210 gSzBattle_bl_raw;

void lSzBattle_CameraCB(void *arg0, s32 arg1) {
    State_001E5210 *state = &gSzBattle_bl_raw;
    s32 var_v1;

    if (arg1 != 0) {
        var_v1 = *(s32 *)((u8 *)arg0 + 0x14);
    } else {
        var_v1 = 0;
    }
    state->field8 = var_v1;
}

s32 SzBattle_InqCamera(void) {
    return data_0031ab90;
}

s32 SzBattle_InqEntry(s32 index) {
    return *(s32 *)&gSzBattle_bl_raw.padding[index * 4];
}

#ifndef GAME_H
/* The game translation unit already declares this global with its local type. */
typedef struct {
    unsigned char padding[8];
    int field8;
} State_001E5210;

extern State_001E5210 gSzBattle_bl_raw;
#endif



typedef union {
    __int128 value;
    s32 axis[4];
} Rotation_001E5260;

extern Rotation_001E5260 data_002921c0;

void SzBattle_Entry(s32 arg0, s32 arg1) {
    extern s32 SzModel_GetModelNo();
    s32 *states = (s32 *)&gSzBattle_bl_raw;
    s32 x = arg1 >> 8;
    s32 y = arg1 & 0xFF;
    void *card;
    s32 model;
    Rotation_001E5260 rotation;
    f32 position[4];
    f32 offset;

    card = (void *)SzDuel_GetUnit(x, y);
    rotation = data_002921c0;
    rotation.axis[1] = (arg0 != 0) ? 0 : 0x8000;
    if (card != NULL && *(s32 *)card != 0) {
        SzDuel_SetBusyFlag(x, y, 1);
        model = *((u8 *)card + 0x46);
        SzSound_TransferSEGrp(arg0 + 2, SzModel_GetModelNo(model), 0);
        SzModel_SetRot(model, &rotation);
        SzModel_GetPos(model, position);
        position[0] = 0.0f;
        if (arg0 != 0) {
            offset = -80.0f;
        } else {
            offset = 80.0f;
        }
        position[2] = offset;
        SzModel_SetPos(model, position);
        SzModel_SetDrawFlag(model, 1);
        SzModel_SetDeform(model, 0);
        SzModel_SetDeformRatio(model, 0);
        SzModel_SetAnimDirect(model, 0);
        SzModel_SetAnimDir(model, 1);
        states[arg0] = arg1;
    }
}

typedef struct {
    s32 entries[2];
    s32 count;
} Slots_001E53C0;

typedef struct {
    s32 v[4];
} Rot_001E53C0 __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x40];
    u8 f40;
    u8 f41;
    u8 pad42[4];
    u8 model;
} Card_001E53C0;

extern Rot_001E53C0 data_002921d0;

void SzBattle_Return(s32 arg0, s32 arg1) {
    extern s32 SzEffect_ClearRefID2();
    Slots_001E53C0 *state = (Slots_001E53C0 *)&gSzBattle_bl_raw;
    s32 pos;
    s32 hi;
    s32 lo;
    Card_001E53C0 *card;
    s32 model;
    Rot_001E53C0 rot;
    f32 vecA[4];
    f32 vecB[4];

    pos = ((Slots_001E53C0 *)&gSzBattle_bl_raw)->entries[arg0];
    hi = pos >> 8;
    lo = pos & 0xFF;
    card = (Card_001E53C0 *)SzDuel_GetUnit(hi, lo);
    rot = data_002921d0;
    rot.v[1] = (hi != 0) ? 0 : 0x8000;
    if (card != NULL) {
        model = card->model;
        SzEffect_ClearRefID2(pos, 1, &rot);
        SzDuel_SetBusyFlag(hi, lo, 0);
        SzModel_SetScale(model, NULL);
        SzModel_SetRot(model, &rot);
        SzModel_GetPos(model, vecA);
        SzDuel_CalcUnitPos(card->f40, card->f41, 0, vecB);
        vecA[0] = vecB[0];
        vecA[2] = vecB[2];
        SzModel_SetPos(model, vecA);
        SzModel_SetDeform(model, 1);
        SzModel_SetDeformRatio(model, 0x20);
        if (arg1 != 0) {
            SzDuel_SetUnitCtlRateM(hi, lo, 0, 0);
            SzDuel_SetUnitCtlGoalM(hi, lo, 0);
        } else {
            SzModel_SetAnimDirect(model, 0);
            SzModel_SetAnimDir(model, 1);
        }
        SzModel_SetRgba(model, 0, NULL);
        SzModel_SetFilter(model, 1, NULL);
        SzModel_CancelMotionBlur(model);
        SzSound_StopSE(arg0 + 2, -1);
        state->entries[arg0] = -1;
    }
}

s32 SzBattle_DrawStage_Pipe(s32 arg0) {
    if (SzModel_GetDrawFlag(8) != 0) {
        return lSzBattle_DrawStage_Pipe(arg0);
    }
    return arg0;
}

typedef struct Vec3_001E55E0 {
    f32 x;
    f32 y;
    f32 z;
} Vec3_001E55E0;

typedef struct Camera_001E55E0 {
    f32 position[4];
    s32 rotation[4];
    f32 scale[4];
    u8 pad30[0x60];
} Camera_001E55E0;

typedef struct Stage_001E55E0 {
    Vec3_001E55E0 light;
    Vec3_001E55E0 ambient;
    u8 pad18[8];
    Camera_001E55E0 camera;
    u8 padB0[0x10];
} Stage_001E55E0;

s32 lSzBattle_DrawStage_Pipe(s32 arg0) {
    extern Stage_001E55E0 gSzBattle_aStageParam_bytes[];
    extern Camera_001E55E0 data_002921f0;
    extern Vec3_001E55E0 data_00292280;
    extern Vec3_001E55E0 data_00292290;
    extern s32 gSzBattle_nSave_word;
    extern s8 chRate;
    extern s8 chFlip;
    extern u8 data_002921e0;
    extern s32 lSzBattle_DrawGrass_Pipe(s32, s32, s32);
    extern s32 SzEffect_GetBlackOutForBattle(void);
    extern s32 SzModel_GetModelNo(s32);
    Camera_001E55E0 camera;
    Vec3_001E55E0 ambient;
    Vec3_001E55E0 light;
    Camera_001E55E0 *stage;
    Camera_001E55E0 *toCamera;
    Camera_001E55E0 *fromCamera;
    Vec3_001E55E0 *toAmbient;
    Vec3_001E55E0 *fromAmbient;
    Vec3_001E55E0 *toLight;
    Vec3_001E55E0 *fromLight;
    f32 scale;
    f32 u;
    f32 t;
    s32 i;
    s32 id;
    s32 value;
    s32 id2;
    s32 j;

    scale = (u8)SzEffect_GetBlackOutForBattle() / 128.0f;
    if (gSzBattle_nSave_word == 7) {
        if (chRate < 0x3C) {
            t = 0.0f;
        } else {
            t = (chRate - 0x3C) / 4.0f;
        }
        u = 1.0f - t;
        toCamera = &gSzBattle_aStageParam_bytes[10].camera;
        fromCamera = &gSzBattle_aStageParam_bytes[7].camera;
        toAmbient = &gSzBattle_aStageParam_bytes[10].ambient;
        fromAmbient = &gSzBattle_aStageParam_bytes[7].ambient;
        toLight = &gSzBattle_aStageParam_bytes[10].light;
        fromLight = &gSzBattle_aStageParam_bytes[7].light;
        camera = data_002921f0;
        camera.position[0] = toCamera->position[0] * t + fromCamera->position[0] * u;
        camera.position[1] = toCamera->position[1] * t + fromCamera->position[1] * u;
        camera.position[2] = toCamera->position[2] * t + fromCamera->position[2] * u;
        camera.position[3] = toCamera->position[3] * t + fromCamera->position[3] * u;
        camera.rotation[0] = (s32)((f32)toCamera->rotation[0] * t + (f32)fromCamera->rotation[0] * u);
        camera.rotation[1] = (s32)((f32)toCamera->rotation[1] * t + (f32)fromCamera->rotation[1] * u);
        camera.rotation[2] = (s32)((f32)toCamera->rotation[2] * t + (f32)fromCamera->rotation[2] * u);
        camera.rotation[3] = (s32)((f32)toCamera->rotation[3] * t + (f32)fromCamera->rotation[3] * u);
        camera.scale[0] = toCamera->scale[0] * t + fromCamera->scale[0] * u;
        camera.scale[1] = toCamera->scale[1] * t + fromCamera->scale[1] * u;
        camera.scale[2] = toCamera->scale[2] * t + fromCamera->scale[2] * u;
        camera.scale[3] = toCamera->scale[3] * t + fromCamera->scale[3] * u;
        ambient = data_00292280;
        ambient.x = toAmbient->x * t + fromAmbient->x * u;
        ambient.y = toAmbient->y * t + fromAmbient->y * u;
        ambient.z = toAmbient->z * t + fromAmbient->z * u;
        light = data_00292290;
        light.x = toLight->x * t + fromLight->x * u;
        light.y = toLight->y * t + fromLight->y * u;
        light.z = toLight->z * t + fromLight->z * u;
        SzModel_SetDrawFlag(0x10, t > 0.0f ? 1 : 0);
        SzEffect_SetStraightLightForMonster((s32)&camera);
        SzEffect_SetStraightLightForBattle((s32)&camera);
        SzEffect_SetIntensityForMonster((s32)&ambient);
        SzEffect_SetIntensityForBattle((s32)&light);
        if (SzModel_GetPauseFlag(8) == 0) {
            if ((s32)(60.0f * (func_0019a5c8() / 2147483648.0f)) == 0) {
                chFlip = 1;
            }
            chRate += chFlip;
            if (chRate < 0) {
                chRate = 0;
                chFlip = 0;
            }
            if (chRate < 0x20) {
                data_002921e0 = 1;
            }
            if (chRate > 0x40) {
                chRate = 0x40;
                chFlip = -1;
                if (data_002921e0 != 0) {
                    if ((s32)(2.0f * (func_0019a5c8() / 2147483648.0f)) != 0) {
                        SzSound_StereoPlaySE(0, 0x22, 0x23);
                    } else {
                        SzSound_StereoPlaySE(0, 0x24, 0x25);
                    }
                    data_002921e0 = 0;
                }
            }
        }
    }
    if (gSzBattle_nSave_word == 5 || gSzBattle_nSave_word == 6) {
        stage = &gSzBattle_aStageParam_bytes[gSzBattle_nSave_word].camera;
        if (SzModel_GetPauseFlag(8) == 0) {
            if (gSzBattle_nSave_word == 6) {
                stage->rotation[1] = (stage->rotation[1] + 0x200) % 0x10000;
            } else {
                stage->rotation[2] = (stage->rotation[2] + 0x200) % 0x10000;
            }
        }
        SzEffect_SetStraightLightForMonster((s32)stage);
        SzEffect_SetStraightLightForBattle((s32)stage);
    }
    if (gSzBattle_nSave_word == 8) {
        value = func_001a23b0(SzModel_GetAnimFrame(9));
        if (value == 0x14 || value == 0xDC) {
            SzSound_PlaySE(0, 0x28);
        }
    }
    SzEffect_SetIntensityOnly(2, scale);
    SzEffect_SetIntensityOnly(3, scale);
    for (i = 0; i < 12; i++) {
        id = i + 8;
        value = SzModel_GetModelNo(id) % 100;
        if (SzModel_IsModelDisp(id) != 0) {
            switch (gSzBattle_nSave_word) {
            case 1:
                arg0 = lSzBattle_DrawWasteland_Pipe(arg0, id, value);
                break;
            case 3:
                arg0 = lSzBattle_DrawGrass_Pipe(arg0, id, value);
                break;
            case 4:
                arg0 = lSzBattle_DrawSea_Pipe(arg0, id, value);
                break;
            case 7:
                arg0 = lSzBattle_DrawNormal_Pipe(arg0, id, value);
                break;
            default:
                arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, id, 0, 0, 0, 0, 0);
                break;
            }
        }
    }
    for (j = 0; j < 1; j++) {
        id2 = j + 7;
        if (SzModel_IsModelDisp(id2) != 0) {
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, id2, 0, 0, 0, 0, 0);
        }
    }
    return arg0;
}

s32 lSzBattle_DrawWasteland_Pipe(s32 arg0, s32 arg1, s32 arg2) {
    s32 parameters[4];

    arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, 0, 0, 0, 0);
    if (arg2 < 9) {
        SzModel_GetRot(arg1, parameters);
        parameters[1] = 0x8000;
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (s32) parameters, 0, 0, 0);
    }
    return arg0;
}

s32 lSzBattle_DrawGrass_Pipe(s32 arg0, s32 arg1, s32 arg2) {
    extern s32 lSzBattle_DrawFakeShadow(s32, s32, s32, f32, f32);
    s32 i;
    s32 *entry;
    s32 result;

    entry = (s32 *)&gSzBattle_bl_raw;
    result = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, 0, 0, 0, 0);
    if (arg2 == 0) {
        result = lSzBattle_DrawFakeShadow(result, 7, -1, -25.0f, 800.0f);
        i = 0;
        do {
            s32 value = *entry;
            if (value >= 0) {
                u8 *card = (u8 *)SzDuel_GetUnit(value >> 8, value & 0xFF);
                if (card != NULL) {
                    result = lSzBattle_DrawFakeShadow(result, card[0x46], -1, -25.0f, 800.0f);
                }
            }
            i++;
            entry++;
        } while (i < 2);
    }
    return result;
}

typedef struct Part_001E5EB0 {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    s32 phase;
    u8 pad1C[4];
} Part_001E5EB0;

typedef struct Rot_001E5EB0 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Rot_001E5EB0;

static inline f32 toAngle_001E5EB0(f32 degrees) {
    return 32768.0f * degrees / 180.0f;
}

s32 lSzBattle_DrawSea_Pipe(s32 arg0, s32 arg1, s32 arg2) {
    extern Part_001E5EB0 *apLocData_ptrs[];
    extern u8 data_00292160[];
    extern Rot_001E5EB0 data_002922a0;
    extern s32 lSzBattle_DrawFakeMirror(s32, s32);
    Rot_001E5EB0 rot;
    s32 i;
    s32 count;
    s32 base;
    Part_001E5EB0 *part;
    s32 idx;
    s32 phase;

    switch (arg2) {
    case 0:
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, 0, 0, 0, 0);
        arg0 = lSzBattle_DrawFakeMirror(arg0, 0xF);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        count = SzModel_GetAnimFrameMax(arg1, -1);
        base = (s32)SzModel_GetAnimFrame(arg1);
        idx = arg2 - 5;
        part = apLocData_ptrs[idx];
        for (i = 0; i < data_00292160[idx]; i++) {
            rot = data_002922a0;
            rot.x = (s32)toAngle_001E5EB0(part->x);
            rot.y = (s32)toAngle_001E5EB0(part->y);
            phase = (base + part->phase) % count;
            SzModel_SetAnimFrame(arg1, (f32)phase);
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)part, (s32)&rot, 0, 0, 0);
            part++;
        }
        SzModel_SetAnimFrame(arg1, (f32)base);
        break;
    default:
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, 0, 0, 0, 0);
        break;
    }
    return arg0;
}

typedef union {
    __int128 q;
    f32 f[4];
    s32 i[4];
} Quad_001E60E0;

extern Quad_001E60E0 data_002922b0;
extern Quad_001E60E0 data_002922c0;
extern Quad_001E60E0 data_002922d0;
extern Quad_001E60E0 data_002922e0;
extern Quad_001E60E0 data_002922f0;
extern Quad_001E60E0 data_00292300;

s32 lSzBattle_DrawNormal_Pipe(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 j;
    s32 *entry = (s32 *)&gSzBattle_bl_raw;
    Quad_001E60E0 scale;

    scale = data_002922b0;
    switch (arg2) {
    case 1: {
        Quad_001E60E0 rotation;
        rotation = data_002922c0;
        i = 0;
        do {
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (s32)&rotation, (s32)&scale, 0, 0);
            rotation.i[1] += 0x4000;
            i++;
        } while (i < 4);
        break;
    }
    case 3: {
        Quad_001E60E0 rotation;
        rotation = data_002922d0;
        i = 0;
        do {
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (s32)&rotation, (s32)&scale, 0, 0);
            rotation.i[1] += 0x8000;
            i++;
        } while (i < 2);
        break;
    }
    case 5: {
        Quad_001E60E0 rotation;
        Quad_001E60E0 position;
        f32 signZ;
        f32 baseX;
        f32 baseZ;
        f32 signX;

        rotation = data_002922e0;
        SzModel_GetPos(arg1, &position);
        i = 0;
        do {
            if (i % 2 == 0) {
                signX = -1.0f;
            } else {
                signX = 1.0f;
            }
            if (i >> 1 == 0) {
                signZ = -1.0f;
            } else {
                signZ = 1.0f;
            }
            baseX = 275.0f * signX;
            position.f[0] = baseX * scale.f[0];
            baseZ = 275.0f * signZ;
            position.f[2] = baseZ * scale.f[2];
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)&position, (s32)&rotation, (s32)&scale, 0, 0);
            position.f[0] = scale.f[0] * (baseX - 100.0f * signX);
            position.f[2] = baseZ * scale.f[2];
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)&position, (s32)&rotation, (s32)&scale, 0, 0);
            position.f[0] = baseX * scale.f[0];
            position.f[2] = scale.f[2] * (baseZ - 100.0f * signZ);
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)&position, (s32)&rotation, (s32)&scale, 0, 0);
            i++;
        } while (i < 4);
        break;
    }
    case 6: {
        Quad_001E60E0 rotation;
        Quad_001E60E0 position;
        f32 signX;
        f32 signZ;
        f32 baseX;
        f32 baseZ;
        f32 distance;

        rotation = data_002922f0;
        SzModel_GetPos(arg1, &position);
        i = 0;
        do {
            if (i % 2 == 0) {
                signX = -1.0f;
            } else {
                signX = 1.0f;
            }
            if (i >> 1 == 0) {
                signZ = -1.0f;
            } else {
                signZ = 1.0f;
            }
            baseX = 425.0f * signX;
            position.f[0] = baseX * scale.f[0];
            baseZ = 425.0f * signZ;
            position.f[2] = baseZ * scale.f[2];
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)&position, (s32)&rotation, (s32)&scale, 0, 0);
            j = 0;
            do {
                distance = 100.0f * (f32)(j + 1);
                position.f[0] = scale.f[0] * (baseX - distance * signX);
                position.f[2] = baseZ * scale.f[2];
                arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)&position, 0, (s32)&scale, 0, 0);
                position.f[0] = baseX * scale.f[0];
                position.f[2] = scale.f[2] * (baseZ - distance * signZ);
                arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (s32)&position, 0, (s32)&scale, 0, 0);
                j++;
            } while (j < 3);
            i++;
        } while (i < 4);
        break;
    }
    default: {
        Quad_001E60E0 rotation;
        rotation = data_00292300;
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, (s32)&rotation, (s32)&scale, 0, 0);
        break;
    }
    }
    if (arg2 == 0) {
        arg0 = lSzBattle_DrawFakeShadow(arg0, 7, -1, -13.5f, 400.0f);
        i = 0;
        do {
            s32 value = *entry;
            if (value >= 0) {
                u8 *card = (u8 *)SzDuel_GetUnit(value >> 8, value & 0xFF);
                if (card != NULL) {
                    arg0 = lSzBattle_DrawFakeShadow(arg0, card[0x46], -1, -13.5f, 400.0f);
                }
            }
            i++;
            entry++;
        } while (i < 2);
    }
    return arg0;
}

typedef struct Quad_001E6650 {
    f32 points[4][4];
} Quad_001E6650;

typedef struct Vec3_001E6650 {
    f32 x;
    f32 y;
    f32 z;
} Vec3_001E6650;

typedef struct Tag_001E6650 {
    u16 count;
    u16 flags;
    u32 padding;
    u64 registers;
} Tag_001E6650;

typedef struct UV_001E6650 {
    u64 u : 14;
    u64 pad0 : 2;
    u64 v : 14;
    u64 pad1 : 34;
} UV_001E6650;

typedef struct Vertex_001E6650 {
    u64 x : 16;
    u64 y : 16;
    u64 z : 24;
    u64 fog : 8;
} Vertex_001E6650;

typedef struct Tex0_001E6650 {
    u64 tbp : 14;
    u64 tbw : 6;
    u64 psm : 6;
    u64 tw : 4;
    u64 th : 4;
    u64 tcc : 1;
    u64 rest : 29;
} Tex0_001E6650;

typedef struct Packet_001E6650 {
    u64 tag[2];
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 q;
    u64 uv0;
    Vertex_001E6650 xyz0;
    u64 uv1;
    Vertex_001E6650 xyz1;
    u64 test;
    Tex0_001E6650 tex0;
    u64 tex1;
    u64 clamp;
} Packet_001E6650;

#define ABS_001E6650(x) ((x) >= 0 ? (x) : -(x))
#define RANDOM_001E6650(n) ((s32)((n) * (func_0019a5c8() / 2147483648.0f)))

s32 lSzBattle_DrawFakeMirror(s32 arg0, s32 arg1) {
    extern Quad_001E6650 data_00292310;
    extern Vec3_001E6650 data_00292350;
    extern s16 data_00292360[4];
    extern s16 data_00292368[4];
    extern s16 data_00292370;
    extern s16 data_00292378;
    extern s32 RhBase_AssignRenderFrame(s32, s32, s32);
    extern s32 ExSortPrim_Pipe(s32, void *, s32, s32);
    Quad_001E6650 quad;
    f32 center[4];
    f32 normal[4];
    f32 view[4];
    u8 screen[0x90];
    f32 previous[4];
    Vec3_001E6650 light __attribute__((aligned(16)));
    Packet_001E6650 packet;
    f32 saved;
    u8 color[4];
    u8 *card;
    s32 flag;
    s32 id;
    s32 j;
    s32 state;
    s32 *slot;
    s32 out;
    f32 scale;

    slot = (s32 *)&gSzBattle_bl_raw;
    out = arg0;
    quad = data_00292310;
    light = data_00292350;
    Ex3d_GetPointsAverage(4, &quad, center);
    Ex3d_GetSurfaceNormal(4, &quad, normal);
    Ex3d_GetViewVector((f32 *)SzView_GetSP(0), 4, quad.points[0], view);
    if (!(RhGeom_InnerProduct(view, normal) <= 0.0f) && RhView_PushFakeMirror(center, normal, screen) != 0) {
        if ((SzGame_GetFrameCount() & 1) == 0) {
            s32 range;
            s32 n;
            s32 dx;
            s32 dy;
            s16 *x;
            s16 *y;
            f32 distance;

            range = 6;
            distance = RhGeom_GetVectorScalor((f32 *)SzView_GetSP(0));
            if (distance <= 600.0f) {
                n = (s32)(600.0f / distance);
                range += (n < 6) ? n : 6;
            }
            if (SzModel_GetPauseFlag(8) == 0) {
                for (j = 2; j >= 0; j--) {
                    data_00292360[j + 1] = data_00292360[j];
                    data_00292368[j + 1] = data_00292368[j];
                }
                if (RANDOM_001E6650(3.0f) != 0) {
                    dx = data_00292370;
                } else {
                    dx = 0;
                }
                data_00292360[0] += dx;
                if (RANDOM_001E6650(3.0f) != 0) {
                    dy = data_00292378;
                } else {
                    dy = 0;
                }
                data_00292368[0] += dy;
                if (ABS_001E6650(data_00292360[0]) > range ||
                    (ABS_001E6650(data_00292360[0]) > range >> 1 && RANDOM_001E6650(3.0f) == 0)) {
                    data_00292370 = (data_00292360[0] > 0) ? -1 : 1;
                }
                if (data_00292368[0] > range * 2 || data_00292368[0] < 0 ||
                    (data_00292368[0] > range && RANDOM_001E6650(3.0f) == 0)) {
                    data_00292378 = (data_00292368[0] > range) ? -1 : 1;
                }
            }
        }
        arg0 = (s32)ExDitherMode_Pipe((void *)arg0, 0);
        RhBase_AssignRenderFrame(0, 10, 0x31);
        out = (s32)RhBase_EnableRenderFrame((void *)arg0);
        out = (s32)RhBase_ClearRenderFrame((u8 *)out, NULL);
        RhView_Modify(screen);
        out = RhView_SendViewParameters(out);
        out = (s32)RhBase_MaskFBuffer((void *)out, 0xFFFFFF);
        out = RhBase_SetZBufferMode(out, 0);
        out = SzModel_DrawWithOutCalc_Pipe(out, arg1, 0, 0, 0, 0, 0);
        out = (s32)RhBase_MaskFBuffer((void *)out, 0xFF000000);
        out = RhBase_SetZBufferMode(out, 1);
        {
        s32 i;
        for (i = 0; i < 2; i++, slot++) {
            if (*slot < 0) {
                continue;
            }
            card = (u8 *)SzDuel_GetUnit(*slot >> 8, *slot & 0xFF);
            flag = 0;
            saved = 1.0f;
            if (card == NULL) {
                continue;
            }
            id = card[0x46];
            if (SzModel_IsModelDisp(id) == 0) {
                continue;
            }
            state = SzModel_GetRgba(id, color);
            SzModel_SetRgba(id, 0, NULL);
            if (color[3] != 0x80) {
                flag = 1;
            }
            if (flag) {
                scale = color[3] / 128.0f;
                (void)scale; /* a use right after the assignment keeps scale at its own
                              * statement, as in the original */
                SzModel_GetIntensityOnly(id, &saved);
                SzModel_SetIntensityOnly(id, scale);
            }
            out = SzModel_DrawWithOutCalc_Pipe(out, id, 0, 0, 0, 0, 0);
            if (flag) {
                SzModel_SetIntensityOnly(id, saved);
            }
            SzModel_SetRgba(id, state, color);
        }
        }
        out = SzModel_DrawWithOutCalc_Pipe(out, 7, 0, 0, 0, 0, 0);
        SzModel_GetIntensity(0x10, previous);
        SzModel_SetIntensity(0x10, &light.x);
        out = SzModel_DrawWithOutCalc_Pipe(out, 0x10, 0, 0, 0, 0, 0);
        SzModel_SetIntensity(0x10, previous);
        ((Tag_001E6650 *)packet.tag)->flags = 0x26;
        ((Tag_001E6650 *)packet.tag)->count = 5;
        ((Tag_001E6650 *)packet.tag)->registers = 0x42421;
        ((Tag_001E6650 *)packet.tag)->padding = 0;
        ((Tag_001E6650 *)packet.tag)->flags |= 0x800;
        ExAssignAlpha(&packet, 0x8000000048LL);
        packet.r = 0x80;
        packet.g = 0x80;
        packet.b = 0x80;
        packet.a = 0x10;
        packet.q = 1.0f;
        packet.xyz0.z = 0xFFFFF6;
        packet.xyz0.fog = 0;
        packet.xyz1.z = 0xFFFFF6;
        packet.xyz1.fog = 0;
        ((UV_001E6650 *)&packet.uv0)->u = 0x2800;
        ((UV_001E6650 *)&packet.uv0)->v = 0;
        ((UV_001E6650 *)&packet.uv1)->u = 0;
        ((UV_001E6650 *)&packet.uv1)->v = 0x1C00;
        RhBase_GetTextureInfoForRender(&packet.tex0, &packet.tex1);
        packet.tex0.tcc = 0;
        out = (s32)RhBase_DisableRenderFrame((void *)out);
        out = RhText_SetTextureFlush(out);
        RhView_PopFakeMirror(screen);
        RhView_Modify(screen);
        out = RhView_SendViewParameters(out);
        out = RhBase_SetZBufferMode(out, 0);
        packet.xyz0.x = ((data_00292360[0] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[0] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[0] + 0x8E6) << 4) - 8;
        out = ExSortPrim_Pipe(out, &packet, 0, 0);
        packet.xyz0.x = ((data_00292360[1] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[1] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[1] + 0x8E6) << 4) - 8;
        out = ExSortPrim_Pipe(out, &packet, 0, 0);
        packet.xyz0.x = ((data_00292360[2] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[2] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[2] + 0x8E6) << 4) - 8;
        out = ExSortPrim_Pipe(out, &packet, 0, 0);
        packet.xyz0.x = ((data_00292360[3] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[3] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[3] + 0x8E6) << 4) - 8;
        out = ExSortPrim_Pipe(out, &packet, 0, 0);
        packet.a = 0x30;
        packet.xyz0.x = 0x6B98;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = 0x9458;
        packet.xyz1.y = 0x8E58;
        out = ExSortPrim_Pipe(out, &packet, 0, 0);
        out = RhBase_SetZBufferMode(out, 1);
        RhBase_FreeRenderFrame();
        out = (s32)ExDitherMode_Pipe((void *)out, 1);
    }
    return out;
}

#endif /* __MWERKS__: compiler context */

typedef struct {
    f32 f[5];
} Plane_001E6F90;

typedef struct {
    f32 matrix[4][4];
    f32 position[4];
    f32 direction[4][4];
} Camera_001E6F90;

typedef struct {
    f32 f[3][2][2][4];
} Points_001E6F90;

typedef union {
    u64 bits;
    struct {
        u64 low : 32;
        u64 flags : 24;
        u64 high : 8;
    } fields;
} Color_001E6F90;

typedef struct {
    f32 position[4];
    Color_001E6F90 color;
    u8 r, g, b, a;
    f32 q;
    f32 uv[2];
    u32 reserved[2];
} Vertex_001E6F90;

typedef struct {
    u16 first, type;
    u32 flags;
    u64 value;
    u8 tail[8];
} Header_001E6F90;

typedef union {
    s64 packed;
    f32 f[2];
} Range_001E6F90;

static inline void tag_001E6F90(Header_001E6F90 *p, s32 first, s32 type, u64 value) { p->type = type; p->first = first; p->value = value; p->flags = 0; }
#define MAX_001E6F90(a, b) ((a) > (b) ? (a) : (b))


#pragma opt_dead_assignments off

s32 lSzBattle_DrawFakeShadow(s32 arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1) {
    f32 delta;
    extern s32 Ex3d_NodeL2V(s32, s32, void *, s32, void *, s32, s32);
    extern void RhBase_AssignRenderFrame(s32, s32, s32);
    extern s32 RhBase_ClearRenderFrame(void *, s32);
    extern s32 ExSortNode_Pipe(s32, void *, s32, void *, s32, s32);
    extern void RhGeom_MulMatrixVectorArray(void *, s32, void *, s32);
    extern s32 RhView_SendViewParameters(void *);
    extern s32 SzModel_GetModelType();
    extern Plane_001E6F90 data_00292380;
    extern Points_001E6F90 data_002923a0;
    extern Range_001E6F90 data_00292460;
    Plane_001E6F90 plane;
    Camera_001E6F90 camera;
    Points_001E6F90 points;
    f32 position[4];
    f32 matrix[4][4];
    f32 midpoint[6];
    Header_001E6F90 header __attribute__((aligned(8)));
    s64 saved[3] __attribute__((aligned(8)));
    Range_001E6F90 range __attribute__((aligned(8)));
    s32 alpha;

    plane = data_00292380;
    plane.f[3] = -fparg0;
    alpha = SzModel_GetModelType(arg1, &plane);
    if (SzModel_IsModelDisp(arg1) == 0) {
        return (s32)arg0;
    }
    if (SzView_InqStraightLight(0, alpha) == 0) {
        alpha = 9;
    }
    RhView_GetCurView(&camera);
    SzView_GetStraightLightVector(0, alpha, 0, camera.direction[0]);
    {
        f32 absolute = camera.direction[0][1];
        absolute = absolute >= 0.0f ? absolute : -absolute;
        if (!(absolute < 1.0f)) {
            camera.direction[0][1] *= 0.999999f;
            camera.direction[0][2] = 0.000001f;
        }
    }
    if (!(RhGeom_InnerProduct(plane.f, camera.direction[0]) <= 0.0f) &&
        RhView_PushStack() != 0) {
        s32 count;
        s32 endpoint;
        s32 opacityScale;
        Vertex_001E6F90 *scratch;
        s32 i;
        Vertex_001E6F90 *vertex;
        Vertex_001E6F90 *current;
        Vertex_001E6F90 *opposite;
        s32 packet;
        s32 drawPacket;
        s32 band;
        s32 pair;
        f32 scale;
        f32 width;
        f32 height;
        u8 *scratchpad;

        opacityScale = (SzModel_GetAlpha(arg1) << 6) >> 7;
        scale = fparg1 / 800.0f;
        scratchpad = (u8 *)0x70000000;
        width = 320.0f * scale;
        height = 224.0f * scale;
        scratch = (Vertex_001E6F90 *)(0x2000 + scratchpad);
        points = data_002923a0;
        points.f[0][0][0][0] = -width;
        points.f[0][0][0][1] = -height;
        points.f[0][0][1][0] = -width;
        points.f[0][0][1][1] = height;
        points.f[0][1][0][0] = width;
        points.f[0][1][0][1] = -height;
        points.f[0][1][1][0] = width;
        points.f[0][1][1][1] = height;
        points.f[1][0][0][0] = -width;
        points.f[1][0][0][1] = -height;
        points.f[1][0][0][2] = fparg1;
        points.f[1][0][1][0] = -width;
        points.f[1][0][1][1] = height;
        points.f[1][0][1][2] = fparg1;
        points.f[1][1][0][0] = width;
        points.f[1][1][0][1] = -height;
        points.f[1][1][0][2] = fparg1;
        points.f[1][1][1][0] = width;
        points.f[1][1][1][1] = height;
        points.f[1][1][1][2] = fparg1;
        packet = (s32)ExDitherMode_Pipe((void *)arg0, 0);
        RhBase_AssignRenderFrame(0, 10, 0x31);
        packet = RhBase_ClearRenderFrame(RhBase_EnableRenderFrame((void *)packet), 0);
        SzModel_GetCenter(arg1, position);
        camera.position[0] = position[0] + camera.direction[0][0] * fparg1;
        camera.position[1] = position[1] + camera.direction[0][1] * fparg1;
        camera.position[2] = position[2] + camera.direction[0][2] * fparg1;
        camera.direction[0][0] *= -1.0f;
        camera.direction[0][1] *= -1.0f;
        camera.direction[0][2] *= -1.0f;
        { f32 h = 448.0f, d = 800.0f, w = 640.0f, cy = 224.0f, cx = 320.0f; RhView_SetScreen(d, w, h, cx, cy); }
        RhView_Modify(&camera);
        count = RhView_SendViewParameters((void *)packet);
        RhView_GetWorldViewMatrix(matrix);
        RhGeom_InvertMatrix(matrix, matrix);
        RhGeom_PushMatrix((s32)matrix);
        RhGeom_MulMatrixVectorArray((void *)(u32)&points, 0, (void *)(u32)&points, 8);
        RhGeom_PopMatrix();
        count = SzModel_DrawWithOutCalc_Pipe(count, arg1, 0, 0, 0, 0, 0);
        RhBase_GetTextureInfoForRender(saved, saved + 1);
        count = RhText_SetTextureFlush((s32)RhBase_DisableRenderFrame((void *)count));
        RhView_PopStack();
        RhView_GetCurView(&camera);
        RhView_Modify(&camera);
        drawPacket = RhBase_SetZBufferMode(RhView_SendViewParameters((void *)count), 0);
        {
            tag_001E6F90((Header_001E6F90 *)(u32)&header, 0, 0x35, 0);
            ExAssignAlpha(&header, 0x8000000044LL);
        }
        i = 0;
        vertex = scratch;
        do {
            f32 * p;
            s32 endpoint;
            s32 side;
            side = i >> 1;
            endpoint = i % 2;
            p = points.f[0][side][endpoint];
            vertex->r = 0;
            vertex->g = 0;
            vertex->b = 0;
            vertex->a = opacityScale;
            vertex->q = 1.0f;
            Ex3d_IntersectP(plane.f, p, p + 16, p + 32);
            i++;
            vertex++;
        } while (i < 4);
        band = 0;
        do {
            s32 j;
            s32 side;
            Vertex_001E6F90 *interpolated;
            f32 *p;
            range = data_00292460;
            range.f[0] = (f32)band / 8.0f;
            interpolated = scratch;
            range.f[1] = (f32)(band + 1) / 8.0f;
            j = 0;
            do {
                f32 nextX;
                f32 x;
                f32 t;
                side = j >> 1;
                p = points.f[0][side][0];
                x = p[32];
                endpoint = (j == 0 || j == 3) ? 0 : 1;
                nextX = p[36];
                t = range.f[endpoint];
                delta = nextX - x;
                interpolated->position[0] = x + delta * t;
                delta = p[37] - p[33];
                interpolated->position[1] = p[33] + delta * t;
                x = p[34];
                delta = p[38] - x;
                interpolated->position[2] = x + delta * t;
                interpolated->position[3] = 1.0f;
                interpolated->uv[0] = (640.0f * (f32)side) / 1024.0f;
                j++;
                interpolated->uv[1] = (448.0f * t) / 512.0f;
                interpolated++;
            } while (j < 4);
            pair = 0;
            current = scratch;
            do {
                f32 distance;
                s32 opacity;
                f32 intensity;
                midpoint[0] = (current->position[0] + (opposite = &scratch[3 - pair])->position[0]) / 2.0f;
                midpoint[1] = (current->position[1] + opposite->position[1]) / 2.0f;
                midpoint[2] = (current->position[2] + opposite->position[2]) / 2.0f;
                distance = RhGeom_GetPointsDistance(position, midpoint);
                if (distance < 1.0f) {
                    distance = 1.0f;
                }
                intensity = 1e11f / (distance * (distance * (distance * (distance * distance))));
                intensity = MAX_001E6F90(intensity, 0.0f) < 1.0f
                    ? MAX_001E6F90(intensity, 0.0f) : 1.0f;
                opacity = func_001a23b0((f32)opacityScale * intensity);
                opposite->a = opacity;
                current->a = opacity;
                current++;
                pair++;
            } while (pair < 2);
            count = Ex3d_NodeL2V(0, 4, scratch, 0xAA, scratchpad, 15, 0);
            if (count >= 3) {
                Ex3d_NodeV2S(0, count, scratch);
                {
                    Vertex_001E6F90 * p;
                    s32 n;
                    n = 0;
                    if (n < count) {
                        p = scratch;
                        do {
                            p->color.fields.flags = 0xFFFFF6;
                            n++;
                            p++;
                        } while (n < count);
                    }
                }
                drawPacket = ExSortNode_Pipe(drawPacket, &header, count, scratch, 0, 0);
            }
            band++;
        } while (band < 7);
        opacityScale = RhBase_SetZBufferMode(drawPacket, 1);
        RhBase_FreeRenderFrame();
        arg0 = (s32)ExDitherMode_Pipe((void *)opacityScale, 1);
    }
    return (s32)arg0;
}

#pragma opt_dead_assignments reset
