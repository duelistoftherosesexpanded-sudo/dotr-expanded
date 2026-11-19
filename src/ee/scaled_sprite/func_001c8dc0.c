#include "dotr/ee/scaled_sprite.h"

/* lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe: draws the deck selection window of the custom
 * duel menu for `side` at (x, y): the title message 129 + the low four bits of the side's byte
 * at 0xa9c002, narrowed to 154 pixels, in a 178 x 32 frame centred on the window, whose width
 * is 580 when the byte at 0xa9c088 is set and 276 otherwise; three deck columns, each with its
 * icon, the leader picture 10 + i (20 + i for side 1) and a frame that is highlighted when the
 * side's byte at 0xa9c004 equals the column and marked when the deck does not have 40 cards or
 * its leader is 999; then the board strip, three icons and the two captions at 0x2bf05b and
 * 0x2bef93. Returns the advanced cursor.
 * The side is a byte pointer (see DOTR_SELDECK_BYTE), as in the inline helper. */

static inline unsigned char *Save_SelectDeck(int save)
{
    return DOTR_SELDECK_PTR(save, 0xA9C000);
}

DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(DotrSpriteWord arg0, int arg1, int x, int y)
{
    unsigned char *state = data_00317490_bytes;
    char *text3 = data_002bf05b;
    char *text4 = data_002bef93;
    int color = (DOTR_SELDECK_BYTE(arg1, 0xA9C002) & 0xF) + 0x81;
    int width = 0x244;
    float scale;
    int hint = 0;
    int cx;
    int w;
    DotrSpriteWord id;
    unsigned char *p;
    int i;
    int a;
    int b;
    int ok;
    int sel;
    int okColor;
    int selColor;
    int base;
    int off;
    int bx;
    int px;
    int py;
    int by;

    width = (DOTR_SELDECK_FLAG != 0) ? width : 0x114;
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
    i = 0;
    p = Save_SelectDeck(arg1);
    do {
        a = SzData_GetDeckCount(arg1, i);
        b = SzData_GetLeader(arg1, i);
        ok = (a >= 0x28 && b != 0x3E7) ? 1 : 0;
        sel = (p[4] == i) ? 1 : 0;
        selColor = sel ? 4 : 7;
        okColor = ok ? -1 : 5;
        base = arg1 ? 0x14 : 0xA;
        off = (width * (i + 1)) >> 2;
        px = x + 0x6B4 + off;
        py = y + 0x74B;
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, gSzDuel_Icon_bytes + (i + 0xE3) * 10, px, py, 0, 1, 0, 0, 0, 0, sel, 0, -1,
                                            hint != 0 ? 0x40 : 0x80);
        bx = x + off - 0x18;
        by = y + 0x46;
        id = lSzDuel_DrawWindow_PicMini_Pipe(id, i + base, bx, by, 0x30, 0x30, *(int *)(state + 0xC58), sel);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, okColor, -1, selColor, bx, by, -0x30, -0x30, 1);
        i++;
    } while (i < 3);
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
    py = y + 0x9C;
    return SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 2, func_0019bef0((char *)text4), text4,
                                       hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
}
