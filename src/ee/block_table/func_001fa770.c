#include "dotr/ee/block_table.h"

int SzView_PauseRequest(int a, int b, int flag)
{
    struct DotrBlockSlot *e = gSzView_CameraReq;
    int r = 0;
    int i;
    for (i = 0; i < 8; i++, e++) {
        if (!e->unknown_00.unknown_bit_0) {
            continue;
        }
        if (a >= 0 && e->unknown_02 != a) {
            continue;
        }
        if (b >= 0 && e->unknown_03 != b) {
            continue;
        }
        e->unknown_01.unknown_bit_0 = (signed char)(flag ? 1 : 0);
        r = 1;
    }
    return r;
}
