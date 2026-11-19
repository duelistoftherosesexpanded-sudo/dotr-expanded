#include "dotr/ee/custom_panes.h"

/* 0x1c7460. The callees are declared inside the function body. Compiled with #pragma
 * opt_propagation off and opt_lifetimes off, set directly before the function and reset after it. */
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
#ifdef __MWERKS__
#pragma opt_propagation off
#pragma opt_lifetimes off
#endif
int SzDuel_DrawCustomDuel_Pipe(int arg0) {
    extern char data_002bf03a_custpane[];
    extern int lSzDuel_DrawCustomDuel_Title_Pipe();
    extern int lSzDuel_DrawCustomDuel_PlayerInfo_Pipe();
    extern int lSzDuel_DrawCustomDuel_Menu_Pipe();
    extern int lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe();
    extern int lSzDuel_DrawCustomDuel_Exit_Pipe();
    extern int lSzDuel_DrawWindow_Frame_Pipe();
    extern int SzDuel_DrawWindow_Font_Pipe();
    extern int lSzDuel_Draw_Fade_Pipe();
    signed char *text;
    int id;
    int textY;
    int height;
    int width;
    int height2;
    int width2;
    int mode;
    int fy;

    id = lSzDuel_DrawCustomDuel_PlayerInfo_Pipe(lSzDuel_DrawCustomDuel_PlayerInfo_Pipe(lSzDuel_DrawCustomDuel_Title_Pipe(lSzDuel_DrawCustomDuel_Base_Pipe(SzDuel_StartWindowSessionWithPalette_Pipe(arg0, 2)), 0, 0), 0, 0x10, 0x40), 1, 0x140, 0x40);
    mode = *(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C002) & 0xF0;
    if (mode == 0x40 || (*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C003) & 0xF0) == 0x40) {
        id = lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe(id, 0, 0x1E, 0xE0);
    } else {
        if (mode == 0x20) {
            text = data_002bf03a_custpane;
            lSzDuel_GetFontSize(2, 3, func_0019bef0(text), text, &width, &height);
            width += 0x18;
            height += 0xC;
            fy = 0x144 - height / 2;
            id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, -1, 0xB, 0xA0 - width / 2, fy, -width, -height, 1);
            textY = 0x14C - height / 2;
            id = SzDuel_DrawWindow_Font_Pipe(id, 0xA0, textY, 2, 3, func_0019bef0(text), text, 0);
        } else {
            id = lSzDuel_DrawCustomDuel_Menu_Pipe(id, 0, 0x10, 0xE6);
        }
        if (*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C088) == 0) {
            if ((*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C003) & 0xF0) == 0x20) {
                text = data_002bf03a_custpane;
                lSzDuel_GetFontSize(2, 3, func_0019bef0(text), text, &width2, &height2);
                width2 += 0x18;
                height2 += 0xC;
                fy = 0x144 - height2 / 2;
                id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, -1, 0xB, 0x1E0 - width2 / 2, fy, -width2, -height2, 1);
                textY = 0x14C - height2 / 2;
                id = SzDuel_DrawWindow_Font_Pipe(id, 0x1E0, textY, 2, 3, func_0019bef0(text), text, 0);
            } else {
                id = lSzDuel_DrawCustomDuel_Menu_Pipe(id, 1, 0x140, 0xE6);
            }
        }
    }
    if ((*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C002) & 0xF0) == 0x30 || (*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C003) & 0xF0) == 0x30) {
        id = lSzDuel_DrawCustomDuel_Exit_Pipe(lSzDuel_Draw_Fade_Pipe(id, 0, 0, 0x280, 0x1C0, 0x40), 0, 0);
    }
    SzMsg_Free(0);
    return id;
}
