#include "dotr/ee/trade_panes.h"

/* 0x1c5870. The callees are declared inside the function body. Compiled with
 * #pragma opt_propagation off, set directly before the function and reset after it. */
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
/* The header shows this prototype to the target compiler only (another function declares the
 * callee inside its body with another type); this function uses it. */
int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text, unsigned char *color);
#endif
#ifdef __MWERKS__
#pragma opt_propagation off
#endif
int lSzDuel_DrawTrade_Ready_Pipe(int arg0, int arg1, int arg2) {
    extern char data_002bf03a_tradepane[];
    signed char *text = data_002bf03a_tradepane;
    int width;
    int height;
    int fx;
    int fy;
    int ty;

    lSzDuel_GetFontSize(2, 3, func_0019bef0(text), text, &width, &height);
    width += 0x18;
    height += 0xC;
    fx = arg1 + 0x140 - (width >> 1);
    fy = arg2 + 0xE0 - (height >> 1);
    arg0 = lSzDuel_DrawWindow_Frame_Pipe(arg0, 0xC, -1, 0xB, fx, fy, -width, -height, 1);
    ty = arg2 + 0xE8 - (height >> 1);
    return SzDuel_DrawWindow_Font_Pipe(arg0, arg1 + 0x140, ty, 2, 3, func_0019bef0(text), text, 0);
}
