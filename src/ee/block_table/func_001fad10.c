#include "dotr/ee/block_table.h"

void *SzView_GetRel(int index)
{
    struct DotrBlock *p;

    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    return &p->unknown_000[0x10];
}
