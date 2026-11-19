#include "dotr/ee/block_table.h"

int SzView_SetStraightLightColor(int index, int k, int m, struct DotrBlockVector4 *in)
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
    if (in != 0) {
        e->unknown_00[m].unknown_00[0] = *in;
    } else {
        e->unknown_00[m].unknown_00[0].y = e->unknown_00[m].unknown_00[0].z = e->unknown_00[m].unknown_00[0].unknown_w = m > 0 ? 0.0f : 0.5f;
    }
    return 1;
}
