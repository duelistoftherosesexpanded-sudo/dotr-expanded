#include "dotr/ee/draw_state.h"

struct DotrDrawPacket *RhBase_SetFogColor(struct DotrDrawPacket *packet, unsigned char *bytes)
{
    int *w = (int *)packet;
    long *q = (long *)packet;
    w[0] = 0x10000002;
    w[1] = 0;
    w[2] = 0;
    w[3] = 0x51000002;
    q[2] = 0x1000000000008001;
    q[3] = 14;
    q[4] = (unsigned long)bytes[2] | ((unsigned long)bytes[1] << 8) | ((unsigned long)bytes[0] << 16);
    q[5] = 61;
    return packet + 3;
}
