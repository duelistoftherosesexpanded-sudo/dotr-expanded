#include "dotr/ee/draw_state.h"

long *RhBase_MaskFBuffer(long *p, unsigned int mask)
{
    struct DotrDrawState *r = &gRhBase_dl;
    struct DotrDrawFrameWords *s = &data_00325f20_frame;
    struct DotrDrawFrameFields *f;

    if (r->unknown_26c & 1) {
        f = (struct DotrDrawFrameFields *)((char *)r + 336);
    } else {
        f = (struct DotrDrawFrameFields *)((char *)r + 96);
    }
    ((int *)p)[0] = 0x10000002;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = 0x11000000;
    ((int *)p)[3] = 0x51000002;
    p[2] = 0x1000000000008001;
    p[3] = 14;
    if (s->unknown_20.unknown_bit_0 && s->unknown_20.unknown_bit_1) {
        p[4] = (unsigned long)s->unknown_0c
            | (unsigned long)(*(int *)(data_00293f1c + s->unknown_00 * 40) >> 6) << 16
            | (unsigned long)s->unknown_14 << 24 | (unsigned long)mask << 32;
        p += 5;
    } else {
        p[4] = ((unsigned long)f->unknown_00 & 0x1ff) | ((unsigned long)f->unknown_02 & 0x3f) << 16
            | ((unsigned long)f->unknown_03 & 0x3f) << 24 | (unsigned long)mask << 32;
        p += 5;
    }
    p[0] = 76;
    return p + 1;
}
