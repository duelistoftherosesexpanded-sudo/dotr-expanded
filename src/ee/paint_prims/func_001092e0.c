#include "dotr/ee/paint_prims.h"

/* 0x1092e0 PAINT__G4_SET_IN (name from the Japanese build's symbols). */
int PAINT__G4_SET_IN(struct DotrPaintPacket *packet, long mode)
{
    packet->flags = 0x14;
    packet->count = 8;
    packet->regs = 0x41414141;
    packet->unknown_04 = 0;
    if (mode == 0) {
        ExAssignAlpha(packet, 0x8000000048);
        packet->entries[3].unknown_04 = 0;
    } else {
        ExAssignAlpha(packet, 0x8000000042);
        packet->entries[3].unknown_04 = 0;
    }
    packet->entries[2].unknown_04 = 0;
    packet->entries[1].unknown_04 = 0;
    packet->entries[0].unknown_04 = 0;
    packet->entries[0].unknown_0f = 0;
    packet->entries[1].unknown_0f = 0;
    packet->entries[2].unknown_0f = 0;
    packet->entries[3].unknown_0f = 0;
    return 0;
}
