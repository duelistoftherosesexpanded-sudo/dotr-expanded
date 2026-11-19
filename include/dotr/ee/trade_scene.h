#ifndef DOTR_EE_TRADE_SCENE_H
#define DOTR_EE_TRADE_SCENE_H

/* SzDuel_DrawTrade_Pipe (0x1c35a0) and the trade-screen draws it calls.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */
#ifdef __MWERKS__
typedef int DotrTradeWord;
#else
#include <stdint.h>
typedef intptr_t DotrTradeWord;
#endif

/* View of the control block at 0x317490 as SzDuel_DrawTrade_Pipe reads it; index 0 is the
 * left side, 1 the right. Member meanings are assumed from the uses: a card list per side,
 * the first shown row and row count per side, a slide amount per side (non-zero while the
 * detail panel is shown), a mode byte per side, the side being drawn and the cursor rows. */
struct DotrTradeControl {
    unsigned char unknown_000[0x6d8];
    unsigned short *list[2];
    unsigned char unknown_6e0[0x5aa];
    unsigned short top0;
    unsigned short count0;
    unsigned short top1;
    unsigned short count1;
    short slide[2];
    unsigned char unknown_c96[0x28];
    unsigned char mode[2];
    unsigned char unknown_cc0[0x9c];
    unsigned char side;
    unsigned char unknown_d5d;
    unsigned char cursor[2];
};
extern struct DotrTradeControl data_00317490;
/* +0xc8c of the block above (count0), read through its own symbol; a separate host object. */
extern unsigned short data_0031811c;

/* Callees. Signatures are assumed from this call site: arguments are the registers set. */
extern DotrTradeWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrTradeWord cursor, int a);
extern DotrTradeWord lSzDuel_DrawTrade_Base_Pipe(DotrTradeWord cursor);
extern DotrTradeWord lSzDuel_DrawTrade_Title_Pipe(DotrTradeWord cursor, int a, int b);
extern DotrTradeWord lSzDuel_DrawTrade_Bag_Pipe(DotrTradeWord cursor, int side, int a, int b);
extern DotrTradeWord lSzDuel_DrawTrade_Cursor_Pipe(DotrTradeWord cursor, int side, int a, int b, int row);
extern DotrTradeWord lSzDuel_Draw_Fade_Pipe(DotrTradeWord cursor, int x, int y, int width, int height, int alpha);
extern DotrTradeWord lSzDuel_DrawBuildDeck_Detail_Pipe(DotrTradeWord cursor, int card, int x, int a, int side, int b, int c,
                                                       int d);
extern DotrTradeWord lSzDuel_DrawTrade_Ready_Pipe(DotrTradeWord cursor, int x, int a);
extern DotrTradeWord lSzDuel_DrawTrade_Exit_Pipe(DotrTradeWord cursor, int a, int b);
extern DotrTradeWord lSzDuel_DrawTrade_Do_Pipe(DotrTradeWord cursor, int a, int b);
extern DotrTradeWord lSzDuel_DrawTrade_Notice_Pipe(DotrTradeWord cursor, int a, int b);
extern void SzMsg_Free(int index);

DotrTradeWord SzDuel_DrawTrade_Pipe(DotrTradeWord cursor);

#endif
