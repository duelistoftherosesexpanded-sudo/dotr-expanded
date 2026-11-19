#include "dotr/ee/block_table.h"

int SzView_GetStraightLightVector(int index, int k, int m, struct DotrBlockVector4 *out)
{
    struct DotrBlock *p;
    struct DotrBlockElement *e;
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
    if (m < 0 || m >= 3) {
        return 0;
    }
    e = &p->modelViewData[k];
    if (out != 0) {
        *out = e->unknown_00[m].unknown_00[2];
    }
    return 1;
}
