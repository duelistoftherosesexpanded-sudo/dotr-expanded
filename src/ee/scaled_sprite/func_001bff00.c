#include "dotr/ee/scaled_sprite.h"

/* lSzDuel_DrawBuildDeck_BagCursor_Pipe: draws the bag cursor pane for row `arg3` (0x36 pixels
 * per row from y + 0x70) at x + 8: an outer frame 0x130 x 0x36; when the halfword at
 * 0x317490 + 0xc96 is zero also an inner frame 0x130 x 0x14, icons 253 and 252 with the first
 * caption, icon 251 with the second caption (byte at 0x3181ed == 3) or the third, icon 255
 * with the fourth caption, and icon 202. Returns the advanced cursor. */
DotrSpriteWord lSzDuel_DrawBuildDeck_BagCursor_Pipe(DotrSpriteWord id, int arg1, int arg2, int arg3)
{
    unsigned char *state = data_00317490_bytes;
    int flag = (BuildDeckHorizontalCursorIndex_bag == 3) ? 1 : 0;
    char *text1 = data_002befe7;
    char *text2 = data_002befec;
    char *text3 = data_002beff3;
    char *text4 = data_002beffb;
    int hint = 0;
    int x;
    int y;
    int sx;
    int sy;
    int px;
    int py;

    x = arg1 + 8;
    y = arg2 + 0x70 + arg3 * 0x36;
    sx = x + 0x6C0;
    sy = y + 0x720;
    id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, 4, 6, x - 2, y - 2, -0x130, -0x36, 1);
    if (*(unsigned short *)(state + 0xC96) != 0) {
        return id;
    }
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xD, -1, 7, x, y + 0x36, -0x130, -0x14, 1);
    px = sx + 5;
    py = sy + 0x38;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a62, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a58, px + 0x11, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    px = x + 0x2C;
    py = y + 0x3B;
    id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text1), text1,
                                     hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    if (flag) {
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a4e, sx + 0x6B, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        px = x + 0x81;
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text2), text2,
                                         hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    } else {
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a4e, sx + 0x5B, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        px = x + 0x71;
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text3), text3,
                                         hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    }
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a76, sx + 0xC9, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    px = x + 0xDF;
    py = y + 0x3B;
    id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text4), text4,
                                     hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    return lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290864, sx + 0x11B, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
}
