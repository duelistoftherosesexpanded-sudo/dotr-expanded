#include "dotr/ee/cursor_packets.h"

/* Draws the item picture `id` of text group 1 at (x + 1728, y + 1824) with the given width
 * and height. Returns the cursor unchanged when SzText_GetReadyItem gives no entry or the
 * entry's texture does not start with the word 0x324852. Otherwise the same sequence as
 * lSzDuel_DrawWindow_PicMini_Pipe: one CLUT slot, texture load, flush, and a two-vertex packet
 * with colour 128 in all four bytes, 1.0, texel corners (8, 8) and (2056, 2056), positions
 * scaled by 16 and z in the 24-bit field. */
DotrDrawWord lSzDuel_DrawWindow_PicItem_Pipe(DotrDrawWord cursor, int id, int x, int y, int width, int height, unsigned int z)
{
    struct DotrCursorSlotRectPacket packet;
    struct DotrCursorPicControl *control = &data_00317490;
    int left = x + 1728;
    int top = y + 1824;
    struct DotrCursorPicEntry *entry;
    int clut;

    entry = SzText_GetReadyItem(1, id);
    if (entry == 0) {
        return cursor;
    }
    if (*entry->texture == 0x324852) {
        clut = control->clut;
        clut += SzText_InitClutManage(clut, 1);
        cursor = SzText_Load_Pipe(cursor, entry->texture, clut, 0);
        cursor = RhText_SetTextureFlush(cursor);
        ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
        ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
        ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
        ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
        ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
        ExAssignTexture((struct DotrCursorPacketHeader *)packet.header, entry->texture, 0);
        packet.colour[0] = 128;
        packet.colour[1] = 128;
        packet.colour[2] = 128;
        packet.colour[3] = 128;
        packet.unknown_14 = 1.0f;
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = 8;
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = 8;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = 2056;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = 2056;
        packet.vertex[0].xyz.x = left << 4;
        packet.vertex[0].xyz.y = top << 4;
        packet.vertex[0].xyz.z = z;
        packet.vertex[0].xyz.f = 0;
        packet.vertex[1].xyz.x = (left + width) << 4;
        packet.vertex[1].xyz.y = (top + height) << 4;
        packet.vertex[1].xyz.z = z;
        packet.vertex[1].xyz.f = 0;
        /* MATCHING DEVICE: header address spelled from the leading member, as in SzMsg_DrawRect_Pipe. */
        cursor = ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
    }
    return cursor;
}
