#include "dotr/ee/camp_model_disp.h"

DotrDrawWord Taku_Camp_Teki_Model_Disp(DotrDrawWord cursor, float dist, int modelId, int x, int y, int w, int h, int alpha)
{
    struct DotrCampModelView view;
    struct DotrCampModelVector centre;
    struct DotrCursorSlotRectPacket packet;
    int half;

    /* MATCHING DEVICES: the header address is spelled from the leading member for the sort call
     * only, as in SzMsg_DrawRect_Pipe, and the two offsets are applied before anything else. */

    x -= 320;
    y -= 224;
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
    packet.vertex[0].xyz.z = 0xfffffa;
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.z = 0xfffffa;
    packet.vertex[1].xyz.f = 0;
    func_0020def0_campmodel(&view);
    if (func_0020e460_campmodel()) {
        RhBase_AssignRenderFrame(0, 10, 49);
        cursor = RhBase_EnableRenderFrame(cursor);
        cursor = RhBase_ClearRenderFrame(cursor, 0);
        func_001f2050_campmodel(modelId, &centre);
        view.eye[0] = 0.0f;
        view.eye[1] = centre.y;
        view.eye[2] = -dist;
        view.dir[0] = 0.0f;
        view.dir[1] = 0.0f;
        view.dir[2] = 1.0f;
        view.roll = 0.0f;
        func_0020dfd0_campmodel(1.0f, 512.0f);
        func_0020e360_campmodel(&view);
        cursor = (DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)cursor);
        func_001f30b0_campmodel(modelId, 1);
        cursor = func_001ee4b0_campmodel(cursor, modelId, 0, 0, 0, 0, 0);
        func_001ed4a0_campmodel(modelId);
        func_001f30b0_campmodel(modelId, 0);
        RhBase_GetTextureInfoForRender(&packet.tex0, &packet.tex1);
        cursor = RhText_SetTextureFlush(RhBase_DisableRenderFrame(cursor));
        half = w * 448 / h >> 1;
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = (320 - half) << 4;
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = 0;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = (half + 320) << 4;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = 7168;
        packet.vertex[0].xyz.x = (x + 2048) << 4;
        packet.vertex[0].xyz.y = (y + 2048) << 4;
        packet.vertex[1].xyz.x = (x + 2048 + w) << 4;
        packet.vertex[1].xyz.y = (y + 2048 + h) << 4;
        func_0020e4f0_campmodel();
        func_0020def0_campmodel(&view);
        func_0020e360_campmodel(&view);
        cursor = (DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)cursor);
        cursor = ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
        RhBase_FreeRenderFrame();
    }
    return cursor;
}
