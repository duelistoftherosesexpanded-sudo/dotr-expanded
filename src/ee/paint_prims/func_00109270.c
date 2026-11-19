#include "dotr/ee/paint_prims.h"

/* 0x109270 PAINT__GT4_SET_IN (name from the Japanese build's symbols). */
int PAINT__GT4_SET_IN(struct DotrPaintPacket *packet)
{
    packet->flags = 0x34;
    packet->count = 12;
    packet->regs = 0x421421421421;
    packet->unknown_04 = 0;
    packet->flags |= 0x800;
    ExAssignAlpha(packet, 0x8000000048);
    return 0;
}
