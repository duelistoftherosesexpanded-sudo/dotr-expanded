#include "dotr/ee/block_table.h"

void *SzView_GetSP(int index)
{
    struct DotrBlockDelta *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_delta[index];
    if (p == 0) {
        return 0;
    }
    return &p->sourcePoint;
}
