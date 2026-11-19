#include "dotr/ee/trade_panes.h"

/* 0x1c5550. The callees are declared inside the function body. */
#ifndef __MWERKS__
/* Host build only: this C passes addresses as 32-bit words and mixes char signedness
 * and pointer types as the target compiler accepts; GCC would reject these as errors. */
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef __MWERKS__
#pragma opt_dead_assignments reset
#endif
int lSzDuel_DrawTrade_Cursor_Pipe(int id, int player, int arg2, int arg3, int arg4) {
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, void *, int, int, int, int, int, int, int, int, int, int, int, int);
    extern char data_002bf030_tradepane[];
    extern char data_002befec_tradepane[];
    extern int data_00290a62_tradepane;
    extern int data_00290a58_tradepane;
    extern int data_00290a4e_tradepane;
    extern int data_00290a76_tradepane;
    unsigned char *state = data_00317490_tradepane.b;
    signed char *text1 = data_002bf030_tradepane + 3;
    signed char *text2 = data_002befec_tradepane;
    signed char *text4 = (signed char *)&data_002bf00b_tradepane;
    int hint = 0;
    unsigned short busy;
    int x;
    int y;
    int sx;
    int sy;
    int px;
    int py;

    x = arg2 + 8 + (player != 0 ? 0x140 : 0);
    y = arg3 + 0x70 + arg4 * 0x36;
    sx = x + 0x6C0;
    sy = y + 0x720;
    busy = (player != 0) ? *(unsigned short *)(state + 0xC98) : *(unsigned short *)(state + 0xC96);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, 4, 6, x - 2, y - 2, -0x130, -0x36, 1);
    if (busy != 0) {
        return id;
    }
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xD, -1, 7, x, y + 0x36, -0x130, -0x14, 1);
    if (state[player + 0xCC0] < 2) {
        px = sx + 5;
        py = sy + 0x38;
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a62_tradepane, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a58_tradepane, px + 0x11, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        px = x + 0x2C;
        py = y + 0x3B;
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text1), text1,
                           hint != 0 ? (int)data_002be944_tradepane : 0);
    }
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a4e_tradepane, sx + 0x6B, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    px = x + 0x81;
    py = y + 0x3B;
    id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text2), text2,
                       hint != 0 ? (int)data_002be944_tradepane : 0);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a76_tradepane, sx + 0xC9, sy + 0x38, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    px = x + 0xDF;
    py = y + 0x3B;
    return SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, func_0019bef0((char *)text4), text4,
                         hint != 0 ? (int)data_002be944_tradepane : 0);
}
