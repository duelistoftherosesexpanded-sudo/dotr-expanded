#include "dotr/ee/cursor_packets.h"

/* Draws the frame-buffer copy as one textured rectangle: header (5, 38, 0, 0x42421), colour
 * (128, 128, 128, 128), 1.0, the vertices (27640, 29176) with texel (0, 0) and (0x93f8, 0x8df8)
 * with texel (0.625, 0.875), each with the 24-bit value 0xffffff. The cursor passes through
 * RhText_SetTextureFlush, ExFrameBuffer2Texture_Pipe(cursor, -1, 0, two local words),
 * RhText_SetTextureFlush, RhBase_MaskFBuffer(cursor, 0xff000000), RhBase_SetZBufferMode(cursor, 0),
 * ExSortPrim_Pipe with the packet, RhBase_SetZBufferMode(cursor, 1) and RhBase_MaskFBuffer(cursor, 0),
 * whose result is returned. */
DotrDrawWord lSzModel_Draw_DefFunc(DotrDrawWord cursor)
{
    struct DotrModelShotPacket packet;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    packet.colour[0] = 128;
    packet.colour[1] = 128;
    packet.colour[2] = 128;
    packet.colour[3] = 128;
    packet.unknown_14 = 1.0f;
    packet.vertex[0].xyz.x = 27640;
    packet.vertex[0].xyz.y = 29176;
    packet.vertex[0].xyz.z = 0xffffff;
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.x = 0x93f8;
    packet.vertex[1].xyz.y = 0x8df8;
    packet.vertex[1].xyz.z = 0xffffff;
    packet.vertex[1].xyz.f = 0;
    packet.vertex[0].u = 0.0f;
    packet.vertex[0].v = 0.0f;
    packet.vertex[1].u = 0.625f;
    packet.vertex[1].v = 0.875f;
    cursor = RhText_SetTextureFlush(cursor);
    cursor = ExFrameBuffer2Texture_Pipe(cursor, -1, 0, &packet.tex0, &packet.tex1);
    cursor = RhText_SetTextureFlush(cursor);
    cursor = RhBase_MaskFBuffer(cursor, 0xff000000);
    cursor = RhBase_SetZBufferMode(cursor, 0);
    /* MATCHING DEVICE: the header address spelled from the member before it. */
    cursor = ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
    cursor = RhBase_SetZBufferMode(cursor, 1);
    return RhBase_MaskFBuffer(cursor, 0);
}
