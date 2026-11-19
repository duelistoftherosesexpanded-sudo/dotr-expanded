#include "dotr/ee/draw_state.h"

struct DotrDrawPacket *ExDitherMode_Pipe(struct DotrDrawPacket *packet, int value)
{
    long *q = (long *)packet;
    q[1] = 0;
    q[0] = 0;
    ((unsigned char *)packet)[3] = 16;
    q[2] = 0x1000000000008001;
    q[3] = 14;
    q[4] = value;
    q[5] = 69;
    packet->unknown_count = 2;
    ((int *)packet)[2] = 0x11000000;
    ((int *)packet)[3] = 0x51000002;
    return packet + 3;
}
