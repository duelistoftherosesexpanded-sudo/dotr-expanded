#include "dotr/ee/draw_state.h"

int ExAssignClamp(struct DotrDrawPacket *packet, long value)
{
    unsigned short count = packet->unknown_count;
    long *words = (long *)(packet + 1);
    unsigned short *flags = &packet->unknown_flags;
    if ((packet->unknown_flags & 0x20) == 0) return 0;
    *flags |= 0x4000;
    {
        long *entry = words + count;
        entry[3] = value;
    }
    return 1;
}
