#include "dotr/ee/trade_scene.h"

/* Draws the trade screen: window session with palette 2, base, title, then per side (left 0,
 * right 1) the bag, the cursor when the side has rows, and while the side's slide amount is
 * non-zero a fade over that half (alpha slide * 64 / 320 as a byte) and the detail of the
 * card under the cursor (the side byte is set first; the slide is passed negated on the
 * right). Modes 4, 6 and 7 of a side add a fade of alpha 64 and the ready mark at x -160 or
 * 160. Mode 5 on either side draws the exit prompt, 6 the confirm prompt, 8 the notice.
 * Ends with SzMsg_Free(0) and returns the cursor. */
DotrTradeWord SzDuel_DrawTrade_Pipe(DotrTradeWord cursor)
{
    struct DotrTradeControl *control = &data_00317490;

    cursor = SzDuel_StartWindowSessionWithPalette_Pipe(cursor, 2);
    cursor = lSzDuel_DrawTrade_Base_Pipe(cursor);
    cursor = lSzDuel_DrawTrade_Title_Pipe(cursor, 0, 0);
    cursor = lSzDuel_DrawTrade_Bag_Pipe(cursor, 0, 0, 0);
    if (data_0031811c != 0) {
        cursor = lSzDuel_DrawTrade_Cursor_Pipe(cursor, 0, 0, 0, control->cursor[0]);
    }
    if (control->slide[0] != 0) {
        cursor = lSzDuel_Draw_Fade_Pipe(cursor, 0, 0, 320, 448, (unsigned char)(((short)control->slide[0] << 6) / 320));
        control->side = 0;
        cursor = lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, control->list[0][control->top0 + control->cursor[0]],
                                                   control->slide[0], 0, 0, 1, 1, 0);
    }
    /* MATCHING DEVICE: the negated test with an empty branch keeps the three compares apart;
     * `== 4 || == 6 || == 7` is compiled as 4 and a range test. Same below. */
    if (control->mode[0] != 4 && control->mode[0] != 6 && control->mode[0] != 7) {
    } else {
        cursor = lSzDuel_Draw_Fade_Pipe(cursor, 0, 0, 320, 448, 64);
        cursor = lSzDuel_DrawTrade_Ready_Pipe(cursor, -160, 0);
    }
    cursor = lSzDuel_DrawTrade_Bag_Pipe(cursor, 1, 0, 0);
    if (control->count1 != 0) {
        cursor = lSzDuel_DrawTrade_Cursor_Pipe(cursor, 1, 0, 0, control->cursor[1]);
    }
    if (control->slide[1] != 0) {
        cursor = lSzDuel_Draw_Fade_Pipe(cursor, 320, 0, 320, 448, (unsigned char)(((short)control->slide[1] << 6) / 320));
        control->side = 1;
        cursor = lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, control->list[1][control->top1 + control->cursor[1]],
                                                   -control->slide[1], 0, 1, 1, 1, 0);
    }
    if (control->mode[1] != 4 && control->mode[1] != 6 && control->mode[1] != 7) {
    } else {
        cursor = lSzDuel_Draw_Fade_Pipe(cursor, 320, 0, 320, 448, 64);
        cursor = lSzDuel_DrawTrade_Ready_Pipe(cursor, 160, 0);
    }
    if (control->mode[0] == 5 || control->mode[1] == 5) {
        cursor = lSzDuel_DrawTrade_Exit_Pipe(cursor, 0, 0);
    }
    if (control->mode[0] == 6 || control->mode[1] == 6) {
        cursor = lSzDuel_DrawTrade_Do_Pipe(cursor, 0, 0);
    }
    if (control->mode[0] == 8 || control->mode[1] == 8) {
        cursor = lSzDuel_DrawTrade_Notice_Pipe(cursor, 0, 0);
    }
    SzMsg_Free(0);
    return cursor;
}
