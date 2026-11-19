#include "dotr/ee/owner_draws.h"

int SzDuel_ClearDrawCard(struct DotrOwnerDrawBlock *block)
{
    if (block == 0) {
        return 0;
    }
    block->unknown_00[0] = 0;
    block->unknown_00[1] = 0;
    block->unknown_00[2] = 0;
    block->unknown_0c = 1.0f;
    block->unknown_10 = 16384;
    block->unknown_14[0] = 0;
    block->unknown_14[1] = 0;
    block->unknown_14[2] = 0;
    block->unknown_14[3] = 0;
    block->unknown_14[4] = 0;
    block->unknown_14[5] = 0;
    block->unknown_14[6] = 0;
    block->unknown_30 = 0.1f;
    block->unknown_34 = 0.1f;
    block->unknown_38 = 1.0f;
    block->unknown_3c = 0;
    block->color[2] = 128;
    block->color[1] = 128;
    block->color[0] = 128;
    block->color[3] = 128;
    block->unknown_44[2] = 0;
    block->unknown_44[1] = 0;
    block->unknown_44[0] = 0;
    block->unknown_44[3] = 0;
    block->unknown_4a = 7;
    block->unknown_48 = 0;
    block->unknown_50 = 0;
    block->unknown_4c = 0;
    block->unknown_78 = 0x70002000;
    block->unknown_7c = 0x70000000;
    block->unknown_80_bits_0_11 = 170;
    block->count = 0;
    block->unknown_82_bits_12_13 = 1;
    block->flag_12 = -1;
    block->flag_13 = 0;
    block->flag_14 = -1;
    block->flag_15 = -1;
    block->flag_30 = 0;
    block->flag_31 = 0;
    return 1;
}
