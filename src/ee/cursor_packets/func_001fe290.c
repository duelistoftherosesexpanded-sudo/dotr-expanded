#include "dotr/ee/cursor_packets.h"

/* Draws the message slot `index` as a textured rectangle with one colour per corner.
 * Returns the cursor unchanged when SzMsg_GetInfo gives no entry or the entry's flag bit is
 * clear. Otherwise as SzMsg_DrawRect_Pipe, with four vertices in the order top-left,
 * top-right, bottom-left, bottom-right: colour bytes 2, 1, 0, 3 of each four-byte group of
 * `colour` (a null colour selects icolDef_226, a null rect gSzMsg_defRect), 1.0, the
 * entry's texel corners as (n << 4) + 8 and the low 24 bits of the word at +628 of
 * gSzMsg_ml. The header (12, 52 | 0x800, 0, 0x421421421421) goes through ExAssignAlpha
 * with 0x8000000044; the result is ExSortPrim_Pipe(cursor, packet, 0, 0). */
DotrDrawWord SzMsg_DrawRectG_Pipe(DotrDrawWord cursor, int index, int x, int y, struct DotrSlotTableWords *rect,
                                  unsigned char *colour)
{
    struct DotrCursorRectGPacket packet;
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
        colour = icolDef_226;
    }
    left = rect->words[0] + x;
    top = rect->words[1] + y;
    right = rect->words[2] + left;
    bottom = rect->words[3] + top;
    v = entry->unknown_004[1];
    u = entry->unknown_004[0];
    u2 = entry->unknown_004[2] + u;
    v2 = entry->unknown_004[3] + v;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 52;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 12;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x421421421421L;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)packet.header, 0x8000000044L);
    packet.vertex[0].colour[0] = colour[2];
    packet.vertex[0].colour[1] = colour[1];
    packet.vertex[0].colour[2] = colour[0];
    packet.vertex[0].colour[3] = colour[3];
    packet.vertex[0].unknown_04 = 1.0f;
    packet.vertex[1].colour[0] = colour[6];
    packet.vertex[1].colour[1] = colour[5];
    packet.vertex[1].colour[2] = colour[4];
    packet.vertex[1].colour[3] = colour[7];
    packet.vertex[1].unknown_04 = 1.0f;
    packet.vertex[2].colour[0] = colour[10];
    packet.vertex[2].colour[1] = colour[9];
    packet.vertex[2].colour[2] = colour[8];
    packet.vertex[2].colour[3] = colour[11];
    packet.vertex[2].unknown_04 = 1.0f;
    packet.vertex[3].colour[0] = colour[14];
    packet.vertex[3].colour[1] = colour[13];
    packet.vertex[3].colour[2] = colour[12];
    packet.vertex[3].colour[3] = colour[15];
    packet.vertex[3].unknown_04 = 1.0f;
    packet.vertex[0].xyz.x = left << 4;
    packet.vertex[0].xyz.y = top << 4;
    packet.vertex[0].xyz.z = state[157];
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.x = right << 4;
    packet.vertex[1].xyz.y = top << 4;
    packet.vertex[1].xyz.z = state[157];
    packet.vertex[1].xyz.f = 0;
    packet.vertex[2].xyz.x = left << 4;
    packet.vertex[2].xyz.y = bottom << 4;
    packet.vertex[2].xyz.z = state[157];
    packet.vertex[2].xyz.f = 0;
    packet.vertex[3].xyz.x = right << 4;
    packet.vertex[3].xyz.y = bottom << 4;
    packet.vertex[3].xyz.z = state[157];
    packet.vertex[3].xyz.f = 0;
    ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = (u << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = (v << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = (u2 << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = (v << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[2].uv)->u = (u << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[2].uv)->v = (v2 << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[3].uv)->u = (u2 << 4) + 8;
    ((struct DotrCursorSlotUv *)&packet.vertex[3].uv)->v = (v2 << 4) + 8;
    lSzMsg_GetTexInfo(&packet.tex0, &packet.tex1);
    return ExSortPrim_Pipe(cursor, &packet, 0, 0);
}
