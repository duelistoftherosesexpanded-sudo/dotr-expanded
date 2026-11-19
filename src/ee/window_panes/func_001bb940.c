#include "dotr/ee/window_panes.h"

#ifndef __MWERKS__
/* Host build only: the two unused locals (numX, numY) are kept. */
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#ifdef __MWERKS__
#pragma opt_propagation off
#endif
int lSzDuel_DrawBuildDeck_Bag_Pipe(int arg0, int arg1, int arg2)
{
    extern int lSzDuel_DrawWindow_ExBoard_Pipe();
    extern int SzDuel_DrawWindow_FontDigit_Pipe();
    extern int SzDuel_DrawWindow_Font_Pipe();
    extern unsigned char data_0029086e_panes[];
    extern char data_002bef9d_panes[];
    unsigned char *data;
    int player;
    unsigned char *color;
    signed char *sortText;
    int special;
    char *titleText;
    char *descText;
    int cursor;
    int sortIndex;
    int selection;
    DotrPaneBagPacket packet;
    DotrPaneBagColor shine;
    int enabled;
    unsigned char *sortData;
    int sortX;
    int sortY;
    int numY;
    int style;
    int sortCursor;
    int flags;
    int textX;
    int textY;
    int numX;
    int cardX;
    int cardY;
    int cardIndex;
    int deckCursor;
    int actionCursor;
    int eligible;
    int barX;
    int barY;
    int pageX;
    int pageY;

    data = data_00317490_panes.b;
    player = DeckBuildingSide_panes;
    special = ((BuildDeckHorizontalCursorIndex_panes == 3) || (data[0xD5D] == 2)) ? 1 : 0;
    enabled = 0;
    if (special != 0) {
        if (SzPad_Check(0, 1, 0x100) != 0) {
            enabled = 1;
        }
    }
    selection = enabled != 0 ? 1 : 0;
    color = 0;
    if (selection != 0) {
        color = data_002be8a4_panes;
    } else {
        color = 0;
    }
    titleText = data_002bef8d_panes;
    if (data[0xD5D] == 2) {
        descText = data_002bef93_panes;
    } else {
        descText = data_002bef98_panes;
    }
    sortText = data_00290fd0_panes[data[0xCB6 + selection]];
    textX = arg1 + 0x10;
    textY = arg2 + 0x10;
    arg0 = lSzDuel_DrawWindow_Frame_Pipe(arg0, 0xC, -1, 11, textX, textY, -0xC0, -0x30, 1);
    textX = arg1 + 0x70;
    textY = arg2 + 0x1F;
    cursor = dotr_pane_bag_text(arg0, textX, textY, 2, 3, (signed char *)titleText, 0);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, 0xC, -1, 5, arg1 + 0x10, arg2 + 0x40, -0x40, -0x14, 1);
    cursor = func_001b3cc0_bag(cursor, data_0029086e_panes, arg1 + 0x6D2, arg2 + 0x762,
                        0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    {
        int c = cursor;
        int x = arg1 + 0x27;
        int y = arg2 + 0x44;
        cursor = dotr_pane_bag_number(c, x, y, player);
    }
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, arg1 + 0x10, arg2 + 0x54, -0x40, -0x1C, -1, 0);
    if (special != 0) {
        cursor = func_001b3cc0_bag(cursor, data_00290aa8_panes, arg1 + 0x6D5, arg2 + 0x77D,
                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    cursor = func_001b3cc0_bag(cursor, data_00290828_panes, arg1 + 0x6EC, arg2 + 0x77C,
                        0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, arg1 + 0x50, arg2 + 0x40, -0xF0, -0x30, -1, 0);
    cursor = func_001b3cc0_bag(cursor, data_00290896_panes, arg1 + 0x71E, arg2 + 0x765,
                        0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    if (special != 0) {
        cursor = func_001b3cc0_bag(cursor, data_00290abc_panes, arg1 + 0x717, arg2 + 0x77E,
                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    sortX = arg1 + 0x738;
    sortY = arg2 + 0x769;
    sortData = data + selection;
    sortIndex = 0;
    do {
        style = -1;
        if (sortData[0xCB6] == sortIndex) {
            style = selection != 0 ? 2 : 1;
        }
        cursor = func_001b3cc0_bag(cursor, gSzDuel_Icon_panes + (sortIndex + 0xD1) * 10, sortX, sortY,
                            0, 1, 0, 0, 0, 0, 1, color, style, 0x80);
        sortIndex++;
        sortX += 0x10;
    } while (sortIndex < 10);
    cursor = func_001b3cc0_bag(cursor, data_002908a0_panes, arg1 + 0x7DA, arg2 + 0x765,
                        0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    sortCursor = cursor;
    if (special != 0) {
        sortCursor = func_001b3cc0_bag(cursor, data_00290ac6_panes, arg1 + 0x7E3, arg2 + 0x77E,
                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    textX = arg1 + 0xC8;
    textY = arg2 + 0x5E;
    flags = func_0019bef0((char *)sortText);
    cursor = SzDuel_DrawWindow_Font_Pipe(sortCursor, textX, textY, 0, (color != 0 ? 8 : 0) | 3, flags, sortText, color);
    cardX = arg1 + 8;
    cardY = arg2 + 0x70;
    cardIndex = 0;
    do {
        cursor = (DotrPaneWord) lSzDuel_DrawBuildDeck_Card_Pipe((void *)(DotrPaneWord) cursor, (*(unsigned short **)(data + 0x6D8))[*(unsigned short *)(data + 0xC8A) + cardIndex],
                            cardX, cardY, 0, 1);
        cardIndex++;
        cardY += 0x36;
    } while (cardIndex < 5);
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, arg1 + 0x10, arg2 + 0x17E, -0xEC, -0x32, -1, 0);
    cursor = func_001b3cc0_bag(cursor, data_00290832_panes, arg1 + 0x6EC, arg2 + 0x8A2,
                        0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    deckCursor = cursor;
    if (special != 0) {
        actionCursor = func_001b3cc0_bag(cursor, data_00290ab2_panes, arg1 + 0x6D5, arg2 + 0x8A7,
                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        actionCursor = func_001b3cc0_bag(actionCursor, data_00290a6c_panes, arg1 + 0x6D7, arg2 + 0x8BA,
                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        {
            int c = actionCursor;
            int x = arg1 + 0x2C;
            int y = arg2 + 0x19D;
            deckCursor = dotr_pane_bag_text(c, x, y, 1, 0, (signed char *)descText, 0);
        }
    }
    if (special != 0) {
        eligible = SzData_GetBagCount(player, (*(unsigned short **)(data + 0x6D8))[*(unsigned short *)(data + 0xC8A) + data[0xD5E]], 3);
        if (SzFile_InqRequestEx() != 0) {
            eligible = 0;
        }
        if (Taku_Get_Duel5_flg() < 5) {
            eligible = 0;
        }
        if (!dotr_pane_bag_ready(player, 0) && !dotr_pane_bag_ready(player, 1) && !dotr_pane_bag_ready(player, 2)) {
            eligible = 0;
        }
        if (eligible > 0) {
            shine = data_00291360_panes;
            shine.bytes[3] = data_00318118_panes >> 8;
            titleText = data_002bef9d_panes;
            textX = arg1 + 0xA4;
            textY = arg2 + 0x191;
            deckCursor = dotr_pane_bag_text(deckCursor, textX, textY, 0, 3, (signed char *)titleText, &shine);
        }
    }
    barX = arg1 + 0xFC;
    barY = arg2 + 0x17E;
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(deckCursor, 2, barX, barY, -0x3C, -0x32, -1, 0);
    cursor = dotr_pane_bag_rectangle(cursor, &packet, barX, barY, data);
    pageX = arg1 + 0x11A;
    pageY = arg2 + 0x186;
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, pageX, pageY, 1, 2, 3, *(unsigned short *)(data + 0xC8A) + data[0xD5E] + 1);
    return SzDuel_DrawWindow_FontDigit_Pipe(cursor, pageX, pageY + 0x17, 1, 2, 3, *(unsigned short *)(data + 0xC8C));
}
#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
