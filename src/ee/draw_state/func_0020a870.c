#include "dotr/ee/draw_state.h"

long *RhBase_ClearRenderFrame(long *p, struct DotrDrawColor *color)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    struct DotrDrawColor local = data_00293f00;

    if (!data_00325f20_setup.unknown_20.unknown_bit_0) {
        return p;
    }
    if (!s->unknown_20.unknown_bit_1) {
        return p;
    }
    if (color == 0) {
        color = &local;
    }
    ((int *)p)[0] = 0x10000009;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = 0x11000000;
    ((int *)p)[3] = 0x51000009;
    p[2] = 0x1000000000008008;
    p[3] = 14;
    p[4] = 0;
    p[5] = 74;
    p += 6;
    p += func_00184ff8(p, 2, 0, 0, 4095, 4095, (unsigned char)(color ? color->c[2] : 0), (unsigned char)(color ? color->c[1] : 0),
                       (unsigned char)(color ? color->c[0] : 0), (unsigned char)(color ? color->c[3] : 0), 0) * 2;
    p[0] = 1;
    p[1] = 74;
    return p + 2;
}
