#include "dotr/ee/block_table.h"

int SzView_SetRoll(int index, int rollAmount)
{
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return 0;
    }
    p->unknown_040 = rollAmount;
    return 1;
}
