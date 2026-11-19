#include "dotr/ee/cursor_packets.h"
#include "dotr/ee/cursor_pad_geom.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* lSzDuel_OpenHandCursor_Pipe (0x1e4570): draws the open-hand cursor as two textured quads
 * (icon entries 2 and 3 of data_00290800) rotated about z by `angle` around (x + 2048,
 * y + 2048); the second ExSortPrim_Pipe result is returned.
 * Compiled warm: the float-to-integer conversions are casts. */

static inline int abs_hand(int value) {
    return value >= 0 ? value : -value;
}

DotrDrawWord lSzDuel_OpenHandCursor_Pipe(DotrDrawWord packet, int angle, int x, int y) {
    struct DotrCursorPicControl *state;
    struct DotrCursorPadIcon *first;
    struct DotrCursorPadIcon *second;
    int originX;
    int originY;
    int halfWidth;
    int halfHeight;
    int width;
    int height;
    int du;
    int dv;
    int turn;
    struct DotrCursorHandQuad corners;
    struct DotrCursorHandRotation rotation;
    struct DotrCursorHandMatrix matrix;
    struct DotrCursorHandPacket sprite __attribute__((aligned(8)));

    state = &data_00317490;
    first = (struct DotrCursorPadIcon *)data_00290814_hand;
    second = (struct DotrCursorPadIcon *)data_0029081e_hand;
    halfWidth = abs_hand(first->u1 - first->u0) >> 1;
    halfHeight = abs_hand(first->v1 - first->v0) >> 1;
    width = abs_hand(second->u1 - second->u0);
    height = abs_hand(second->v1 - second->v0);
    corners = data_00291600_hand;
    corners.v[0][0] = -(float)halfWidth;
    corners.v[0][1] = -(float)halfHeight;
    corners.v[1][0] = (float)halfWidth;
    corners.v[1][1] = -(float)halfHeight;
    corners.v[2][0] = -(float)halfWidth;
    corners.v[2][1] = (float)halfHeight;
    corners.v[3][0] = (float)halfWidth;
    corners.v[3][1] = (float)halfHeight;
    corners.v[4][0] = (float)-halfWidth;
    corners.v[4][1] = (float)-halfHeight;
    corners.v[5][0] = (float)(width - halfWidth);
    corners.v[5][1] = (float)-halfHeight;
    corners.v[6][0] = (float)-halfWidth;
    corners.v[6][1] = (float)(height - halfHeight);
    corners.v[7][0] = (float)(width - halfWidth);
    corners.v[7][1] = (float)(height - halfHeight);
    turn = (angle + 0x10000) % 0x10000;
    originY = y + 0x800;
    rotation = data_00291680_hand;
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
    RhGeom_GetRotXYZMatrix(matrix.m[0], rotation.v);
    RhGeom_PushMatrix(matrix.m[0]);
    RhGeom_MulMatrixVectorArray(DOTR_CURSOR_PAD_SCRATCH[0], 0, corners.v[0], 8);
    RhGeom_PopMatrix();
    ((struct DotrCursorHandTag *)sprite.tag)->flags = 0x24;
    ((struct DotrCursorHandTag *)sprite.tag)->count = 9;
    ((struct DotrCursorHandTag *)sprite.tag)->registers = 0x424242421ULL;
    ((struct DotrCursorHandTag *)sprite.tag)->padding = 0;
    ((struct DotrCursorHandTag *)sprite.tag)->flags |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&sprite, 0x8000000044ULL);
    sprite.r = 0x80;
    sprite.g = 0x80;
    sprite.b = 0x80;
    sprite.a = 0x80;
    sprite.q = 1.0f;
    sprite.corners[0].xyz.z = state->z;
    sprite.corners[0].xyz.fog = 0;
    sprite.corners[1].xyz.z = state->z;
    sprite.corners[1].xyz.fog = 0;
    sprite.corners[2].xyz.z = state->z;
    sprite.corners[2].xyz.fog = 0;
    sprite.corners[3].xyz.z = state->z;
    sprite.corners[3].xyz.fog = 0;
    ExAssignTexture((struct DotrCursorPacketHeader *)&sprite, (int *)(DotrDrawWord)state->textures[first->texture], 0);
    ((struct DotrCursorHandUv *)&sprite.corners[0].uv)->u = (first->u0 << 4) + du;
    ((struct DotrCursorHandUv *)&sprite.corners[0].uv)->v = (first->v0 << 4) + dv;
    ((struct DotrCursorHandUv *)&sprite.corners[1].uv)->u = (first->u1 << 4) - du;
    ((struct DotrCursorHandUv *)&sprite.corners[1].uv)->v = (first->v0 << 4) + dv;
    ((struct DotrCursorHandUv *)&sprite.corners[2].uv)->u = (first->u0 << 4) + du;
    ((struct DotrCursorHandUv *)&sprite.corners[2].uv)->v = (first->v1 << 4) - dv;
    ((struct DotrCursorHandUv *)&sprite.corners[3].uv)->u = (first->u1 << 4) - du;
    ((struct DotrCursorHandUv *)&sprite.corners[3].uv)->v = (first->v1 << 4) - dv;
    sprite.corners[0].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[0]);
    sprite.corners[0].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[1]);
    sprite.corners[1].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[4]);
    sprite.corners[1].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[5]);
    sprite.corners[2].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[8]);
    sprite.corners[2].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[9]);
    sprite.corners[3].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[12]);
    sprite.corners[3].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[13]);
    packet = ExSortPrim_Pipe(packet, &sprite, 0, 0);
    ExAssignTexture((struct DotrCursorPacketHeader *)&sprite, (int *)(DotrDrawWord)state->textures[second->texture], 0);
    ((struct DotrCursorHandUv *)&sprite.corners[0].uv)->u = (second->u0 << 4) + du;
    ((struct DotrCursorHandUv *)&sprite.corners[0].uv)->v = (second->v0 << 4) + dv;
    ((struct DotrCursorHandUv *)&sprite.corners[1].uv)->u = (second->u1 << 4) - du;
    ((struct DotrCursorHandUv *)&sprite.corners[1].uv)->v = (second->v0 << 4) + dv;
    ((struct DotrCursorHandUv *)&sprite.corners[2].uv)->u = (second->u0 << 4) + du;
    ((struct DotrCursorHandUv *)&sprite.corners[2].uv)->v = (second->v1 << 4) - dv;
    ((struct DotrCursorHandUv *)&sprite.corners[3].uv)->u = (second->u1 << 4) - du;
    ((struct DotrCursorHandUv *)&sprite.corners[3].uv)->v = (second->v1 << 4) - dv;
    sprite.corners[0].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[16]);
    sprite.corners[0].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[17]);
    sprite.corners[1].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[20]);
    sprite.corners[1].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[21]);
    sprite.corners[2].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[24]);
    sprite.corners[2].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[25]);
    sprite.corners[3].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[28]);
    sprite.corners[3].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[29]);
    return ExSortPrim_Pipe(packet, &sprite, 0, 0);
}
