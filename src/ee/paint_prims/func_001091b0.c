#include "dotr/ee/paint_prims.h"

/* 0x1091b0 PAINT__2G_SET_IN (name from the Japanese build's symbols). */
int PAINT__2G_SET_IN(struct DotrPaintPacket *packet, unsigned char *first, unsigned char *second)
{
    packet->flags = 0x11;
    packet->count = 4;
    packet->regs = 0x4141;
    packet->unknown_04 = 0;
    ExAssignAlpha(packet, 0x8000000048);
    packet->entries[0].rgba[0] = first[0];
    packet->entries[0].rgba[1] = first[1];
    packet->entries[0].rgba[2] = first[2];
    packet->entries[0].rgba[3] = first[3];
    packet->entries[1].rgba[0] = second[0];
    packet->entries[1].rgba[1] = second[1];
    packet->entries[1].rgba[2] = second[2];
    packet->entries[1].rgba[3] = second[3];
    packet->entries[1].unknown_04 = 0x3f800000;
    packet->entries[0].unknown_04 = 0x3f800000;
    packet->entries[0].unknown_0f = 0;
    packet->entries[1].unknown_0f = 0;
    return 0;
}
