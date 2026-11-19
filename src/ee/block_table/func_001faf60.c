#include "dotr/ee/block_table.h"

struct DotrBlock *SzView_GetLook(int index)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    return p;
}
