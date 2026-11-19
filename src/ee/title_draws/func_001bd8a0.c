#include "dotr/ee/title_draws.h"

/* lSzDuel_DrawBuildDeck_GhostCard_Pipe: prepares a textured two-vertex packet (header 5,
 * 38 | 0x800, 0, 0x42421, alpha value 0x8000000044, colour 128, 128, 128, alpha, 1.0); renders
 * the build-deck card `id` at (0, 0) into the render frame (enable, clear, draw, texture info
 * into the packet's two words, disable, flush), then sorts the packet as a 304 x 54 rectangle
 * at (x + 1728, y + 1824) with texel corners (0, 0) and (304, 54) in 12.4 fixed point, both
 * vertices at the depth word 0x3180e8. Returns the advanced cursor. */
DotrDrawWord lSzDuel_DrawBuildDeck_GhostCard_Pipe(DotrDrawWord cursor, int id, int x, int y, int flag, int alpha)
{
    struct DotrCursorSlotRectPacket packet;
    int px;
    int py;
    /* MATCHING DEVICE: the rectangle size held in initialised locals. */
    int w = 304;
    int h = 54;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)packet.header, 0x8000000044);
    packet.colour[0] = 128;
    packet.colour[1] = 128;
    packet.colour[2] = 128;
    packet.colour[3] = alpha;
    packet.unknown_14 = 1.0f;
    cursor = RhBase_EnableRenderFrame(cursor);
    cursor = RhBase_ClearRenderFrame(cursor, 0);
    cursor = lSzDuel_DrawBuildDeck_Card_Pipe(cursor, id, 0, 0, flag, 0);
    RhBase_GetTextureInfoForRender(&packet.tex0, &packet.tex1);
    cursor = RhText_SetTextureFlush(RhBase_DisableRenderFrame(cursor));
    px = x + 1728;
    py = y + 1824;
    ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = 0;
    ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = 0;
    ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = 4864;
    ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = 864;
    packet.vertex[0].xyz.x = (px << 4) - 8;
    packet.vertex[0].xyz.y = (py << 4) - 8;
    packet.vertex[0].xyz.z = data_003180e8;
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.x = ((px + w) << 4) - 8;
    packet.vertex[1].xyz.y = ((py + h) << 4) - 8;
    packet.vertex[1].xyz.z = data_003180e8;
    packet.vertex[1].xyz.f = 0;
    /* MATCHING DEVICE: header address spelled from the leading member, as in SzMsg_DrawRect_Pipe. */
    return ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
}
