#include "dotr/ee/cursor_packets.h"

/* Draws the frame-buffer rectangle of lSzModel_Draw_DefFunc with a caller colour: header
 * (5, 38, 0, 0x42421), colour bytes 2, 1, 0, 3 of `colour` (of the 0x293ed0 default when it is
 * null), 1.0, the vertices (27640, 29176) with texel (0, 0) and (0x93f8, 0x8df8) with texel
 * (0.625, 0.875), each with the 24-bit value 0xfffff7. The header then goes through
 * ExAssignAlpha(alpha) and ExAssignFrameBuffer(buffer); returns ExSortPrim_Pipe(cursor,
 * packet, a1, a2). */
DotrDrawWord ExSortDispBlur_Pipe(DotrDrawWord cursor, int a1, int a2, int buffer, long alpha,
                                 unsigned char *colour)
{
    struct DotrBlurPacket packet;
    struct DotrBlurColour normal = data_00293ed0;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    if (colour == 0) {
        colour = normal.c;
    }
    packet.colour[0] = colour[2];
    packet.colour[1] = colour[1];
    packet.colour[2] = colour[0];
    packet.colour[3] = colour[3];
    packet.unknown_14 = 1.0f;
    packet.vertex[0].xyz.x = 27640;
    packet.vertex[0].xyz.y = 29176;
    packet.vertex[0].xyz.z = 0xfffff7;
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.x = 0x93f8;
    packet.vertex[1].xyz.y = 0x8df8;
    packet.vertex[1].xyz.z = 0xfffff7;
    packet.vertex[1].xyz.f = 0;
    packet.vertex[0].u = 0.0f;
    packet.vertex[0].v = 0.0f;
    packet.vertex[1].u = 0.625f;
    packet.vertex[1].v = 0.875f;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&packet, alpha);
    ExAssignFrameBuffer((unsigned short *)&packet, buffer);
    return ExSortPrim_Pipe(cursor, &packet, a1, a2);
}
