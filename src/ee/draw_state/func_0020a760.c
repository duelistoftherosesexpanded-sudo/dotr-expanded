#include "dotr/ee/draw_state.h"

long *RhBase_DisableRenderFrame(long *p)
{
    struct DotrDrawState *r = &gRhBase_dl;
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    long *f;

    if (gRhBase_dl.unknown_26c & 1) {
        f = (long *)((char *)r + 336);
    } else {
        f = (long *)((char *)r + 96);
    }
    if (!s->unknown_20.unknown_bit_0) {
        return p;
    }
    ((int *)p)[0] = 0x10000005;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = 0x11000000;
    ((int *)p)[3] = 0x51000005;
    p[2] = 0x1000000000008004;
    p[3] = 14;
    p[4] = f[0];
    p[5] = f[1];
    p[6] = f[2];
    p[7] = f[3];
    p[8] = 0;
    p[9] = 74;
    p[10] = (long)(r->unknown_23c - 1) << 16 | (long)(r->unknown_240 - 1) << 48;
    p[11] = 64;
    s->unknown_20.unknown_bit_1 = 0;
    return p + 12;
}
