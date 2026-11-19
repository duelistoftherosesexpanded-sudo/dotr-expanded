#include "dotr/ee/custom_panes.h"

/* 0x1c7c00. The callees are declared inside the function body. */
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
void lSzDuel_DrawCustomDuel_Title_Pipe(int arg0, int arg1, int arg2) {
    extern char data_002bf047_custpane[];
    extern char data_002bf04e_custpane[];
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    int text = (int)data_002bf047_custpane;
    int text2 = (int)data_002bf04e_custpane;
    int hint = 0;
    int x = arg1 + 0xE0;
    int y = arg2 + 0x20;
    int tx;
    int ty;
    int element;

    arg0 = lSzDuel_DrawWindow_Base_Pipe(arg0, x, y, 6, 1);
    element = lSzDuel_DrawWindow_Frame_Pipe(arg0, -1, 0x73, 4, x, y, 0x15, 2, 1);
    tx = arg1 + 0x140;
    ty = arg2 + 0x27;
    element = SzDuel_DrawWindow_Font_Pipe(element, tx, ty, 2, 3, func_0019bef0((const char *)text), (signed char *)text,
                            hint != 0 ? (int)data_002be944_custpane : 0);
    SzDuel_DrawWindow_Font_Pipe(element, arg1 + 0x140, arg2 + 0x46, 2, 3, func_0019bef0((const char *)text2),
                  (signed char *)text2, hint != 0 ? (int)data_002be944_custpane : 0);
}
