#include "dotr/ee/window_panes.h"

int lSzDuel_DrawDialog_Window_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7,
                  int arg8, DotrPaneWord arg9, int arg10)
{
    extern unsigned char gSzDuel_Icon_panes[];
    extern int lSzDuel_DrawWindow_Icon_Pipe();
    extern int lSzDuel_DrawWindow_Frame_Pipe();
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void (*)(void));
    unsigned char *state;
    int frame;
    int flip;
    int card;
    int textW;
    int textH;
    int minW;
    int minH;
    int id;
    int *info;
    float scale;
    int nw;

    state = data_00317490_panes.b;
    frame = (unsigned int)SzGame_GetFrameCount() / 5;
    if (arg6 < 0) {
        flip = 1;
    } else {
        flip = 0;
    }
    card = (arg2 >= 0x8000) ? 1 : 0;
    scale = 1.0f;
    arg6 = dotr_pane_dlg_abs(arg6);
    if (card) {
        arg2 -= 0x8000;
    }
    if (arg8 > 0) {
        lSzDuel_GetFontSize(2, 3, arg8, (signed char *)arg9, &textW, &textH);
        textW += 0x18;
        textH += 0xC;
    } else {
        textH = 0;
        textW = 0;
    }
    if (arg3 >= 0) {
        arg0 = SzMsg_LoadFont_Pipe(arg0, 1, arg3, 0, 0x16);
        arg0 = RhText_SetTextureFlush(arg0);
        info = SzMsg_GetInfo(1);
        if (flip) {
            if (arg6 - 0x18 < info[3]) {
                scale = (float)(arg6 - 0x18) / (float)info[3];
            }
            minW = arg6;
        } else {
            minW = ((info[3] > textW) ? info[3] : textW) + 0x18;
        }
        minH = info[4] + ((arg8 > 0) ? textH + 0x18 : 0xC) + 0xC;
    } else {
        minW = textW + 0x18;
        minH = (arg8 > 0) ? textH + 0x18 : 0xC;
    }
    if (card) {
        minH += 0x3C;
    } else if (arg2 >= 0) {
        minH += 0x8C;
    }
    if (arg10 != 0) {
        minH += 0x1C;
    }
    if (arg6 < minW) {
        arg6 = minW;
    }
    if (arg7 < minH) {
        arg7 = minH;
    }
    nw = -arg6;
    (void)nw; 
    id = lSzDuel_DrawWindow_Frame_Pipe(arg0, 0xD, -1, 1, arg4 - (arg6 >> 1) - 4, arg5, nw, -arg7, 1);
    if (arg8 > 0) {
        id = lSzDuel_DrawWindow_Frame_Pipe(id, arg1, -1, 0xB, arg4 - (textW >> 1), arg5 + 0xC, -textW, -textH, 1);
        id = SzDuel_DrawWindow_Font_Pipe(id, arg4, arg5 + 0x14, 2, 3, arg8, (signed char *)arg9, 0);
    }
    if (card) {
        minW = arg4 - 0x18;
        minH = arg5 + ((arg8 > 0) ? textH + 0x18 : 0xC);
        id = lSzDuel_DrawWindow_PicMini_Pipe(id, arg2, minW, minH, 0x30, 0x30, *(int *)(state + 0xC58), 1);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, -1, 4, minW, minH, 3, 6, 1);
    } else if (arg2 >= 0) {
        id = lSzDuel_DrawWindow_PicItem_Pipe(id, arg2, arg4 - 0x40, arg5 + ((arg8 > 0) ? textH + 0x18 : 0xC), 0x80, 0x80,
                           *(int *)(state + 0xC58));
    }
    if (arg3 >= 0) {
        minW = arg4 - (int)((float)info[3] * scale / 2.0f);
        minH = arg5 + ((arg8 > 0) ? textH + 0x18 : 0x10);
        if (card) {
            minH += 0x3C;
        } else if (arg2 >= 0) {
            minH += 0x8C;
        }
        id = SzMsg_DrawRatioX_Pipe(id, 1, minW, minH, scale, 1.0f, 0, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    }
    if (arg10 != 0) {
        id = lSzDuel_DrawWindow_Icon_Pipe(id, gSzDuel_Icon_panes + (frame % 3 + 0xBE) * 10, arg4 + 0x6B8, arg5 + 0x704 + arg7,
                           0, 1, 0, 0, 1, 0, -1);
    }
    return id;
}
