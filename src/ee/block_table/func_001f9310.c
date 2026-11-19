#include "dotr/ee/block_table.h"

int SzView_SetFogParamY(int index, int k, struct DotrBlockPair *in)
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
    if (in != 0) {
        if (in != (struct DotrBlockPair *)1) {
            p->unknown_830[k] = *in;
        }
        p->unknown_8ae |= 1 << k;
    } else {
        p->unknown_830[k].x = 2000.0f;
        p->unknown_830[k].y = 4000.0f;
        p->unknown_8ae &= ~(1 << k);
    }
    return 1;
}
