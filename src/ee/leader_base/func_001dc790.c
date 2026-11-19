#include "dotr/ee/leader_base.h"

/* Draws the square under a side's leader. Returns the cursor unchanged when
 * SzDuel_GetUnit(side, 0) gives no unit or the unit's word at +4 is set. Otherwise four
 * corner vectors are copied from data_00291590 and given x and z of plus or minus
 * 16.666666 * (byte at +0xd34 of the control storage) / 128, rotated by the model's Y
 * rotation (SzModel_GetRot with the X and Z angles cleared) through the matrix stack and
 * RhGeom_MulMatrixVectorArray, and written to the scratchpad vertices as centre + corner
 * with colour unit brightness * level / 128 (level 128, or the map cell's byte at +0x1a
 * when neither unit flag is set), the caller's alpha and texture coordinates (side half
 * + 0 or 0.5, 0 or 1). The packet header (0, 53, 0, 0) gets alpha 0x8000000044 and the
 * side's texture and goes through Ex3d_NodeL2V, Ex3d_NodeV2S and ExSortNode_Pipe. With
 * `outline` a second header (0, 0x402, 0, 0; alpha 0x8000000048) draws the closed outline
 * (five points) in the map cell's colour scaled the same way.
 * The loop count passed to Ex3d_NodeL2V is the loop variable, as in the original (a1 is
 * not reloaded). The three header member pointers are locals because the original keeps
 * them in stack slots across the calls.
 * MATCHING DEVICE: `(scale = scale * ...)` makes the constant load first; `(src = ...)[0]`
 * inside the sum makes centre[0] load before the corner address is formed. */
int SzDuel_DrawLeaderBase_Pipe(int cursor, int side, float *centre, int alpha, int outline)
{
    struct DotrLeaderBaseControl *control = &data_00317490;
    struct DotrLeaderBaseUnit *unit = SzDuel_GetUnit(side, 0);
    float scale = 16.666666f;
    float half = (scale = scale * (float)(unsigned int)data_00317490.unknown_d34) / 128.0f;
    struct DotrLeaderBaseCorners corners = data_00291590;
    int ang[4];
    float matrix[16];
    long packet[6];
    struct DotrLeaderBaseCell *cell;
    struct DotrLeaderBaseSide *base;
    float u;
    int count;
    unsigned char level;
    short *kind;
    long *word8;
    int *word4;
    float *src;
    int shade;
    int i;

    corners.v[0][0] = half;
    corners.v[0][2] = half;
    corners.v[1][0] = half;
    corners.v[1][2] = -half;
    corners.v[2][0] = -half;
    corners.v[2][2] = -half;
    corners.v[3][0] = -half;
    corners.v[3][2] = half;
    if (side != 0) {
        u = 0.5f;
    } else {
        u = 0.0f;
    }
    base = &gSzDuel_Base[side];
    level = 128;
    if (unit == 0) {
        return cursor;
    }
    if (unit->unknown_04 != 0) {
        return cursor;
    }
    cell = SzDuel_GetMapInfo(unit->unknown_40, unit->unknown_41);
    if (!unit->unknown_57_7 && !unit->unknown_58_1) {
        level = cell->unknown_1a;
    }
    SzModel_GetRot(unit->unknown_46, ang);
    ang[2] = 0;
    ang[0] = 0;
    RhGeom_GetRotXYZMatrix(matrix, ang);
    RhGeom_PushMatrix(matrix);
    RhGeom_MulMatrixVectorArray(corners.v[0], 0, corners.v[0], 4);
    RhGeom_PopMatrix();
    *(kind = &((struct DotrLeaderBasePacket *)packet)->unknown_02) = 53;
    word8 = &((struct DotrLeaderBasePacket *)packet)->unknown_08;
    word4 = &((struct DotrLeaderBasePacket *)packet)->unknown_04;
    ((struct DotrLeaderBasePacket *)packet)->unknown_00 = 0;
    *word8 = 0;
    *word4 = 0;
    ExAssignAlpha((struct DotrLeaderBasePacket *)packet, 0x8000000044L);
    ExAssignTexture((struct DotrLeaderBasePacket *)packet, control->texture[base->texture], 1);
    for (i = 0; i < 4; i++) {
        struct DotrLeaderBaseVertex *p = &DOTR_LEADER_BASE_VERTICES[i];
        shade = (unit->unknown_4d * level) >> 7;
        p->colour[2] = shade;
        p->colour[1] = shade;
        p->colour[0] = shade;
        p->colour[3] = alpha;
        p->unknown_1c = 1.0f;
        p->uv[0] = u + ((i == 0 || i == 1) ? 0.0f : 0.5f);
        p->uv[1] = (i == 0 || i == 3) ? 0.0f : 1.0f;
        p->position[0] = centre[0] + corners.v[i][0];
        p->position[1] = centre[1] + corners.v[i][1];
        p->position[2] = centre[2] + corners.v[i][2];
    }
    count = Ex3d_NodeL2V(0, i, DOTR_LEADER_BASE_VERTICES, 170, 0x70000000, 15, 0);
    Ex3d_NodeV2S(0, count, DOTR_LEADER_BASE_VERTICES);
    cursor = ExSortNode_Pipe(cursor, (struct DotrLeaderBasePacket *)packet, count, DOTR_LEADER_BASE_VERTICES, 0, 0);
    if (outline != 0) {
        *kind = 0x402;
        ((struct DotrLeaderBasePacket *)packet)->unknown_00 = 0;
        *word8 = 0;
        *word4 = 0;
        ExAssignAlpha((struct DotrLeaderBasePacket *)packet, 0x8000000048L);
        DOTR_LEADER_BASE_VERTICES[0].colour[0] = (cell->unknown_12 * level) >> 7;
        DOTR_LEADER_BASE_VERTICES[0].colour[1] = (cell->unknown_11 * level) >> 7;
        DOTR_LEADER_BASE_VERTICES[0].colour[2] = (cell->unknown_10 * level) >> 7;
        DOTR_LEADER_BASE_VERTICES[0].colour[3] = alpha;
        DOTR_LEADER_BASE_VERTICES[0].unknown_1c = 1.0f;
        for (i = 0; i < 5; i++) {
            struct DotrLeaderBaseVertex *p = &DOTR_LEADER_BASE_VERTICES[i];
            p->position[0] = centre[0] + (src = corners.v[i % 4])[0];
            p->position[1] = centre[1] + src[1];
            p->position[2] = centre[2] + src[2];
        }
        count = Ex3d_NodeL2V(0, i, DOTR_LEADER_BASE_VERTICES, 170, 0x70000000, 15, 0);
        Ex3d_NodeV2S(0, count, DOTR_LEADER_BASE_VERTICES);
        cursor = ExSortNode_Pipe(cursor, (struct DotrLeaderBasePacket *)packet, count, DOTR_LEADER_BASE_VERTICES, 0, 0);
    }
    return cursor;
}
