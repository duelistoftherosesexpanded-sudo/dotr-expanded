#include "dotr/ee/cursor_packets.h"

/* Draws the message slot `index` as a textured rectangle. Returns the cursor unchanged when
 * SzMsg_GetInfo gives no entry or the entry's flag bit is clear. Otherwise the rectangle is
 * (rect x + x, rect y + y) to that plus the rect's width and height (a null rect selects
 * gSzMsg_defRect, a null colour iColDef_210), the texel corners are the entry's four words
 * (u, v, width, height) as (n << 4) + 8 in 14-bit fields, each vertex carries the low 24
 * bits of the word at +628 of gSzMsg_ml, and the header (5, 38 | 0x800, 0, 0x42421) goes
 * through ExAssignAlpha with 0x8000000044. lSzMsg_GetTexInfo fills the two words after the
 * vertices; the result is ExSortPrim_Pipe(cursor, header, 0, 0).
 * The local begins 8 bytes before the header (the original passes sp+200 from an object
 * that the frame shows starting at sp+192); header and texel words are reached through
 * casts, which is what makes the compiler address them through a temporary pointer. */
DotrDrawWord SzMsg_DrawRect_Pipe(DotrDrawWord cursor, int index, int x, int y, struct DotrSlotTableWords *rect,
                                 unsigned char *colour)
{
    struct DotrCursorSlotRectPacket packet;
    int *state = (int *)gSzMsg_ml;
    struct DotrCursorSlotEntry *entry;
    int left, top, right, bottom;
    int u, v, u2, v2;

    entry = SzMsg_GetInfo(index);
    if (entry == 0) {
        return cursor;
    }
    if (!entry->flag) {
        return cursor;
    }
    if (rect == 0) {
        rect = gSzMsg_defRect;
    }
    if (colour == 0) {
        colour = iColDef_210;
    }
    left = rect->words[0] + x;
    top = rect->words[1] + y;
    right = rect->words[2] + left;
    bottom = rect->words[3] + top;
    v = entry->unknown_004[1];
    u = entry->unknown_004[0];
    u2 = entry->unknown_004[2] + u;
    v2 = entry->unknown_004[3] + v;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)packet.header, 0x8000000044L);
    packet.colour[0] = colour[2];
    packet.colour[1] = colour[1];
    packet.colour[2] = colour[0];
    packet.colour[3] = colour[3];
    packet.unknown_14 = 1.0f;
    packet.vertex[0].xyz.x = left << 4;
    packet.vertex[0].xyz.y = top << 4;
    packet.vertex[0].xyz.z = state[157];
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.x = right << 4;
    packet.vertex[1].xyz.y = bottom << 4;
    packet.vertex[1].xyz.z = state[157];
    packet.vertex[1].xyz.f = 0;
    ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = (u << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = (v << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = (u2 << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = (v2 << 4) + 8;
    lSzMsg_GetTexInfo(&packet.tex0, &packet.tex1);
    /* MATCHING DEVICE: the header address is spelled from the leading member so that it is
     * formed again here instead of being kept in a saved register. */
    return ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
}
