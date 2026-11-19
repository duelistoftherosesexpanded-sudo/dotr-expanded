#include "dotr/ee/draw_quad_block.h"

int SzDuel_GetCardNodeOnScreen(struct DotrDrawQuadBlock *block, int a, float x, float y)
{
    int *control = data_00317490_drawwords;
    struct DotrDrawQuadScale *scale = block->flag_14 ? &block->scale : 0;
    int i;
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
    /* MATCHING DEVICE: SzDuel_GetCardNodeUnitOnScreen through its caller-order view, see
     * draw_quad_block.h. */
    block->count = func_001da5e0_caller(block->flag_12, block->vertices, x, y, scale);
    if (a) {
        block->count = Ex3d_ViewClip(block->count, block->vertices, block->unknown_80_bits_0_11, block->unknown_7c, 15);
    }
    Ex3d_NodeV2S(0, block->count, block->vertices);
    if (control[789]) {
        for (i = 0; i < block->count; i++) {
            ((struct DotrDrawQuadVertexWord *)block->vertices)[i].unknown_10_colour = control[789];
        }
    }
    return 1;
}
