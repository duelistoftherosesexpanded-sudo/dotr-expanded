#include "dotr/ee/custom_panes.h"

/* 0x1cb6b0. The callees are declared inside the function body. */
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
int lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(int id, int player, int x, int y) {
    extern int *SzMsg_GetInfo(int);
    extern int SzMsg_DrawX_Pipe(int, int, int, int, int, void (*)(void));
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    extern int SzDuel_DrawWindow_FontDigit_Pipe(int, int, int, int, int, int, int);
    extern int lSzDuel_DrawWindow_ExBoard_Pipe(int, int, int, int, int, int, int, int);
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, void *, int, int, int, int, int, int, int, int, int, int, int, int);
    extern unsigned char gSzDuel_Icon_custpane[];
    extern unsigned char gSzDuel_aaicolField_custpane[];
    extern char data_002bf074_custpane[];
    extern char data_002bef93_custpane[];
    extern int data_002902c4_custpane;
    extern int data_00290256_custpane;
    extern int data_00290a58_custpane;
    extern int data_00290a62_custpane;
    extern int data_00290a6c_custpane;
    int ret;
    unsigned char *state = data_00317490_custpane.b;
    unsigned char *save = (unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C000);
    DotrCustPaneMaps1cb6b0 *maps;
    signed char *text1;
    signed char *text2;
    int sel;
    int hint = 0;
    int tx;
    int ty;
    int i;
    int j;
    int row;
    int count;
    int n;
    int hl;
    unsigned char *board;
    int px;
    int py;
    int w;
    int h;
    DotrCustPaneRect1cb6b0 rect __attribute__((aligned(8)));
    DotrCustPanePacket1cb6b0 packet __attribute__((aligned(8)));

    maps = SzData_GetData(0);
    text1 = data_002bf074_custpane;
    text2 = data_002bef93_custpane;
    sel = (*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C08C) + 2) % *(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C08D);
    id = SzMsg_LoadFont_Pipe(id, 1, 0x88, 0, 0x16);
    id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(id, 2, 0x8B, 0, 0x2C));
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, 3, 4, x + 0xBA, y, -0xD0, -0x20, 1);
    id = SzMsg_DrawX_Pipe(id, 1, x + 0x122 - (SzMsg_GetInfo(1)[3] >> 1), y + 5, 0, SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    py = y + 0x20;
    id = lSzDuel_DrawWindow_Frame_Pipe(id, player, -1, 0xB, x, py, -0x244, -0xAA, 1);
    py = y + 0x35;
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, -1, 6, x + 0x5D, py, -0xEC, -0x7C, 1);
    px = x + 0x71D;
    py = y + 0x789;
    ((DotrCustPaneTag1cb6b0 *)rect.tag)->flags = 6;
    ((DotrCustPaneTag1cb6b0 *)rect.tag)->count = 3;
    ((DotrCustPaneTag1cb6b0 *)rect.tag)->registers = 0x441;
    ((DotrCustPaneTag1cb6b0 *)rect.tag)->padding = 0;
    rect.color.r = 0x80;
    rect.color.g = 0;
    rect.color.b = 0;
    rect.color.a = 0x80;
    rect.color.q = 1.0f;
    dotr_custpane_set_corner_1cb6b0(&rect.xyz[0], px, py);
    dotr_custpane_set_corner_1cb6b0(&rect.xyz[1], px + 0xF0, py + 0x18);
    id = ExSortPrim_Pipe(id, (int)rect.tag, 0, 0);
    py = y + 0x35;
    id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, -1, 6, x + 0x5D, py, -0xEC, -0x7C, hint != 0 ? 0 : 1);
    for (i = 0; i < 5; i++) {
        hl = (i == 2) ? 0 : (int)data_002be944_custpane;
        n = (save[0x8C] + i) % save[0x8D];
        id = SzMsg_DrawX_Pipe(id, 2, x + 0x69, y + 0x3A + i * 0x18, hl, SzMsg_DrawRect_Pipe);
        id = SzDuel_DrawWindow_ExDigit_Pipe(id, x + 0x119, y + 0x3A + i * 0x18, 2, n, 0, 1, hl);
    }
    SzMsg_Free(2);
    py = y + 0x78D;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002902c4_custpane, x + 0x71F, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    py = y + 0x78D;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002902c4_custpane, x + 0x803, py, 0, 1, 0, 0, 1, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    board = &maps->maps[sel][0][0];
    px = x + 0x81C;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, gSzDuel_Icon_custpane + (board[i * 7 + j] + 0x10E) * 10, px + j * 16,
                               y + 0x75C + i * 16, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        }
    }
    py = y + 0x35;
    id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, 1, 1, x + 0x155, py, -0x76, -0x76, 1);
    py = y + 0x7C7;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290256_custpane, x + 0x834, py, 0, 1, 0, 0, 0, 1, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    for (row = 0; row < 10; row++) {
        count = 0;
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                if (row == board[i * 7 + j]) {
                    count++;
                }
            }
        }
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, gSzDuel_Icon_custpane + (row + 0x10E) * 10, x + 0x89B, y + 0x745 + row * 16, 0, 1, 0,
                           0, 0, 0, 1, 0, -1, 0x80);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xD, -1, 5, x + 0x1EB, y + 0x25 + row * 16, -0x54, -0x10, 1);
        px = x + 0x8AB;
        py = y + 0x745 + row * 16;
        w = (int)((float)count * 84.0f / 49.0f);
        h = 8;
        ((DotrCustPaneTag1cb6b0 *)packet.tag)->flags = 0x14;
        ((DotrCustPaneTag1cb6b0 *)packet.tag)->count = 8;
        ((DotrCustPaneTag1cb6b0 *)packet.tag)->registers = 0x41414141;
        ((DotrCustPaneTag1cb6b0 *)packet.tag)->padding = 0;
        dotr_custpane_set_color_1cb6b0(&packet.points[0].color, &packet.points[1].color, &((DotrCustPaneSource1cb6b0 *)gSzDuel_aaicolField_custpane)[row * 3]);
        dotr_custpane_set_color_1cb6b0(&packet.points[2].color, &packet.points[3].color, &((DotrCustPaneSource1cb6b0 *)gSzDuel_aaicolField_custpane)[row * 3 + 1]);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[0].xyz, px, py, state);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[1].xyz, px + w, py, state);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[2].xyz, px, py + h, state);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[3].xyz, px + w, py + h, state);
        id = ExSortPrim_Pipe(id, (int)packet.tag, 0, 0);
        dotr_custpane_set_color_1cb6b0(&packet.points[0].color, &packet.points[1].color, &((DotrCustPaneSource1cb6b0 *)gSzDuel_aaicolField_custpane)[row * 3 + 1]);
        dotr_custpane_set_color_1cb6b0(&packet.points[2].color, &packet.points[3].color, &((DotrCustPaneSource1cb6b0 *)gSzDuel_aaicolField_custpane)[row * 3 + 2]);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[0].xyz, px, py + h, state);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[1].xyz, px + w, py + h, state);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[2].xyz, px, py + h * 2, state);
        dotr_custpane_set_xyz_1cb6b0(&packet.points[3].xyz, px + w, py + h * 2, state);
        id = ExSortPrim_Pipe(id, (int)packet.tag, 0, 0);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, -1, 5, x + 0x1EB, y + 0x25 + row * 16, -0x54, -0x10, 1);
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 0x215, y + 0x27 + row * 16, 0, 2, 2, count);
    }
    id = lSzDuel_DrawWindow_ExBoard_Pipe(id, player, x + 6, y + 0x26, -0x4E, -0x9E, -1, 0);
    py = y + 0x76E;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a62_custpane, x + 0x6CE, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    py = y + 0x77E;
    ret = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a58_custpane, x + 0x6CE, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    tx = x + 0x20;
    ty = y + 0x58;
    ret = SzDuel_DrawWindow_Font_Pipe(ret, tx, ty, 1, 0, func_0019bef0((char *)text1), text1,
                       hint != 0 ? (int)data_002be944_custpane : 0);
    py = y + 0x7B7;
    ret = lSzDuel_DrawWindow_ExIcon_Pipe(ret, &data_00290a6c_custpane, x + 0x6CE, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    tx = x + 0x20;
    ty = y + 0x99;
    return SzDuel_DrawWindow_Font_Pipe(ret, tx, ty, 1, 0, func_0019bef0((char *)text2), text2,
                         hint != 0 ? (int)data_002be944_custpane : 0);
}
