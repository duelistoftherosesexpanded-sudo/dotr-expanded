#include "dotr/ee/block_table.h"

int SzView_InqStraightLight(int index, int k)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    if (k < 0 || k >= 10) {
        return 0;
    }
    return (p->unknown_8a8 & (1 << k)) ? 1 : 0;
}
