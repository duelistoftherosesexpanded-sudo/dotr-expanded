#include "dotr/ee/scaled_sprite.h"

/* lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe: draws the card-display window of the custom
 * duel menu for `side` at (x, y): the title message 129 + the low four bits of the side's
 * byte at 0xa9c002, centred at x + 0x8a in a 178 x 32 frame; the 276 x 150 window frame; two
 * choice captions (0x2bf056, 0x2bf051) at y + 0x4d, the first coloured when the side's byte
 * at 0xa9c00c is set, the second when it is clear; then the board strip, icons 252..254 and
 * the captions at 0x2bf05b / 0x2bef93 with no colour. Returns the advanced cursor.
 * The side parameter is a byte pointer indexed by the absolute addresses (DotrSelDeckSide,
 * DOTR_SELDECK_SIDE_BYTE). */
DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe(DotrSpriteWord arg0, DotrSelDeckSide arg1, int x, int y)
{
    char *text3 = data_002bf05b;
    char *text4 = data_002bef93;
    char *text2 = data_002bf051;
    char *text1 = data_002bf056;
    int hint = 0;
    DotrSpriteWord win;
    int tx;
    int ty;
    DotrSpriteWord id;

    id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 1, (DOTR_SELDECK_SIDE_BYTE(arg1, 0xA9C002) & 0xF) + 0x81, 0, 0x16));
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, 3, 4, x + 0x31, y, -0xB2, -0x20, 1);
    id = func_001fe7d0_sprite(id, 1, x + 0x8A - (((int *)SzMsg_GetInfo(1))[3] >> 1), y + 5, 0,
                              SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    win = lSzDuel_DrawWindow_Frame_Pipe(id, (int)arg1, -1, 0xB, x, y + 0x20, -0x114, -0x96, 1);
    tx = x + 0x5C;
    ty = y + 0x4D;
    id = SzDuel_DrawWindow_Font_Pipe(win, tx, ty, 0, 3, func_0019bef0((char *)text1), text1,
                                     DOTR_SELDECK_SIDE_BYTE(arg1, 0xA9C00C) != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    tx = x + 0xB8;
    ty = y + 0x4D;
    id = SzDuel_DrawWindow_Font_Pipe(id, tx, ty, 0, 3, func_0019bef0((char *)text2), text2,
                                     DOTR_SELDECK_SIDE_BYTE(arg1, 0xA9C00C) != 0 ? 0 : DOTR_SELDECK_HINT_COLOUR);
    id = lSzDuel_DrawWindow_ExBoard_Pipe(id, (int)arg1, x + 0x12, y + 0x7A, -0xF0, -0x34, -1, 0);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a58, x + 0x6FE, y + 0x7A1, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a62, x + 0x70E, y + 0x7A1, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    tx = x + 0x4E;
    ty = y + 0x9C;
    id = SzDuel_DrawWindow_Font_Pipe(id, tx, ty, 1, 2, func_0019bef0((char *)text3), text3,
                                     hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a6c, x + 0x742, y + 0x7A1, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    tx = x + 0x8A;
    ty = y + 0x9C;
    return SzDuel_DrawWindow_Font_Pipe(id, tx, ty, 1, 2, func_0019bef0((char *)text4), text4,
                                       hint != 0 ? DOTR_SELDECK_HINT_COLOUR : 0);
}
