#include "dotr/ee/draw_quad_block.h"

/* Card quad in world space; returns whether it faces the viewer. */
int SzDuel_GetCardNode(struct DotrDrawQuadBlock *block, int a)
{
    int *control = data_00317490_drawwords;
    struct DotrDrawQuadVector normal = data_00291540;
    struct DotrDrawQuadVector view;
    float m1[16];
    float m2[16];
    int clip = a ? 15 : -1;
    struct DotrDrawQuadScale *scale;
    int i;

    RhGeom_PushMatrix(0);
    if (block->flag_13) {
        float m3[16];
        float m4[16];
        struct DotrDrawQuadAngles ra = data_00291550;
        struct DotrDrawQuadAngles rb;

        ra.x = block->turn.x;
        ra.z = block->turn.z;
        rb = data_00291560;
        rb.y = block->turn.y;
        scale = block->flag_14 ? &block->scale : 0;
        RhGeom_GetScaleRotXYZTransMatrix(m3, 0, block->rot, block->pos);
        RhGeom_GetRotXYZMatrix(m4, &rb.x);
        RhGeom_MulMatrixRev(m4, m3);
        RhGeom_GetScaleRotXYZTransMatrix(m1, (float *)scale, &ra.x, 0);
        RhGeom_MulMatrixRev(m1, m4);
    } else {
        float m5[16];
        struct DotrDrawQuadAngles rc = data_00291570;
        struct DotrDrawQuadAngles rd;

        rc.x = block->turn.x;
        rc.z = block->turn.z;
        rd = data_00291580;
        rd.y = block->turn.y;
        scale = block->flag_14 ? &block->scale : 0;
        RhGeom_GetScaleRotXYZTransMatrix(m5, 0, &rd.x, block->pos);
        RhGeom_GetScaleRotXYZTransMatrix(m1, (float *)scale, &rc.x, 0);
        RhGeom_MulMatrixRev(m1, m5);
    }
    RhView_GetWorldViewMatrix((struct DotrViewMatrix *)m2);
    RhGeom_MulMatrix(m2, m1);
    RhGeom_MulMatrixVector((struct DotrViewVector *)&normal, (struct DotrViewMatrix *)m2, (struct DotrViewVector *)&normal);
    block->packet.unknown_flags = 53;
    block->packet.unknown_count = 0;
    *(long *)&block->packet.unknown_04[4] = 0;
    *(int *)&block->packet.unknown_04[0] = 0;
    ExAssignAlpha(&block->packet, 0x8000000044L);
    if (block->flag_12) {
        ExAssignClamp(&block->packet, 0x5c00040000aL);
    } else {
        ExAssignClamp(&block->packet, 0x2d0001e000aL);
    }
    block->count = SzDuel_GetCardNodeUnit(block->flag_12, block->vertices);
    block->count = Ex3d_NodeL2V((struct DotrViewMatrix *)m2, block->count, block->vertices, block->unknown_80_bits_0_11,
                                block->unknown_7c, clip, 0);
    Ex3d_GetViewVectorForNode(0, block->count, block->vertices, &view);
    Ex3d_NodeV2S(0, block->count, block->vertices);
    if (control[789]) {
        for (i = 0; i < block->count; i++) {
            ((struct DotrDrawQuadVertexWord *)block->vertices)[i].unknown_10_colour = control[789];
        }
    }
    RhGeom_PopMatrix();
    return RhGeom_InnerProduct(&view, &normal) <= 0.0f ? 0 : 1;
}
