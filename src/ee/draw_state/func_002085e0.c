#include "dotr/ee/draw_state.h"

DotrDrawWord ExDitherMatrixEz_Pipe(DotrDrawWord cursor, int index)
{
    struct DotrDrawPacket *packet = (struct DotrDrawPacket *)cursor;
    long *q = (long *)packet;
    long value;
    if (index < 0 || index >= 4) {
        return cursor;
    }
    value = g_aqwExDitherMatrix[index];
    q[1] = 0;
    q[0] = 0;
    ((unsigned char *)packet)[3] = 16;
    q[2] = 0x1000000000008001;
    q[3] = 14;
    q[4] = value;
    q[5] = 68;
    packet->unknown_count = 2;
    ((int *)packet)[2] = 0x11000000;
    ((int *)packet)[3] = 0x51000002;
    return (DotrDrawWord)(packet + 3);
}
