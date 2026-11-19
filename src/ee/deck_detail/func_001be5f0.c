#include "dotr/ee/deck_detail.h"

/* 0x1be5f0. The callees are declared inside the function body. Compiled with #pragma
 * opt_propagation off, set directly before the function and reset after it. */
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
/* The header shows this prototype to the target compiler only (other functions declare the
 * callee inside their bodies with another type); this function calls it through that prototype,
 * so the host build declares it here. */
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet, unsigned short *src, int x, int y, int background, int image, int border, int shrink, int flipX, int flipY, int bright, unsigned char *rgb, int blend, unsigned char alpha);
#endif
#ifdef __MWERKS__
#pragma opt_propagation off
#endif
int lSzDuel_DrawBuildDeck_Detail_Basic_Pipe(int arg0, int cardNo, int arg2, int arg3, int arg4) {
    extern int lSzDuel_DrawWindow_ExBoard_Pipe();
    extern int lSzDuel_DrawWindow_PicItem_Pipe();
    extern int SzMsg_DrawX_Pipe();
    extern int SzDuel_DrawWindow_FontDigit_Pipe();
    extern int *SzMsg_GetInfo(int);
    extern int SzData_GetRank();
    extern int SzDuel_Type2Attr();
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void (*)(void));
    extern char gSzDuel_aicolMsg_deckpane[];
    extern unsigned char data_002901b6_deckpane[];
    extern unsigned char data_002908f0_deckpane[];
    int pictureX;
    int pictureY;
    unsigned char *data;
    DotrDeckPaneCard1be5f0 *card;
    int palette;
    int attribute;
    int message;
    int cardType;
    int player;
    unsigned char *name;
    int cardId;
    int deckIndex;
    int subtype;
    signed char *loadingText;
    int slide;
    int nameCtx;
    int nameX;
    int cursor;
    signed char text[0x58];
    DotrDeckPanePacket1be5f0 packet __attribute__((aligned(8)));
    DotrDeckPaneColor1be5f0 color;
    float scale;
    int labelY;
    int textX;
    int textY;
    int labelX;
    int boxX;
    int numberX;
    int i;
    int baseX;
    int width;
    int big;
    int c;
    int x;
    int y;
    int stars;

    data = data_00317490_deckpane.b;
    player = DeckBuildingSide_deckpane;
    card = (DotrDeckPaneCard1be5f0 *)SzDuel_GetCardConstInfo(cardNo);
    slide = arg4 ? 0x280 : -0x140;
    deckIndex = -1;
    if (cardNo == 0x29F) {
        name = data_0029008a_deckpane;
        message = 0x26;
        cardType = 0;
        deckIndex = 0;
        palette = 0;
        subtype = 1;
    } else {
        if (card == NULL) {
            return arg0;
        }
        cardType = card->data.kind >> 5;
        attribute = card->data.kind & 0x1F;
        if (cardType == 0) { /* 0: monster */
            deckIndex = SzData_GetRank(player, cardNo);
            name = gSzDuel_Icon_deckpane + attribute * 10;
            message = attribute + 0x25;
        } else {
            name = gSzDuel_Icon_deckpane + (cardType + 0x14) * 10;
            message = cardType + 0x1E;
        }
        palette = SzDuel_Kind2Type(cardType);
        subtype = SzDuel_Type2Attr(palette, card->data.stats & 0xF);
        if (palette == 0 && card->data.code != 0xFFFF) {
            palette = 4;
        }
    }
    arg0 = SzMsg_LoadFont_Pipe(arg0, 1, 0x9A, 0, 0x16);
    cursor = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(
          SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(
          SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(arg0, 2, message, 0, 0x2C),
          3, 0x9B, 0, 0x42), 4, subtype + 0x3B, 0, 0x58),
          5, 0x9C, 0, 0x6E), 6, 0x9D, 0, 0x84), 7, 0x9E, 0, 0x9A),
          8, 0xA0, 0, 0xB0), 9, deckIndex + 0x52, 0, 0xC6), 10, 0x9F, 0, 0xDC));
    pictureY = arg3 + 0x57;
    baseX = arg2 + slide;
    pictureX = baseX + 0x9D;
    cardId = cardNo == 0x29F ? cardNo : SzData_ExchSpecial(player, cardNo);
    if (SzText_GetReadyItem(1, cardId) != 0) {
        cursor = lSzDuel_DrawWindow_PicItem_Pipe(cursor, cardId, pictureX, pictureY, 0x80, 0x80,
                            *(int *)(data + 0xC58));
    } else {
        loadingText = data_002befcb_deckpane;
        pictureX += 0x6C0;
        color = data_00291368_deckpane;
        color.bytes[3] = data_00318118_deckpane >> 8;
        pictureY += 0x720;
        cursor = dotr_deckpane_loading_1be5f0(cursor, &packet, pictureX, pictureY, data);
        {
            int c = cursor;
            int x = baseX + 0xDD;
            int y = arg3 + 0x87;
            cursor = dotr_deckpane_text_1be5f0(c, x, y, 0, 2, loadingText, &color);
        }
    }
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, -1, -1, 11, baseX + 0x98, arg3 + 0x52, -0x8A, -0x8A, 1);
    textX = baseX + 0xDD;
    textY = arg3 + 0xDE;
    func_0019b648(text, data_002befdb_deckpane, cardNo);
    cursor = dotr_deckpane_text_1be5f0(cursor, textX, textY, 1, 2, text, NULL);
    labelX = baseX + 0x1D;
    cursor = SzMsg_DrawX_Pipe(cursor, 1, labelX, arg3 + 0x35, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
    x = boxX = baseX + 0x4E;
    y = arg3 + 0x30;
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, x, y, -0xD4, -0x20, -1, 0);
    nameCtx = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, name, dotr_deckpane_pos_1be5f0(arg2 + 0x712, slide), arg3 + 0x754,
                           0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    nameX = baseX + 0x73;
    labelY = arg3 + 0x34;
    width = SzMsg_GetInfo(2)[3];
    big = width > 172;
    if (big) {
        scale = 172.0f / width;
    } else {
        scale = 1.0f;
    }
    cursor = SzMsg_DrawRatioX_Pipe(nameCtx, 2, nameX, labelY, scale, 1.0f, (int)data_002be94c_deckpane, SzMsg_DrawRect_Pipe);
    if (cardType == 0) {
        cursor = SzMsg_DrawX_Pipe(cursor, 3, labelX, arg3 + 0x56, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
        y = arg3 + 0x51;
        cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, boxX, y, -0x48, -0x20, -1, 0);
        cursor = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_00290020_deckpane + subtype * 10, dotr_deckpane_pos_1be5f0(arg2 + 0x726, slide),
                           arg3 + 0x775, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        cursor = SzMsg_DrawX_Pipe(cursor, 5, labelX, arg3 + 0x77, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
        y = arg3 + 0x72;
        cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, boxX, y, -0x48, -0x20, -1, 0);
        x = numberX = baseX + 0x4F;
        y = arg3 + 0x77;
        if (card != NULL) {
            cursor = SzDuel_DrawWindow_ExDigit_Pipe(cursor, x, y, 4, card->data.attack & 0x1FFF, 1, 0, 0);
        }
        cursor = SzMsg_DrawX_Pipe(cursor, 6, labelX, arg3 + 0x98, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
        y = arg3 + 0x93;
        cursor = c = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, boxX, y, -0x48, -0x20, -1, 0);
        y = arg3 + 0x98;
        if (card != NULL) {
            cursor = SzDuel_DrawWindow_ExDigit_Pipe(c, numberX, y, 4, card->data.defense & 0x1FFF, 1, 0, 0);
        }
        labelY = arg3 + 0xB3;
        cursor = SzMsg_DrawRatioX_Pipe(cursor, 7, labelX, labelY, dotr_deckpane_fit_1be5f0(7, 116), 1.0f, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
        x = baseX + 0x1A;
        y = arg3 + 0xC9;
        stars = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, x, y, -0x7C, -0x26, -1, 0);
        y = arg3 + 0x7EC;
        x = dotr_deckpane_pos_1be5f0(arg2 + 0x6DD, slide);
        if (card != NULL) {
            for (i = 0; i < (int)card->bits.level; i++) {
                stars = lSzDuel_DrawWindow_ExIcon_Pipe(stars, data_002901b6_deckpane, x + (i % 6) * 16,
                                   y + (i / 6) * 16, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
            }
        }
        x = baseX + 0x87;
        y = arg3 + 0xD6;
        if (card != NULL) {
            stars = SzDuel_DrawWindow_FontDigit_Pipe(stars, x, y, 1, 2, 2, card->bits.level);
        }
        labelY = arg3 + 0xF4;
        cursor = SzMsg_DrawRatioX_Pipe(stars, 8, labelX, labelY, dotr_deckpane_fit_1be5f0(8, 116), 1.0f, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
        cursor = lSzDuel_DrawWindow_Emblem_Pipe(cursor, baseX + 0x93, arg3 + 0xEF, deckIndex, 0, 0);
        x = baseX + 0xB4;
        y = arg3 + 0xEF;
        cursor = c = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, x, y, -0x6E, -0x20, -1, 0);
        x = baseX + 0xBF;
        y = arg3 + 0xF5;
        if (card != NULL) {
            cursor = SzMsg_DrawX_Pipe(c, 9, x, y, (int)data_002be94c_deckpane, SzMsg_DrawRect_Pipe);
        }
    }
    labelY = arg3 + 0x115;
    cursor = SzMsg_DrawRatioX_Pipe(cursor, 10, labelX, labelY, dotr_deckpane_fit_1be5f0(10, 116), 1.0f, (int)gSzDuel_aicolMsg_deckpane, SzMsg_DrawRect_Pipe);
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, palette + 7, baseX + 0xB4, arg3 + 0x110, -0x6E, -0x20, -1, 0);
    cursor = c = lSzDuel_DrawWindow_ExIcon_Pipe(cursor, data_002908f0_deckpane, dotr_deckpane_pos_1be5f0(arg2 + 0x77B, slide), arg3 + 0x838,
                       0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = baseX + 0xDA;
    y = arg3 + 0x116;
    if (card != NULL) {
        cursor = SzDuel_DrawWindow_ExDigit_Pipe(c, x, y, 3, card->data.cost, 1, 0, 0);
    }
    SzMsg_Free(1);
    SzMsg_Free(2);
    SzMsg_Free(3);
    SzMsg_Free(4);
    SzMsg_Free(5);
    SzMsg_Free(6);
    SzMsg_Free(7);
    SzMsg_Free(8);
    SzMsg_Free(9);
    SzMsg_Free(10);
    return cursor;
}
