#include "dotr/ee/block_table.h"

int SzView_GetMode(int index)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return -1;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return -1;
    }
    return p->viewMode;
}
