#include "dotr/ee/deck_detail.h"

/* 0x1c02e0. The callees are declared inside the function body. */
#ifndef __MWERKS__
/* Host build only: the code passes addresses as 32-bit words and mixes char signedness and
 * pointer types, which the target compiler accepts and GCC would reject as errors. */
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
int lSzDuel_DrawBuildDeck_DeckCursor_Pipe(int id, int arg1, int arg2, int arg3) {
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, void *, int, int, int, int, int, int, int, int, int, int, int, int);
    extern char data_002befe7_deckpane[];
    extern char data_002befec_deckpane[];
    extern char data_002beff3_deckpane[];
    extern int data_00290a62_deckpane;
    extern int data_00290a58_deckpane;
    extern int data_00290a4e_deckpane;
    extern unsigned char BuildDeckHorizontalCursorIndex_deckpane;
    unsigned char *state = data_00317490_deckpane.b;
    int flag = (BuildDeckHorizontalCursorIndex_deckpane == 4) ? 1 : 0;
    signed char *text1 = data_002befe7_deckpane;
    signed char *text2 = data_002befec_deckpane;
    signed char *text3 = data_002beff3_deckpane;
    int hint = 0;
    int x;
    int y;
    int sx;
    int sy;
    int px;
    int py;

    x = arg1 + 0x148;
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
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a62_deckpane, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a58_deckpane, px + 0x11, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    px = x + 0x2C;
    py = y + 0x3B;
    id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text1), text1,
                       hint != 0 ? (int)data_002be944_deckpane : 0);
    if (flag) {
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a4e_deckpane, sx + 0x6B, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        px = x + 0x81;
        py = y + 0x3B;
        return SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text2), text2,
                             hint != 0 ? (int)data_002be944_deckpane : 0);
    } else {
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a4e_deckpane, sx + 0x5B, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        px = x + 0x71;
        py = y + 0x3B;
        return SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text3), text3,
                             hint != 0 ? (int)data_002be944_deckpane : 0);
    }
}
