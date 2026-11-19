#include "dotr/ee/block_table.h"

int SzView_SetMode(int index, int v)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    p->viewMode = v;
    return 1;
}
