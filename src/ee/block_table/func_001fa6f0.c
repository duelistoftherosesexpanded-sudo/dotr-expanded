#include "dotr/ee/block_table.h"

struct DotrBlockSlot *SzView_InqRequest(int a, int b)
{
    struct DotrBlockSlot *e = gSzView_CameraReq;
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
        return e;
    }
    return 0;
}
