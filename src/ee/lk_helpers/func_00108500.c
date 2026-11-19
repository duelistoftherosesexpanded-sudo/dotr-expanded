#include "dotr/ee/lk_helpers.h"

/* Writes a texel rectangle {u, v, w, h} to the four corners; `mode` picks which vertex
 * gets which corner. */
void POLPAN_UVIN_(struct DotrLkUvPacket *packet, short *rect, int mode)
{
    if (mode == 0) {
        packet->vertex[0].u = rect[0] << 4;
        packet->vertex[0].v = rect[1] << 4;
        packet->vertex[1].u = (rect[0] + rect[2]) << 4;
        packet->vertex[1].v = rect[1] << 4;
        packet->vertex[2].u = rect[0] << 4;
        packet->vertex[2].v = (rect[1] + rect[3]) << 4;
        packet->vertex[3].u = (rect[0] + rect[2]) << 4;
        packet->vertex[3].v = (rect[1] + rect[3]) << 4;
    } else if (mode == 1) {
        packet->vertex[0].u = rect[0] << 4;
        packet->vertex[0].v = (rect[1] + rect[3]) << 4;
        packet->vertex[1].u = rect[0] << 4;
        packet->vertex[1].v = rect[1] << 4;
        packet->vertex[2].u = (rect[0] + rect[2]) << 4;
        packet->vertex[2].v = (rect[1] + rect[3]) << 4;
        packet->vertex[3].u = (rect[0] + rect[2]) << 4;
        packet->vertex[3].v = rect[1] << 4;
    } else if (mode == 10) {
        packet->vertex[2].u = rect[0] << 4;
        packet->vertex[2].v = rect[1] << 4;
        packet->vertex[0].u = (rect[0] + rect[2]) << 4;
        packet->vertex[0].v = rect[1] << 4;
        packet->vertex[3].u = rect[0] << 4;
        packet->vertex[3].v = (rect[1] + rect[3]) << 4;
        packet->vertex[1].u = (rect[0] + rect[2]) << 4;
        packet->vertex[1].v = (rect[1] + rect[3]) << 4;
    } else {
        packet->vertex[1].u = rect[0] << 4;
        packet->vertex[1].v = rect[1] << 4;
        packet->vertex[0].u = (rect[0] + rect[2]) << 4;
        packet->vertex[0].v = rect[1] << 4;
        packet->vertex[3].u = rect[0] << 4;
        packet->vertex[3].v = (rect[1] + rect[3]) << 4;
        packet->vertex[2].u = (rect[0] + rect[2]) << 4;
        packet->vertex[2].v = (rect[1] + rect[3]) << 4;
    }
}
