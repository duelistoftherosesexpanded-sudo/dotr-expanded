#include "dotr/ee/deck_list_pipe.h"

/* lSzDuel_DrawBuildDeck_Deck_Pipe (0x1bc350): the build-deck screen's deck panel.
 * Compiled with `#pragma opt_propagation off` (docs/DECOMPILATION.md): the original keeps copies
 * the compiler would otherwise propagate. Text and rectangle drawing are two static helpers.
 * MATCHING DEVICE: the casts of the packet address through a word. */

static inline DotrDeckListWord text_001BC350(DotrDeckListWord cursor, int x, int y, int align,
                                             int flags, signed char *text, void *color)
{
    int length = func_0019bef0((char *)text);
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, align, flags,
                                       length, text, color);
}

static inline DotrDeckListWord rectangle_001BC350(DotrDeckListWord cursor, struct DotrDeckListPacket *packet,
                                                  int x, int y, unsigned char *data)
{
    ((struct DotrDeckListHeader *)(DotrDeckListWord)&packet->header)->count = 6;
    ((struct DotrDeckListHeader *)(DotrDeckListWord)&packet->header)->type = 3;
    ((struct DotrDeckListHeader *)(DotrDeckListWord)&packet->header)->registers = 0x441;
    ((struct DotrDeckListHeader *)(DotrDeckListWord)&packet->header)->pad = 0;
    packet->r = 0xA0;
    packet->g = 0xA0;
    packet->b = 0xA0;
    packet->a = 0x80;
    packet->q = 1.0f;
    packet->first.x = (x + 0x6CA) << 4;
    packet->first.y = (y + 0x738) << 4;
    packet->first.z = *(int *)(data + 0xC58);
    packet->first.fog = 0;
    packet->second.x = (x + 0x6F2) << 4;
    packet->second.y = (y + 0x739) << 4;
    packet->second.z = *(int *)(data + 0xC58);
    packet->second.fog = 0;
    return ExSortPrim_Pipe(cursor, (DotrDeckListWord)&packet->header, 0, 0);
}

#ifdef __MWERKS__
#pragma opt_propagation off
#endif
void lSzDuel_DrawBuildDeck_Deck_Pipe(DotrDeckListWord arg0, int arg1, int arg2)
{
    unsigned char *data;
    int deckX;
    unsigned char *color;
    int baseX;
    DotrDeckListWord cursor;
    int leader;
    char *deckText;
    signed char *sortText;
    int state;
    int flags;
    int special;
    char *actionText;
    struct DotrDeckListPacket packet;
    int sortIndex;
    int deckY;
    unsigned char *sortData;
    int sortY;
    int textX;
    int textY;
    DotrDeckListWord sortCursor;
    int cardX;
    int cardY;
    int cardIndex;
    int barX;
    int barY;
    int pageX;
    int pageY;
    DotrDeckListWord actionCursor;
    int actionX;
    int actionY;
    DotrDeckListWord deckCursor;
    int selection;
    int enabled;
    int sortX;
    int style;

    leader = DeckBuildingSide_decklist;
    data = data_00317490_decklist;
    special = ((BuildDeckHorizontalCursorIndex_decklist == 4) || (data[0xD5D] == 5)) ? 1 : 0;
    enabled = 0;
    if (special != 0) {
        if (SzPad_Check(0, 1, 0x100) != 0) {
            enabled = 1;
        }
    }
    selection = enabled != 0 ? 1 : 0;
    color = 0;
    if (selection != 0) {
        color = data_002be8a4_decklist;
    } else {
        color = 0;
    }
    deckText = data_002befab_decklist;
    if (data[0xD5D] == 5) {
        actionText = data_002bef93_decklist;
    } else {
        actionText = data_002bef98_decklist;
    }
    sortText = data_00290fd0_decklist[data[0xCB9 + selection]];
    state = SzData_GetCurrentDeck(leader);
    baseX = arg1 + 0x280;
    flags = 3;
    arg0 = lSzDuel_DrawWindow_Frame_Pipe(arg0, -1, -1, 11, baseX - 0x50, arg2 + 0x10, -0x40, -0x44, 1);
    cursor = lSzDuel_DrawWindow_PicMini_Pipe(arg0, leader, baseX - 0x4B, arg2 + 0x15, 0x36, 0x3A,
                                             *(int *)(data + 0xC58), 1);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, -1, -1, 4, baseX - 0x4B, arg2 + 0x15, -0x36, -0x3A, 1);
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, baseX - 0x50, arg2 + 0x54, -0x40, -0x1C, -1, 0);
    cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290828_decklist, baseX + 0x674, arg2 + 0x77C,
                                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    if (special != 0) {
        cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290aa8_decklist, baseX + 0x695, arg2 + 0x77C,
                                                0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, baseX - 0x140, arg2 + 0x40, -0xF0, -0x30, -1, 0);
    cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290896_decklist, baseX + 0x58E, arg2 + 0x765,
                                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    if (special != 0) {
        cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290abc_decklist, baseX + 0x587, arg2 + 0x77E,
                                                0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    sortX = baseX + 0x5A8;
    sortY = arg2 + 0x769;
    sortData = data + selection;
    sortIndex = 0;
    do {
        style = -1;
        if (sortData[0xCB9] == sortIndex) {
            style = selection != 0 ? 2 : 1;
        }
        cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, gSzDuel_Icon_decklist + (sortIndex + 0xD1) * 10, sortX,
                                                sortY, 0, 1, 0, 0, 0, 0, 1, color, style, 0x80);
        sortIndex++;
        sortX += 0x10;
    } while (sortIndex < 10);
    cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_002908a0_decklist, baseX + 0x64A, arg2 + 0x765,
                                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    sortCursor = cursor;
    if (special != 0) {
        sortCursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290ac6_decklist, baseX + 0x653, arg2 + 0x77E,
                                                    0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    textX = baseX - 0xC8;
    textY = arg2 + 0x5E;
    if (color != 0) {
        flags |= 8;
    }
    cursor = text_001BC350(sortCursor, textX, textY, 0, flags, sortText, color);
    cardX = baseX - 0x138;
    cardY = arg2 + 0x70;
    cardIndex = 0;
    do {
        int first;
        first = 0;
        if (*(unsigned short *)(data + 0xC8E) == 0) {
            if (cardIndex == 0) {
                first = 1;
            }
        }
        cursor = (DotrDeckListWord)lSzDuel_DrawBuildDeck_Card_Pipe(
            (void *)cursor, (*(unsigned short **)(data + 0x6DC))[*(unsigned short *)(data + 0xC8E) + cardIndex],
            cardX, cardY, first != 0 ? 1 : 0, 0);
        cardIndex++;
        cardY += 0x36;
    } while (cardIndex < 5);
    barX = baseX - 0x138;
    barY = arg2 + 0x17E;
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, barX, barY, -0x3C, -0x32, -1, 0);
    cursor = rectangle_001BC350(cursor, &packet, barX, barY, data);
    pageX = baseX - 0x11A;
    pageY = arg2 + 0x186;
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, pageX, pageY, 1, 2, 2,
                                              *(unsigned short *)(data + 0xC8E) + data[0xD5F] + 1);
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, pageX, pageY + 0x17, 1, 2, 2, *(unsigned short *)(data + 0xC90));
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, baseX - 0xFC, arg2 + 0x17E, -0xEC, -0x32, -1, 0);
    cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290832_decklist, baseX + 0x674, arg2 + 0x8A2,
                                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    deckCursor = cursor;
    if (special != 0) {
        deckCursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290ab2_decklist, baseX + 0x695, arg2 + 0x8A7,
                                                    0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    if (special != 0) {
        actionCursor = lSzDuel_DrawWindow_ExIcon_Pipe(deckCursor, data_00290a6c_decklist, baseX + 0x699,
                                                      arg2 + 0x8BA, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        actionX = baseX - 0x4F;
        actionY = arg2 + 0x19D;
        cursor = text_001BC350(actionCursor, actionX, actionY, 1, 0, (signed char *)actionText, 0);
        deckCursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290a76_decklist, baseX + 0x5D8, arg2 + 0x8A2,
                                                    0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    }
    deckX = baseX - 0xCE;
    deckY = arg2 + 0x184;
    cursor = text_001BC350(deckCursor, deckX, deckY, 1, 0, (signed char *)deckText, 0);
    cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_0029095e_decklist, baseX + 0x5D8, arg2 + 0x8B4,
                                            0, 1, 0, 0, 0, 0, state == 0 ? 1 : 0, 0, -1, 0x80);
    {
        int x = baseX + 0x608;
        int y = arg2 + 0x8B4;
        cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290968_decklist, x, y, 0, 1, 0, 0, 0, 0,
                                                state == 1 ? 1 : 0, 0, -1, 0x80);
    }
    {
        int x = baseX + 0x638;
        int y = arg2 + 0x8B4;
        lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290972_decklist, x, y, 0, 1, 0, 0, 0, 0,
                                       state == 2 ? 1 : 0, 0, -1, 0x80);
    }
}
#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
