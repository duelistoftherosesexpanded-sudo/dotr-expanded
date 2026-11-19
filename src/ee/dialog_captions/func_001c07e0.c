#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawBuildDeck_ReincarnationDone_Pipe(DotrCaptionWord cursor, int x, int y)
{
    struct DotrCaptionControl *g = &data_00317490_caption;
    char *text = data_002bef9b_caption + 2;
    unsigned short *ids = (unsigned short *)(g->work + 32);
    int name = -1;
    int w;
    int h;

    if (g->choice >= 128) {
        if (g->step < 3) {
            name = func_001de210_caption(ids[g->step]);
        } else {
            name = 147;
        }
    }
    cursor = func_001b0ce0_caption(cursor, 2, -1, name, x + 320, y + 93, -460, 262, func_0019bef0_caption(text), text,
                                   (g->choice < 128 ? 0L : 1L));
    if (g->step == 0) {
        cursor = func_002084d0_caption(cursor, ((long)g->choice << 14) | 0xa | ((long)g->choice << 34));
        w = h = g->choice;
    } else {
        h = w = 128;
    }
    cursor = func_001b7d60_caption(cursor, ids[0], x + 116, y + 187, w, h, g->pic_z);
    if (g->step < 1) {
        w = h = 0;
    } else if (g->step == 1) {
        cursor = func_002084d0_caption(cursor, ((long)g->choice << 14) | 0xa | ((long)g->choice << 34));
        w = h = g->choice;
    } else {
        h = w = 128;
    }
    cursor = func_001b7d60_caption(cursor, ids[1], x + 256, y + 187, w, h, g->pic_z);
    if (g->step < 2) {
        w = h = 0;
    } else if (g->step == 2) {
        cursor = func_002084d0_caption(cursor, ((long)g->choice << 14) | 0xa | ((long)g->choice << 34));
        w = h = g->choice;
    } else {
        h = w = 128;
    }
    cursor = func_001b7d60_caption(cursor, ids[2], x + 396, y + 187, w, h, g->pic_z);
    if (g->step < 3) {
        cursor = func_002084d0_caption(cursor, ExGetRegVal(0, 4));
    }
    return cursor;
}
