#include "dotr/ee/block_table.h"

int SzView_SetStraightLight(int index, int k, union DotrBlockElement144 *in)
{
    /* Two views of one pointer: the element copy is 16-byte aligned (q), the rest uses the group's
     * block view. */
    struct DotrBlockElements *q;
    struct DotrBlock *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    q = (struct DotrBlockElements *)gSzView_hView_blocks[index];
    if (q == 0) {
        return 0;
    }
    if (k < 0 || k >= 10) {
        return 0;
    }
    if (in != 0) {
        p = (struct DotrBlock *)q;
        if (in != (union DotrBlockElement144 *)1) {
            q->modelViewData[k] = *in;
            SzView_SetStraightLightDirection(index, k, 0, &p->modelViewData[k].unknown_00[0].unknown_00[1]);
            SzView_SetStraightLightDirection(index, k, 1, &p->modelViewData[k].unknown_00[1].unknown_00[1]);
            SzView_SetStraightLightDirection(index, k, 2, &p->modelViewData[k].unknown_00[2].unknown_00[1]);
        }
        p->unknown_8a8 |= 1 << k;
    } else {
        p = (struct DotrBlock *)q;
        p->unknown_8a8 &= ~(1 << k);
    }
    return 1;
}
