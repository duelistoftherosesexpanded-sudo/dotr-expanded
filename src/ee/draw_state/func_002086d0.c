#include "dotr/ee/draw_state.h"

int ExAssignAlpha(struct DotrDrawPacket *packet, long value)
{
    long *q = (long *)(packet + 1);
    int n = packet->unknown_count;
    packet->unknown_flags |= 0x200;
    q[n] = value;
    return 1;
}
