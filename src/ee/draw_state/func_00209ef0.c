#include "dotr/ee/draw_state.h"

int RhBase_GetBufferFormatEx(int a, int *x, int *y, int *z)
{
    struct DotrDrawState *s = &gRhBase_dl;
    if (x != 0) {
        unsigned short *h;
        if ((s->unknown_26c & 1) == (a ? 1 : 0)) {
            h = (unsigned short *)&s->unknown_108[0x48];
        } else {
            h = (unsigned short *)&s->unknown_000[0x60];
        }
        *x = (*h & 0x1ff) << 5;
    }
    if (y != 0) {
        *y = *(int *)&s->unknown_24c[4];
    }
    if (z != 0) {
        *z = *(int *)&s->unknown_24c[12];
    }
    return 1;
}
