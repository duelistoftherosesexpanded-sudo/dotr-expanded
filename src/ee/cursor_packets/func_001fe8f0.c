#include "dotr/ee/cursor_packets.h"

/* Builds the two-vertex packet for the rectangle (rect x + x, rect y + y) to that plus the
 * rect's width and height, scaled by 16, with the colour bytes 2, 1, 0, 3 of `colour`, 1.0
 * and the low 24 bits of the word at +628 of gSzMsg_ml in each vertex. A null rect
 * selects gSzMsg_defRect and a null colour icolDef_266. The header (3, 6, 0, 1089) goes
 * through ExAssignAlpha with the constant 0x8000000044. Returns ExSortPrim_Pipe(cursor,
 * packet, 0, 0). */
DotrDrawWord SzMsg_DrawGlass_Pipe(DotrDrawWord cursor, int x, int y, struct DotrSlotTableWords *rect,
                           unsigned char *colour)
{
    struct DotrCursorRectPacket packet;
    int *state = (int *)gSzMsg_ml;
    int left, top, right, bottom;

    if (rect == 0) {
        rect = gSzMsg_defRect;
    }
    if (colour == 0) {
        colour = icolDef_266;
    }
    left = rect->words[0] + x;
    top = rect->words[1] + y;
    right = rect->words[2] + left;
    bottom = rect->words[3] + top;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 6;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 3;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 1089;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ExAssignAlpha((struct DotrCursorPacketHeader *)&packet, 0x8000000044L);
    packet.colour[0] = colour[2];
    packet.colour[1] = colour[1];
    packet.colour[2] = colour[0];
    packet.colour[3] = colour[3];
    packet.unknown_14 = 1.0f;
    packet.vertex[0].unknown_x = left << 4;
    packet.vertex[0].unknown_y = top << 4;
    packet.vertex[0].unknown_z = state[157];
    packet.vertex[0].unknown_f = 0;
    packet.vertex[1].unknown_x = right << 4;
    packet.vertex[1].unknown_y = bottom << 4;
    packet.vertex[1].unknown_z = state[157];
    packet.vertex[1].unknown_f = 0;
    return ExSortPrim_Pipe(cursor, &packet, 0, 0);
}
