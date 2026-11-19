#include "dotr/ee/scaled_sprite.h"

/* lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe: draws the summon-charge window of the custom
 * duel menu for `side` at (x, y): the title message 129 + the low four bits of the side's byte
 * at 0xa9c002, narrowed to 154 pixels, in a 178 x 32 frame centred on the window (580 wide when
 * the byte at 0xa9c088 is set, else 276); the window frame; icon 207, a 60 x 20 frame, icon 215,
 * the side's byte at 0xa9c006 in digits and icon 208 around the centre; then the board strip,
 * icons 252..254 and the captions at 0x2bf05b / 0x2bef93. Returns the advanced cursor.
 * The side is a byte pointer (see DOTR_SELDECK_BYTE). */
DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(DotrSpriteWord arg0, int arg1, int x, int y)
{
    char *text3 = data_002bf05b;
    char *text4 = data_002bef93;
    int color = (DOTR_SELDECK_BYTE(arg1, 0xA9C002) & 0xF) + 0x81;
    int width;
    float scale;
    int hint = 0;
    int cx;
    int w;
    DotrSpriteWord id;
    int px;
    int py;

    if (DOTR_SELDECK_FLAG != 0) {
        width = 0x244;
    } else {
        width = 0x114;
    }
    scale = 1.0f;
    id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 1, color, 0, 0x16));
    cx = x + (width >> 1);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, 3, 4, cx - 0x59, y, -0xB2, -0x20, 1);
    w = SzMsg_GetInfo(1)->unknown_0c;
    if (w > 0x9A) {
        scale = 154.0f / (float)w;
    }
    id = SzMsg_DrawRatioX_Pipe(id, 1, cx - (int)((float)(w >> 1) * scale), y + 5, 0, SzMsg_DrawRect_Pipe, scale, 1.0f);
    SzMsg_Free(1);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, arg1, -1, 0xB, x, y + 0x20, -width, -0x96, 1);
    px = x + 0x688 + (width >> 1);
    py = y + 0x764;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290896, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, -1, 5, cx - 0x1E, y + 0x46, -0x3C, -0x14, 1);
    px = x + 0x6A4 + (width >> 1);
    py = y + 0x768;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_002908e6, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    id = SzDuel_DrawWindow_FontDigit_Pipe(id, cx - 6, y + 0x4A, 0, 0, 2, DOTR_SELDECK_BYTE(arg1, 0xA9C006));
    px = x + 0x6E0 + (width >> 1);
    py = y + 0x764;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_002908a0, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    id = lSzDuel_DrawWindow_ExBoard_Pipe(id, arg1, x + 0x12, y + 0x7A, -(width - 0x24), -0x34, -1, 0);
    px = x + 0x6C2 + ((width - 0x24) >> 2);
    py = y + 0x7A1;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a58, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x6D2 + ((width - 0x24) >> 2);
    py = y + 0x7A1;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a62, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x12 + ((width - 0x24) >> 2);
    py = y + 0x9C;
    id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 2, func_0019bef0((char *)text3), text3,
                                     hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    px = x + 0x6CA + (((width - 0x24) * 2) >> 2);
    py = y + 0x7A1;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a6c, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x12 + (((width - 0x24) * 2) >> 2);
    return SzDuel_DrawWindow_Font_Pipe(id, px, y + 0x9C, 1, 2, func_0019bef0((char *)text4), text4,
                                       hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
}
