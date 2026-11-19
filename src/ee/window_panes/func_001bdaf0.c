#include "dotr/ee/window_panes.h"

#ifndef __MWERKS__
/* Host build only: the text pointer is signed char and the string it is set to is char. */
#pragma GCC diagnostic ignored "-Wpointer-sign"
#endif

#ifdef __MWERKS__
#pragma opt_propagation off
#endif
int lSzDuel_DrawBuildDeck_Detail_Pipe(int arg0, int cardNo, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    extern int *SzMsg_GetInfo(int);
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void (*)(void));
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    extern int lSzDuel_DrawWindow_ExBoard_Pipe(int, int, int, int, int, int, int, int);
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, void *, int, int, int, int, int, int, int, int, int, int, int, int);
    extern int lSzDuel_DrawWindow_Frame_Pipe();
    extern unsigned char *SzDuel_GetCardConstInfo(int);
    extern int lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(int, int, int, int, int);
    extern int lSzDuel_DrawBuildDeck_Detail_Special_Pipe(int, int, int, int, int);
    extern int lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(int, int, int, int, int, int);
    extern int lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(int, int, int, int, int, int);
    extern char data_002befc3_panes[];
    extern char gSzDuel_aicolMsg_panes[];
    extern char data_002be944_panes[];
    extern char data_002be948_panes[];
    extern char data_002be94c_panes[];
    extern unsigned char data_00290abc_panes[];
    extern unsigned char data_00290828_panes[];
    extern unsigned char data_00290ac6_panes[];
    extern unsigned char data_00290832_panes[];
    extern int data_00290a76_panes;
    unsigned char *data;
    unsigned char *card;
    signed char *text;
    int xoff;
    int color;
    float scale;
    int id;
    int *info;
    int bx;
    int px;
    unsigned char *sel;
    int tx;
    int ty;

    data = data_00317490_panes.b;
    card = SzDuel_GetCardConstInfo(cardNo);
    text = data_002befc3_panes;
    xoff = (arg4 != 0) ? 0x280 : -0x140;
    color = 2;
    if (card != 0) {
        scale = 1.0f;
        color = SzDuel_Kind2Type(card[0] >> 5);
        if (color == 0 && *(unsigned short *)(card + 4) != 0xFFFF) {
            color = 4;
        }
        color += 7;
        arg0 = SzMsg_LoadFont_Pipe(arg0, 1, SzDuel_GetMsgName(cardNo), 0, 0x16);
        id = RhText_SetTextureFlush(arg0);
        bx = arg2 + xoff;
        tx = bx + 0x14;
        ty = arg3 + 0x2E;
        id = dotr_pane_detail_frame(id, color, tx, ty);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, color, -1, 4, bx + 0x27, arg3 + 0x10, -0xF0, -0x1E, 1);
        info = SzMsg_GetInfo(1);
        if (info[3] > 0xE6) {
            scale = 230.0f / (float)info[3];
        }
        id = SzMsg_DrawRatioX_Pipe(id, 1, bx + 0xA1 - (int)((float)(info[3] >> 1) * scale), arg3 + 0x15, scale, 1.0f,
                           (DotrPaneWord)gSzDuel_aicolMsg_panes, SzMsg_DrawRect_Pipe);
        id = SzMsg_DrawRatioX_Pipe(id, 1, bx + 0xA0 - (int)((float)(info[3] >> 1) * scale), arg3 + 0x14, scale, 1.0f,
                           (DotrPaneWord)data_002be944_panes, SzMsg_DrawRect_Pipe);
        arg0 = SzMsg_DrawRatioX_Pipe(id, 1, bx + 0x9F - (int)((float)(info[3] >> 1) * scale), arg3 + 0x13, scale, 1.0f,
                             (DotrPaneWord)data_002be948_panes, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    } else if (cardNo == 0x29F) {
        scale = 1.0f;
        color = 0;
        color += 7;
        id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 1, SzDuel_GetMsgName(cardNo), 0, 0x16));
        bx = arg2 + xoff;
        tx = bx + 0x14;
        ty = arg3 + 0x2E;
        id = dotr_pane_detail_frame(id, color, tx, ty);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, color, -1, 4, bx + 0x27, arg3 + 0x10, -0xF0, -0x1E, 1);
        info = SzMsg_GetInfo(1);
        if (info[3] > 0xE6) {
            scale = 230.0f / (float)info[3];
        }
        id = SzMsg_DrawRatioX_Pipe(id, 1, bx + 0xA1 - (int)((float)(info[3] >> 1) * scale), arg3 + 0x15, scale, 1.0f,
                           (DotrPaneWord)gSzDuel_aicolMsg_panes, SzMsg_DrawRect_Pipe);
        id = SzMsg_DrawRatioX_Pipe(id, 1, bx + 0xA0 - (int)((float)(info[3] >> 1) * scale), arg3 + 0x14, scale, 1.0f,
                           (DotrPaneWord)data_002be944_panes, SzMsg_DrawRect_Pipe);
        arg0 = SzMsg_DrawRatioX_Pipe(id, 1, bx + 0x9F - (int)((float)(info[3] >> 1) * scale), arg3 + 0x13, scale, 1.0f,
                             (DotrPaneWord)data_002be948_panes, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    }
    id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(arg0, 2, 0xA3, 0, 0x2C),
                                                                  3, 0xA4, 0, 0x42), 4, 0xA5, 0, 0x58), 5, 0xA6, 0, 0x6E));
    if (arg7 != 0) {
        if (data[0xD5D] == 0) {
            id = lSzDuel_DrawWindow_ExBoard_Pipe(id, 2, arg2 - 0x110, arg3 + 0x142, -0x40, -0x1C, -1, 0);
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290abc_panes, arg2 + 0x5B5, arg3 + 0x86B, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290828_panes, arg2 + 0x5CC, arg3 + 0x86A, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
            id = lSzDuel_DrawWindow_ExBoard_Pipe(id, 2, arg2 - 0x110, arg3 + 0x190, -0x40, -0x1C, -1, 0);
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290ac6_panes, arg2 + 0x5B5, arg3 + 0x8B9, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290832_panes, arg2 + 0x5CC, arg3 + 0x8B4, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        } else {
            int cursor;

            cursor = lSzDuel_DrawWindow_ExBoard_Pipe(id, 2, arg2 - 0x114, arg3 + 0x161, -0x44, -0x2C, -1, 0);
            id = cursor;
            if (data[0xD5D] == 1) {
                id = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, &data_00290a76_panes, arg2 + 0x5C6, arg3 + 0x886, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
            }
            tx = arg2 - 0xF2;
            ty = arg3 + 0x17B;
            id = SzDuel_DrawWindow_Font_Pipe(id, tx, ty, 1, 3, func_0019bef0((char *)text), text, 0);
        }
    }
    px = bx = arg2 + (((arg4 != 0) ? 0x280 : ((arg7 != 0) ? 0x24 : 0) - 0x140) + 0x4F);
    ty = arg3 + 0x13E;
    id = lSzDuel_DrawWindow_ExBoard_Pipe(id, (arg6 != 0) ? color : -1, px, ty, -0xA0, -0x72, (arg6 != 0) ? color : -1, 0);
    tx = bx + 0xA;
    ty = arg3 + 0x142 + (arg4 + data)[0xCBC] * 26;
    sel = arg4 + data + 0xCBC;
    if (arg5 != 0) {
        id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, -1, 6, tx - 2, ty - 2, -0x8E, -0x1C, 1);
    } else {
        id = lSzDuel_DrawWindow_ExBoard_Pipe(id, 2, tx, ty, -0x8E, -0x1C, -1, 0);
    }
    tx = bx + 0xE;
    ty = arg3 + 0x145;
    info = SzMsg_GetInfo(2);
    id = SzMsg_DrawRatioX_Pipe(id, 2, tx, ty, (info[3] >= 0x87) ? 134.0f / (float)info[3] : 1.0f, 1.0f,
                       (DotrPaneWord)data_002be94c_panes, SzMsg_DrawRect_Pipe);
    ty = arg3 + 0x15F;
    info = SzMsg_GetInfo(3);
    if (info[3] > 0x86) {
        scale = 134.0f / (float)info[3];
    } else {
        scale = 1.0f;
    }
    id = SzMsg_DrawRatioX_Pipe(id, 3, tx, ty, scale, 1.0f,
                       (DotrPaneWord)data_002be94c_panes, SzMsg_DrawRect_Pipe);
    ty = arg3 + 0x179;
    info = SzMsg_GetInfo(4);
    if (info[3] > 0x86) {
        scale = 134.0f / (float)info[3];
    } else {
        scale = 1.0f;
    }
    id = SzMsg_DrawRatioX_Pipe(id, 4, tx, ty, scale, 1.0f,
                       (DotrPaneWord)data_002be94c_panes, SzMsg_DrawRect_Pipe);
    ty = arg3 + 0x193;
    info = SzMsg_GetInfo(5);
    if (info[3] > 0x86) {
        scale = 134.0f / (float)info[3];
    } else {
        scale = 1.0f;
    }
    arg0 = SzMsg_DrawRatioX_Pipe(id, 5, tx, ty, scale, 1.0f,
                         (DotrPaneWord)data_002be94c_panes, SzMsg_DrawRect_Pipe);
    SzMsg_Free(2);
    SzMsg_Free(3);
    SzMsg_Free(4);
    SzMsg_Free(5);
    if (card != 0 || cardNo == 0x29F) {
        switch (*sel) {
        case 0:
            arg0 = lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(arg0, cardNo, arg2, arg3, arg4);
            break;
        case 1:
            arg0 = lSzDuel_DrawBuildDeck_Detail_Special_Pipe(arg0, cardNo, arg2, arg3, arg4);
            break;
        case 2:
            arg0 = lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(arg0, cardNo, arg2, arg3, arg4, color);
            break;
        case 3:
            arg0 = lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(arg0, cardNo, arg2, arg3, arg4, color);
            break;
        }
    }
    return arg0;
}
#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
