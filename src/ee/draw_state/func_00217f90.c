#include "dotr/ee/draw_state.h"

void TAKU_SP_XYWH(void *packet, int x, int y, int width, int height, int z, int f)
{
    DOTR_DRAW_VERTEX(packet, 32)->unknown_x = (x + 1728) << 4;
    DOTR_DRAW_VERTEX(packet, 32)->unknown_y = (y + 1824) << 4;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_x = DOTR_DRAW_VERTEX(packet, 32)->unknown_x + (width << 4);
    DOTR_DRAW_VERTEX(packet, 48)->unknown_y = DOTR_DRAW_VERTEX(packet, 32)->unknown_y + (height << 4);
    DOTR_DRAW_VERTEX(packet, 32)->unknown_z = z;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_z = z;
    DOTR_DRAW_VERTEX(packet, 32)->unknown_f = f;
    DOTR_DRAW_VERTEX(packet, 48)->unknown_f = f;
}
