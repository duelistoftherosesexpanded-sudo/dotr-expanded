#include "dotr/ee/draw_state.h"

void TAKU_FT4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                   int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3)
{
    DOTR_DRAW_VERTEX(packet, 32)->unknown_x = (x0 + 1728) << 4;
    DOTR_DRAW_VERTEX(packet, 32)->unknown_y = (y0 + 1824) << 4;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_x = (x1 + 1728) << 4;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_y = (y1 + 1824) << 4;
    DOTR_DRAW_VERTEX(packet, 64)->unknown_x = (x2 + 1728) << 4;
    DOTR_DRAW_VERTEX(packet, 64)->unknown_y = (y2 + 1824) << 4;
    DOTR_DRAW_VERTEX(packet, 80)->unknown_x = (x3 + 1728) << 4;
    DOTR_DRAW_VERTEX(packet, 80)->unknown_y = (y3 + 1824) << 4;
    DOTR_DRAW_VERTEX(packet, 32)->unknown_z = z0;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_z = z1;
    DOTR_DRAW_VERTEX(packet, 64)->unknown_z = z2;
    DOTR_DRAW_VERTEX(packet, 80)->unknown_z = z3;
    DOTR_DRAW_VERTEX(packet, 32)->unknown_f = f0;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_f = f1;
    DOTR_DRAW_VERTEX(packet, 64)->unknown_f = f2;
    DOTR_DRAW_VERTEX(packet, 80)->unknown_f = f3;
}
