#include "dotr/ee/cursor_packets.h"

/* Builds the two-vertex packet for the fixed rectangle (1728, 1824) to (1728 + 640,
 * 1824 + 448), scaled by 16, with the colour bytes 2, 1, 0, 3 of `colour`, 1.0 and the low
 * 24 bits of z in each vertex; the header (3, 6, 0, 1089) goes through ExAssignAlpha with
 * the value of ExGetAlphaVal(index). Returns ExSortPrim_Pipe(cursor, packet, 0, 0). */
DotrDrawWord SzFade_Draw_Pipe(DotrDrawWord cursor, int index, unsigned char *colour, unsigned int z)
{
    struct DotrCursorRectPacket packet;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 6;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 3;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 1089;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&packet, ExGetAlphaVal(index));
    packet.colour[0] = colour[2];
    packet.colour[1] = colour[1];
    packet.colour[2] = colour[0];
    packet.colour[3] = colour[3];
    packet.unknown_14 = 1.0f;
    packet.vertex[0].unknown_x = 1728 << 4;
    packet.vertex[0].unknown_y = 1824 << 4;
    packet.vertex[0].unknown_z = z;
    packet.vertex[0].unknown_f = 0;
    packet.vertex[1].unknown_x = (1728 + 640) << 4;
    packet.vertex[1].unknown_y = (1824 + 448) << 4;
    packet.vertex[1].unknown_z = z;
    packet.vertex[1].unknown_f = 0;
    return ExSortPrim_Pipe(cursor, &packet, 0, 0);
}
