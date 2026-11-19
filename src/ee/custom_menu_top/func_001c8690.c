#include "dotr/ee/custom_menu_top.h"

/* lSzDuel_DrawCustomDuel_Menu_Top_Pipe (0x1c8690): draws the top page of the custom-duel menu. */
DotrMenuTopWord lSzDuel_DrawCustomDuel_Menu_Top_Pipe(DotrMenuTopWord arg0, unsigned char *arg1, int x, int y) {
    unsigned char *cursor = DOTR_MENU_TOP_BLOCK;
    signed char *text1 = data_002bf05b_menutop;
    signed char *text2 = data_002bef98_menutop;
    signed char *text3 = data_002bf03a_menutop;
    int row;
    int enabled;
    int width = 0x25A;
    unsigned char *list;
    int hint = 0;
    int col;
    int item;
    int sel;
    int w;
    int *info;
    float scale;
    int px;
    int py;

    width = (DOTR_MENU_TOP_WIDE != 0) ? width : 0x12A;
    list = SzDuel_CustomDuelMenu_menutop[(DOTR_MENU_TOP_WIDE != 0) ? 1 : 0];
    arg0 = SzMsg_LoadFont_Pipe(arg0, 1, 0x81, 0, 0x16);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 2, 0x82, 0, 0x2C);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 3, 0x83, 0, 0x42);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 4, 0x84, 0, 0x58);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 5, 0x85, 0, 0x6E);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 6, 0x86, 0, 0x84);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 7, 0x87, 0, 0x9A);
    arg0 = SzMsg_LoadFont_Pipe(arg0, 8, 0x88, 0, 0xB0);
    arg0 = lSzDuel_DrawWindow_Frame_Pipe(RhText_SetTextureFlush(arg0), (DotrMenuTopWord)arg1, (DotrMenuTopWord)arg1, 0, x, y, -(width - 0x14), -0xAE, 1);
    for (row = 0; row < 2; row++) {
        for (col = 0; col < 5; col++) {
            item = list[row * 5 + col];
            enabled = 1;
            if (item == 0xFF) {
                continue;
            }
            if (item == 7) {
                if (SzData_GetMapEditFlag((DotrMenuTopWord)arg1) == 0) {
                    enabled = 0;
                }
            }
            if (enabled) {
                info = SzMsg_GetInfo(item + 1);
                if (cursor[0x88]) {
                    w = info[3];
                    scale = 1.0f;
                    if (w > 0xAC) {
                        scale = 172.0f / (float)w;
                    }
                    arg0 = SzMsg_DrawRatioX_Pipe(arg0, item + 1, x + (width / 3) * (row + 1) - (int)((float)(w >> 1) * scale),
                                         y + 0x20 + col * 0x20,
                                         scale, 1.0f, 0, SzMsg_DrawRect_Pipe);
                } else {
                    w = info[3];
                    scale = 1.0f;
                    if (w > 0xAC) {
                        scale = 172.0f / (float)w;
                    }
                    arg0 = SzMsg_DrawRatioX_Pipe(arg0, item + 1, x + (width >> 1) - (int)((float)(w >> 1) * scale),
                                         y + 0x1A + col * 0x14,
                                         scale, 0.90909094f, 0, SzMsg_DrawRect_Pipe);
                }
                sel = cursor[(DotrMenuTopWord)arg1 + 2] & 0xF;
                if (sel == item) {
                    if (cursor[0x88]) {
                        arg0 = lSzDuel_DrawWindow_Frame_Pipe(arg0, -1, -1, 6, x + (width / 3) * (row + 1) - 0x62,
                                             y + 0x1C + col * 0x20, -0xBE, -0x1A, 1);
                    } else {
                        arg0 = lSzDuel_DrawWindow_Frame_Pipe(arg0, -1, -1, 6, x + (width >> 1) - 0x62,
                                             y + 0x16 + col * 0x14, -0xBE, -0x18, 1);
                    }
                }
            }
            SzMsg_Free(item + 1);
        }
    }
    arg0 = lSzDuel_DrawWindow_ExBoard_Pipe(arg0, (DotrMenuTopWord)arg1, x + 0x20, y + 0x85, -(width - 0x3A), -0x34, -1, 0);
    px = x + 0x6D0 + ((width - 0x3A) >> 2);
    py = y + 0x7AC;
    arg0 = lSzDuel_DrawWindow_ExIcon_Pipe(arg0, &data_00290a58_menutop, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x6E0 + ((width - 0x3A) >> 2);
    py = y + 0x7AC;
    arg0 = lSzDuel_DrawWindow_ExIcon_Pipe(arg0, &data_00290a62_menutop, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x20 + ((width - 0x3A) >> 2);
    py = y + 0xA7;
    arg0 = SzDuel_DrawWindow_Font_Pipe(arg0, px, py, 1, 2, func_0019bef0((char *)text1), text1,
                         hint != 0 ? (DotrMenuTopWord)data_002be944_menutop : 0);
    px = x + 0x6D8 + (((width - 0x3A) * 2) >> 2);
    py = y + 0x7AC;
    arg0 = lSzDuel_DrawWindow_ExIcon_Pipe(arg0, &data_00290a6c_menutop, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x20 + (((width - 0x3A) * 2) >> 2);
    py = y + 0xA7;
    arg0 = SzDuel_DrawWindow_Font_Pipe(arg0, px, py, 1, 2, func_0019bef0((char *)text2), text2,
                         hint != 0 ? (DotrMenuTopWord)data_002be944_menutop : 0);
    px = x + 0x6D8 + (((width - 0x3A) * 3) >> 2);
    py = y + 0x7AC;
    arg0 = lSzDuel_DrawWindow_ExIcon_Pipe(arg0, &data_00290a76_menutop, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    px = x + 0x20 + (((width - 0x3A) * 3) >> 2);
    py = y + 0xA7;
    return SzDuel_DrawWindow_Font_Pipe(arg0, px, py, 1, 2, func_0019bef0((char *)text3), text3,
                         hint != 0 ? (DotrMenuTopWord)data_002be944_menutop : 0);
}
