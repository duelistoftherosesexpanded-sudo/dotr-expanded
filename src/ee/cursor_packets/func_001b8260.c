#include "dotr/ee/cursor_packets.h"

/* Builds a two-vertex packet for the rectangle (x + 1728, y + 1824) to that plus (width,
 * height), both scaled by 16, with colour (255, 255, 255, alpha), 1.0 and the 24-bit value
 * 0xfffffd in each vertex; the header (3, 6, 0, 1089) goes through ExAssignAlpha with the
 * value of ExGetAlphaVal(2). The cursor passes through RhBase_SetZBufferMode(cursor, 0),
 * ExSortPrim_Pipe with the packet and RhBase_SetZBufferMode(cursor, 1), whose result is returned. */
DotrDrawWord lSzDuel_Draw_Fade_Pipe(DotrDrawWord cursor, int x, int y, int width, int height, int alpha)
{
    struct DotrCursorRectPacket packet;
    int left = x + 1728;
    int top = y + 1824;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 6;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 3;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 1089;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&packet, ExGetAlphaVal(2));
    packet.colour[0] = 255;
    packet.colour[1] = 255;
    packet.colour[2] = 255;
    packet.colour[3] = alpha;
    packet.unknown_14 = 1.0f;
    packet.vertex[0].unknown_x = left << 4;
    packet.vertex[0].unknown_y = top << 4;
    packet.vertex[0].unknown_z = 0xfffffd;
    packet.vertex[0].unknown_f = 0;
    packet.vertex[1].unknown_x = (left + width) << 4;
    packet.vertex[1].unknown_y = (top + height) << 4;
    packet.vertex[1].unknown_z = 0xfffffd;
    packet.vertex[1].unknown_f = 0;
    cursor = RhBase_SetZBufferMode(cursor, 0);
    cursor = ExSortPrim_Pipe(cursor, &packet, 0, 0);
    return RhBase_SetZBufferMode(cursor, 1);
}
