#include "dotr/ee/block_table.h"

int SzView_IsFogY(int index, int k)
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
    if (p->unknown_8ae & (1 << k)) {
        return 1;
    }
    return (p->unknown_8ae & 0x200) ? 1 : 0;
}
