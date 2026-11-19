#include "dotr/ee/draw_state.h"

int RhBase_SetClearColorEx(int a, int b, int c, int d)
{
    gRhBase_dl.unknown_100[0] = a;
    gRhBase_dl.unknown_100[1] = b;
    gRhBase_dl.unknown_100[2] = c;
    gRhBase_dl.unknown_100[3] = d;
    gRhBase_dl.unknown_104 = 0;
    gRhBase_dl.unknown_1f0[0] = a;
    gRhBase_dl.unknown_1f0[1] = b;
    gRhBase_dl.unknown_1f0[2] = c;
    gRhBase_dl.unknown_1f0[3] = d;
    gRhBase_dl.unknown_1f4 = 0;
    return 1;
}
