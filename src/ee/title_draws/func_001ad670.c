#include "dotr/ee/title_draws.h"

/* lSzGame_DrawTitle_Rose_Pipe: prepares a textured two-vertex packet (header 5, 38 | 0x800,
 * 0, 0x42421, alpha value 0x8000000044, colour 128 with the byte at 0x317476, 1.0, depth
 * 0xfffff8). When RhView_PushStack succeeds, renders model `index` into render frame
 * (0, 2, 50) with the current view moved to (0, 0, -z) looking along (0, 0, 1) and clip
 * range 1..512, then restores the view and sorts the packet as a width x height rectangle
 * centred on (x + 2048, y + 2048) with texel corners (0, 0) and (640, 448) in 12.4 fixed
 * point. Returns the advanced cursor, or the cursor unchanged when the push fails. */
DotrDrawWord lSzGame_DrawTitle_Rose_Pipe(DotrDrawWord cursor, int index, int x, int y, int width, int height, float z)
{
    struct DotrTitleViewBlock view;
    struct DotrCursorSlotRectPacket packet;

    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)packet.header, 0x8000000044);
    packet.colour[0] = 128;
    packet.colour[1] = 128;
    packet.colour[2] = 128;
    packet.colour[3] = data_00317476;
    packet.unknown_14 = 1.0f;
    packet.vertex[0].xyz.z = 0xfffff8;
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.z = 0xfffff8;
    packet.vertex[1].xyz.f = 0;
    RhView_GetCurView((struct DotrViewBlock *)&view);
    if (RhView_PushStack()) {
        RhBase_AssignRenderFrame(0, 2, 50);
        cursor = RhBase_EnableRenderFrame(cursor);
        cursor = RhBase_ClearRenderFrame(cursor, 0);
        view.u.values[16] = 0.0f;
        view.u.values[17] = 0.0f;
        view.u.values[18] = -z;
        view.u.values[20] = 0.0f;
        view.u.values[21] = 0.0f;
        view.u.values[22] = 1.0f;
        view.u.values[32] = 0.0f;
        RhView_SetClipZ(1.0f, 512.0f);
        RhView_Modify((struct DotrViewBlock *)&view);
        cursor = (DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)cursor);
        cursor = SzModel_DrawWithOutCalc_Pipe(cursor, index, 0, 0, 0, 0, 0);
        RhBase_GetTextureInfoForRender(&packet.tex0, &packet.tex1);
        cursor = RhText_SetTextureFlush(RhBase_DisableRenderFrame(cursor));
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = 0;
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = 0;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = 10240;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = 7168;
        packet.vertex[0].xyz.x = ((x + 2048 - (width >> 1)) << 4) - 8;
        packet.vertex[0].xyz.y = ((y + 2048 - (height >> 1)) << 4) - 8;
        packet.vertex[1].xyz.x = ((x + 2048 + (width >> 1)) << 4) - 8;
        packet.vertex[1].xyz.y = ((y + 2048 + (height >> 1)) << 4) - 8;
        RhView_PopStack();
        RhView_GetCurView((struct DotrViewBlock *)&view);
        RhView_Modify((struct DotrViewBlock *)&view);
        cursor = (DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)cursor);
        /* MATCHING DEVICE: header address spelled from the leading member, as in SzMsg_DrawRect_Pipe. */
        cursor = ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
        RhBase_FreeRenderFrame();
    }
    return cursor;
}
