#include "dotr/ee/block_table.h"

int SzView_SetFogColor(int index, int k, struct DotrBlockQuad *in)
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
    if (in != 0) {
        p->fogColour[k] = *in;
    } else {
        p->fogColour[k].unknown_bytes[0] = 255;
        p->fogColour[k].unknown_bytes[1] = 255;
        p->fogColour[k].unknown_bytes[2] = 255;
    }
    return 1;
}
