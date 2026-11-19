#include "dotr/ee/cursor_packets.h"
#include "dotr/ee/cursor_pad_geom.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* lSzDuel_DrawWindow_DualShock2_Pipe: draws the pad icon (entry 1 of data_00290800) as a
 * textured quad centred on (x, y) and rotated about z by the current player's angle word
 * (+0x1d4) relative to 0x4000 for side 0 and 0xc000 otherwise. The corner template is copied
 * from 0x2912e0, its x and y set to the icon's half width and height, rotated through
 * RhGeom_GetRotXYZMatrix / RhGeom_MulMatrixVectorArray into the scratchpad, and each vertex is
 * ((x + 1728) << 4) + (int)(16 * corner) - 8 (same for y with 1824), z from the control word at
 * +0xc58. Colour is 128, 128, 128, alpha. The packet goes through ExSortPrim_Pipe between
 * RhBase_SetZBufferMode(cursor, 0) and (.., 1).
 * Compiled warm: the float-to-integer conversions are casts. MATCHING DEVICE: the scratchpad at 0x70000000 is
 * addressed through the literal address (DOTR_CURSOR_PAD_SCRATCH, a host array on the host). */
DotrDrawWord lSzDuel_DrawWindow_DualShock2_Pipe(DotrDrawWord packet, int x, int y, int alpha)
{
    struct DotrCursorPicControl *state;
    struct DotrCursorPadIcon *texture;
    int originX;
    int originY;
    unsigned char *player;
    int base;
    int halfWidth;
    int halfHeight;
    struct DotrCursorDs2Quad corners;
    struct DotrCursorHandRotation rotation;
    struct DotrCursorHandMatrix matrix;
    struct DotrCursorHandPacket sprite __attribute__((aligned(8)));

    state = &data_00317490;
    player = (unsigned char *)&data_00317490 + WhoGoesFirst_ds2 * 0x350;
    texture = (struct DotrCursorPadIcon *)data_0029080a_ds2;
    originX = x + 0x6C0;
    originY = y + 0x720;
    halfWidth = (texture->u1 - texture->u0) >> 1;
    halfHeight = (texture->v1 - texture->v0) >> 1;
    corners = data_002912e0;
    base = WhoGoesFirst_ds2 ? 0xC000 : 0x4000;
    corners.v[0][0] = -(float)halfWidth;
    corners.v[0][1] = -(float)halfHeight;
    corners.v[1][0] = (float)halfWidth;
    corners.v[1][1] = -(float)halfHeight;
    corners.v[2][0] = -(float)halfWidth;
    corners.v[2][1] = (float)halfHeight;
    corners.v[3][0] = (float)halfWidth;
    corners.v[3][1] = (float)halfHeight;
    rotation = data_00291320;
    rotation.v[2] = -(*(int *)(player + 0x1D4) - base);
    ((struct DotrCursorHandTag *)sprite.tag)->flags = 0x24;
    ((struct DotrCursorHandTag *)sprite.tag)->count = 9;
    ((struct DotrCursorHandTag *)sprite.tag)->registers = 0x424242421ULL;
    ((struct DotrCursorHandTag *)sprite.tag)->padding = 0;
    ((struct DotrCursorHandTag *)sprite.tag)->flags |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&sprite, 0x8000000044ULL);
    sprite.r = 0x80;
    sprite.g = 0x80;
    sprite.b = 0x80;
    sprite.a = alpha;
    sprite.q = 1.0f;
    /* the texture words at +0x6a0 of the control block hold addresses */
    ExAssignTexture((struct DotrCursorPacketHeader *)&sprite, (int *)(DotrDrawWord)state->textures[texture->texture], 0);
    ((struct DotrCursorHandUv *)&sprite.corners[0].uv)->u = texture->u0 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[0].uv)->v = texture->v0 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[1].uv)->u = texture->u1 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[1].uv)->v = texture->v0 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[2].uv)->u = texture->u0 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[2].uv)->v = texture->v1 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[3].uv)->u = texture->u1 * 16;
    ((struct DotrCursorHandUv *)&sprite.corners[3].uv)->v = texture->v1 * 16;
    RhGeom_GetRotXYZMatrix((float *)&matrix, (int *)&rotation);
    RhGeom_PushMatrix((float *)&matrix);
    RhGeom_MulMatrixVectorArray((float *)DOTR_CURSOR_PAD_SCRATCH, 0, (float *)&corners, 4);
    RhGeom_PopMatrix();
    sprite.corners[0].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[0]) - 8;
    sprite.corners[0].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[1]) - 8;
    sprite.corners[0].xyz.z = state->z;
    sprite.corners[0].xyz.fog = 0;
    sprite.corners[1].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[4]) - 8;
    sprite.corners[1].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[5]) - 8;
    sprite.corners[1].xyz.z = state->z;
    sprite.corners[1].xyz.fog = 0;
    sprite.corners[2].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[8]) - 8;
    sprite.corners[2].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[9]) - 8;
    sprite.corners[2].xyz.z = state->z;
    sprite.corners[2].xyz.fog = 0;
    sprite.corners[3].xyz.x = (originX << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[12]) - 8;
    sprite.corners[3].xyz.y = (originY << 4) + (int)(16.0f * ((float *)DOTR_CURSOR_PAD_SCRATCH)[13]) - 8;
    sprite.corners[3].xyz.z = state->z;
    sprite.corners[3].xyz.fog = 0;
    packet = RhBase_SetZBufferMode(packet, 0);
    packet = ExSortPrim_Pipe(packet, &sprite, 0, 0);
    return RhBase_SetZBufferMode(packet, 1);
}
