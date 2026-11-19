#include "dotr/ee/paint_prims.h"

int G4_BOX_IN__(struct DotrPaintPacket *packet)
{
    struct DotrPaintBoxEntry *entries = (struct DotrPaintBoxEntry *)packet->entries;
    int h = 240;
    int w = 320;
    unsigned short x0;
    unsigned short y0;
    unsigned short x1;
    unsigned short y1;
    /* MATCHING DEVICE: each coordinate is assigned just before its first use;
     * initialising the four at their declarations gives the same code with other
     * registers. */
    entries[3].unknown_04 = 0;
    entries[2].unknown_04 = 0;
    entries[1].unknown_04 = 0;
    entries[0].unknown_04 = 0;
    x0 = (2048 - w) << 4;
    entries[0].x = x0;
    y0 = (2048 - h) << 4;
    entries[0].y = y0;
    entries[0].z = 0xffffff;
    entries[0].unknown_high = 0;
    x1 = (w + 2048) << 4;
    entries[1].x = x1;
    entries[1].y = y0;
    entries[1].z = 0xffffff;
    entries[1].unknown_high = 0;
    entries[2].x = x0;
    y1 = (h + 2048) << 4;
    entries[2].y = y1;
    entries[2].z = 0xffffff;
    entries[2].unknown_high = 0;
    entries[3].x = x1;
    entries[3].y = y1;
    entries[3].z = 0xffffff;
    entries[3].unknown_high = 0;
    return 0;
}
