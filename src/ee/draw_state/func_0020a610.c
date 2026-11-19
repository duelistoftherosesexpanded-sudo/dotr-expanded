#include "dotr/ee/draw_state.h"

long *RhBase_EnableRenderFrame(long *p)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;

    if (!s->unknown_20.unknown_bit_0) {
        return p;
    }
    ((int *)p)[0] = 0x10000005;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = 0x11000000;
    ((int *)p)[3] = 0x51000005;
    p[2] = 0x1000000000008004;
    p[3] = 14;
    p[4] = (long)s->unknown_0c | (long)(*(int *)(data_00293f1c + s->unknown_00 * 40) >> 6) << 16
        | (long)s->unknown_14 << 24;
    p[5] = 76;
    p[6] = (long)(s->unknown_0c + s->unknown_1c) | (long)s->unknown_18 << 24;
    p[7] = 78;
    p[8] = 1;
    p[9] = 74;
    p[10] = (long)(s->unknown_04 - 1) << 16 | (long)(s->unknown_08 - 1) << 48;
    p[11] = 64;
    s->unknown_20.unknown_bit_1 = -1;
    return p + 12;
}
