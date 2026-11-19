#include "dotr/ee/cursor_packets.h"

/* Draws the frame buffer back over the screen as a soft-focus rectangle: header (5, 38, 0,
 * 0x42421) with alpha 0x8000000044, colour bytes 2, 1, 0, 3 of `colour` (of the 0x293ed8
 * default when it is null), 1.0, the vertices (x + 27640, y + 29176) with texel (0, 0) and
 * (x + 0x93f8, y + 0x8df8) with texel (0.625, 0.875), each with the 24-bit value 0xffffff.
 * Between two texture flushes the frame buffer is turned into a texture (GS address, a3, the
 * two words at the end of the packet); bit 34 of the first word is then set. Returns
 * ExSortPrim_Pipe(cursor, packet, a1, a2). MATCHING DEVICE: the packet address is passed as
 * `&packet`, not `packet.header`. */
DotrDrawWord ExSortSoftFocus_Pipe(DotrDrawWord cursor, int a1, int a2, int a3, int x, int y, unsigned char *colour)
{
    struct DotrSoftFocusPacket packet;
    struct DotrBlurColour normal = data_00293ed8;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&packet, 0x8000000044);
    if (colour == 0) {
        colour = normal.c;
    }
    packet.colour[0] = colour[2];
    packet.colour[1] = colour[1];
    packet.colour[2] = colour[0];
    packet.colour[3] = colour[3];
    packet.unknown_14 = 1.0f;
    packet.vertex[0].xyz.x = x + 27640;
    packet.vertex[0].xyz.y = y + 29176;
    packet.vertex[0].xyz.z = 0xffffff;
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.x = x + 0x93f8;
    packet.vertex[1].xyz.y = y + 0x8df8;
    packet.vertex[1].xyz.z = 0xffffff;
    packet.vertex[1].xyz.f = 0;
    packet.vertex[0].u = 0.0f;
    packet.vertex[0].v = 0.0f;
    packet.vertex[1].u = 0.625f;
    packet.vertex[1].v = 0.875f;
    cursor = RhText_SetTextureFlush(cursor);
    cursor = func_00208870_soft(cursor, RhBase_GetUserGsAddress(), a3, &packet.tex0, &packet.tex1);
    cursor = RhText_SetTextureFlush(cursor);
    packet.tex0.flag = 1;
    return ExSortPrim_Pipe(cursor, &packet, a1, a2);
}
