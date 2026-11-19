#include "dotr/ee/draw_state.h"

int RhBase_GetZMinMax(int *a, int *b)
{
    struct DotrDrawSelectorView *s = data_00325f20_view;
    int w = data_003261a8[0];
    if (data_00325f20_view[0].unknown_20.unknown_bit_0) {
        if (s->unknown_20.unknown_bit_1) {
            w = s->unknown_18;
        }
    }
    *a = 0;
    switch (w) {
    case 48: *b = 0x7fffffff; break;
    case 49: *b = 0xffffff; break;
    case 50:
    case 58: *b = 0xffff; break;
    }
    return 1;
}
