#include "dotr/ee/draw_state.h"

long ExGetRegVal(int a, int b)
{
    if (b < 0 || b >= 7) {
        return -1;
    }
    return gExPrim_aaRegs[a][b].unknown_00;
}
