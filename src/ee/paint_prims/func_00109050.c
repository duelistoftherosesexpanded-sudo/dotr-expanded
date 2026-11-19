#include "dotr/ee/paint_prims.h"

/* 0x109050 PAINT_PIRM_LG2 (name from the Japanese build's symbols). */
int PAINT_PIRM_LG2(void *source, struct DotrPaintWidePacket *packet, int arg2, int arg3)
{
    char out[16];

    if (Ex3d_RotTransPersAuto(source, 0x70000000, arg2, packet, out) != 0) {
        packet->entries[0].color = 0xffffff;
        packet->entries[1].color = 0xffffff;
        ExSortPrim(packet, arg3, 0);
    }
    return 0;
}
