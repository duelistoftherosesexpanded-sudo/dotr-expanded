#include "dotr/ee/draw_state.h"

int RhBase_AssignRenderFrame(int index, int mode, int kind)
{
    struct DotrDrawState *r = &gRhBase_dl;
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    int x = 0;
    int y = 0;
    int *row;

    if (s->unknown_20.unknown_bit_0) {
        return 0;
    }
    s->unknown_00 = index;
    s->unknown_04 = *(int *)(data_00293f1c + index * 40);
    s->unknown_08 = *(int *)(data_00293f20 + index * 40);
    s->unknown_14 = mode;
    s->unknown_18 = kind;
    switch (mode) {
    case 0:
        x = 1;
        /* fall through */
    case 1:
        x = 1;
        break;
    case 2:
        x = 0;
        /* fall through */
    case 10:
        x = 0;
        break;
    }
    switch (kind) {
    case 48:
        y = 1;
        /* fall through */
    case 49:
        y = 1;
        break;
    case 50:
        y = 0;
        /* fall through */
    case 58:
        y = 0;
        break;
    }
    s->unknown_0c = r->unknown_268 >> 5;
    row = (int *)(data_00293f30 + index * 40);
    s->unknown_1c = row[x];
    s->unknown_10 = row[x] + row[y];
    if (s->unknown_0c + s->unknown_10 > 512) {
        return 0;
    }
    s->unknown_20.unknown_bit_0 = -1;
    s->unknown_20.unknown_bit_1 = 0;
    return 1;
}
