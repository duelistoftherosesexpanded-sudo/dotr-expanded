#include "dotr/ee/title_draws.h"

/* lSzDuel_DrawWindow_FieldPiece_Pipe: sorts a flat 92 x 62 rectangle at (x + 1730, y + 1826)
 * in the colour of row `colour` of gSzDuel_aaicolRgba (bytes 6, 5, 4 of the row, alpha 128)
 * at the depth word 0x3180e8. When model field + 74 is displayed and RhView_PushStack
 * succeeds, renders that model into render frame (0, 10, 49): the view direction is
 * flattened onto its z axis and normalised, the eye is put at -direction * f lowered by 25,
 * the model is drawn with its third intensity forced to 1.0 and the rotation words copied
 * from 0x2912d0 with the second replaced by the word at 0x31ab80 (nRotY in the Japanese
 * build's symbols), which then advances by 512. The result is sorted as a textured 90 x 61
 * rectangle at (x + 1731, y + 1827). Returns the advanced cursor. */
DotrDrawWord lSzDuel_DrawWindow_FieldPiece_Pipe(DotrDrawWord cursor, int colour, int field, float f, int x, int y)
{
    unsigned char *control = data_00317490_bytes;
    struct DotrTitleViewBlock view;
    struct DotrTitleRotation rotation;
    struct DotrTitleFlatPacket flat;
    struct DotrCursorSlotRectPacket packet;
    struct DotrTitleIntensity saved;
    struct DotrTitleIntensity bright;
    int px = x + 1728;
    int py = y + 1824;
    int w = 96;
    int h = 64;
    unsigned char *rgba = gSzDuel_aaicolRgba[colour];

    ((struct DotrCursorPacketHeader *)flat.header)->unknown_02 = 6;
    ((struct DotrCursorPacketHeader *)flat.header)->unknown_00 = 3;
    ((struct DotrCursorPacketHeader *)flat.header)->unknown_08 = 0x441;
    ((struct DotrCursorPacketHeader *)flat.header)->unknown_04 = 0;
    flat.colour[0] = rgba[6];
    flat.colour[1] = rgba[5];
    flat.colour[2] = rgba[4];
    flat.colour[3] = 128;
    flat.unknown_14 = 1.0f;
    flat.xyz[0].x = (px + 2) << 4;
    flat.xyz[0].y = (py + 2) << 4;
    flat.xyz[0].z = data_003180e8;
    flat.xyz[0].f = 0;
    flat.xyz[1].x = (px + w - 2) << 4;
    flat.xyz[1].y = (py + h) << 4;
    flat.xyz[1].z = data_003180e8;
    flat.xyz[1].f = 0;
    cursor = ExSortPrim_Pipe(cursor, &flat.unknown_m8 + 1, 0, 0);
    if (!SzModel_IsModelDisp(field + 74)) {
        return cursor;
    }
    px += 3;
    py += 3;
    w -= 6;
    h -= 3;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
    ExAssignAlpha((struct DotrCursorPacketHeader *)packet.header, 0x8000000044);
    packet.colour[0] = 255;
    packet.colour[1] = 255;
    packet.colour[2] = 255;
    packet.colour[3] = 128;
    packet.unknown_14 = 1.0f;
    packet.vertex[0].xyz.z = *(int *)(control + 3160);
    packet.vertex[0].xyz.f = 0;
    packet.vertex[1].xyz.z = *(int *)(control + 3160);
    packet.vertex[1].xyz.f = 0;
    RhView_GetCurView((struct DotrViewBlock *)&view);
    if (RhView_PushStack()) {
        rotation = data_002912d0;
        rotation.word[1] = RotY;
        RhBase_AssignRenderFrame(0, 10, 49);
        cursor = RhBase_EnableRenderFrame(cursor);
        cursor = RhBase_ClearRenderFrame(cursor, 0);
        view.u.values[22] = func_001a6c60(view.u.values[20] * view.u.values[20] + view.u.values[22] * view.u.values[22]);
        view.u.values[20] = 0.0f;
        RhGeom_NormalizeVector((struct DotrViewVector *)&view.u.values[20]);
        view.u.values[16] = -(view.u.values[20] * f);
        view.u.values[17] = -(view.u.values[21] * f) - 25.0f;
        view.u.values[18] = -(view.u.values[22] * f);
        view.u.values[32] = 0.0f;
        RhView_Modify((struct DotrViewBlock *)&view);
        cursor = RhBase_SetZBufferMode((DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)cursor), 1);
        SzModel_GetIntensity(field + 74, &saved);
        bright = saved;
        bright.z = 1.0f;
        SzModel_SetIntensity(field + 74, &bright);
        cursor = SzModel_DrawWithOutCalc_Pipe(cursor, field + 74, 0, &rotation, 0, 0, 0);
        SzModel_SetIntensity(field + 74, &saved);
        cursor = RhBase_SetZBufferMode(cursor, 0);
        RhBase_GetTextureInfoForRender(&packet.tex0, &packet.tex1);
        cursor = RhText_SetTextureFlush(RhBase_DisableRenderFrame(cursor));
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = 0;
        ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = 0;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = 10240;
        ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = 7168;
        packet.vertex[0].xyz.x = (px << 4) - 8;
        packet.vertex[0].xyz.y = (py << 4) - 8;
        packet.vertex[1].xyz.x = ((px + w) << 4) - 8;
        packet.vertex[1].xyz.y = ((py + h) << 4) - 8;
        RhView_PopStack();
        RhView_GetCurView((struct DotrViewBlock *)&view);
        RhView_Modify((struct DotrViewBlock *)&view);
        cursor = (DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)cursor);
        cursor = ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
        RhBase_FreeRenderFrame();
        RotY += 512;
    }
    return cursor;
}
