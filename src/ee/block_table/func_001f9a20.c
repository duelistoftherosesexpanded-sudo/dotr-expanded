#include "dotr/ee/block_table.h"

float SzView_GetScreenDistance(int index)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0.0f;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0.0f;
    }
    return p->screenDistance;
}
