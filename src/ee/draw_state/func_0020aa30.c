#include "dotr/ee/draw_state.h"

int RhBase_GetTextureInfoForRender(struct DotrDrawBits64A *a, struct DotrDrawBits64B *b)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    int w;
    int h;

    if (!s->unknown_20.unknown_bit_0) {
        return 0;
    }
    if (a != 0) {
        w = *(int *)(data_00293f1c + s->unknown_00 * 40);
        h = *(int *)(data_00293f20 + s->unknown_00 * 40);
        a->unknown_bits_0_13 = s->unknown_0c << 5;
        a->unknown_bits_14_19 = w >> 6;
        a->unknown_bits_20_25 = s->unknown_14;
        a->unknown_bits_26_29 = w <= 320 ? 9 : 10;
        a->unknown_bits_30_33 = h <= 224 ? 8 : 9;
        a->unknown_bit_34 = 1;
        a->unknown_bits_35_36 = 0;
        a->unknown_bits_37_50 = 0;
        a->unknown_bits_51_54 = 0;
        a->unknown_bit_55 = 0;
        a->unknown_bits_56_60 = 0;
        a->unknown_bits_61_63 = 1;
    }
    if (b != 0) {
        b->unknown_bit_0 = 0;
        b->unknown_bits_2_4 = 0;
        b->unknown_bit_5 = 1;
        b->unknown_bits_6_8 = 1;
        b->unknown_bit_9 = 0;
        b->unknown_bits_19_20 = 0;
        b->unknown_bits_32_43 = 0;
    }
    return 1;
}
