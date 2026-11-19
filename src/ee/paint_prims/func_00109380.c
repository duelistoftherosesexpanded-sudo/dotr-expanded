#include "dotr/ee/paint_prims.h"

/* 0x109380 GT4_Xyz_ZIN (name from the Japanese build's symbols). */
int GT4_Xyz_ZIN(void *packet, int mode)
{
    if (mode == 0) {
        struct DotrPaintGt4Packet *gt4 = packet;

        gt4->entries[0].color = gt4->entries[1].color = gt4->entries[2].color = gt4->entries[3].color = 0xffffff;
    } else {
        struct DotrPaintWidePacket *g4 = packet;

        g4->entries[0].color = g4->entries[1].color = g4->entries[2].color = g4->entries[3].color = 0xffffff;
    }
    return 0;
}
