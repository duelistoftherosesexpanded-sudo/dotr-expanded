#ifndef DOTR_EE_DECK_LIST_PIPE_H
#define DOTR_EE_DECK_LIST_PIPE_H

/* The build-deck screen's deck panel: leader picture, sort tabs, five card rows, page counter and
 * deck name.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The packet cursor the draw functions pass along is a 32-bit word in the game; a host
 * pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrDeckListWord;
#else
typedef long DotrDeckListWord;
#endif

/* A 64-bit GS vertex word: x and y (12.4 fixed point), z and fog. The game's code uses 64-bit
 * bit-fields; on the host the same little-endian layout is spelled with 32-bit units (64-bit
 * bit-fields are an extension there). */
#ifdef __MWERKS__
typedef unsigned long long DotrDeckListBits;
#else
typedef unsigned int DotrDeckListBits;
#endif
struct DotrDeckListVertex {
    DotrDeckListBits x : 16;
    DotrDeckListBits y : 16;
    DotrDeckListBits z : 24;
    DotrDeckListBits fog : 8;
};

/* The packet header ExSortPrim_Pipe takes: primitive type, word count, a pad word and the
 * GS register list. Assumed: field meanings from the values stored here (3, 6, 0x441). */
struct DotrDeckListHeader {
    unsigned short type;
    unsigned short count;
    unsigned int pad;
    unsigned long long registers;
};

/* The grey bar packet built on the stack: header, colour, q and two vertices. */
struct DotrDeckListPacket {
    struct DotrDeckListHeader header;
    unsigned char r, g, b, a;
    float q;
    struct DotrDeckListVertex first;
    struct DotrDeckListVertex second;
};

/* The duel control block at 0x317490, read here as bytes: +0x6dc the card list (a pointer
 * word), +0xc58 the draw depth word, +0xc8e the first row shown, +0xc90 the card count,
 * +0xcb9 two sort indexes, +0xd5d a mode byte (5: the second action text), +0xd5f a page
 * offset. Assumed: field meanings from their use here. */
extern unsigned char data_00317490_decklist[0xd70];
/* 0x3181ec: the current leader (side) byte; 0x3181ed: a mode byte (4 enables the second
 * set of icons). */
extern unsigned char DeckBuildingSide_decklist;
extern unsigned char BuildDeckHorizontalCursorIndex_decklist;
/* Icon definitions (10 bytes each) drawn by lSzDuel_DrawWindow_ExIcon_Pipe; the sort tabs are
 * entries 0xd1..0xda from 0x290080. */
extern unsigned char gSzDuel_Icon_decklist[];
extern unsigned char data_00290828_decklist[];
extern unsigned char data_00290832_decklist[];
extern unsigned char data_00290896_decklist[];
extern unsigned char data_002908a0_decklist[];
extern unsigned char data_0029095e_decklist[];
extern unsigned char data_00290968_decklist[];
extern unsigned char data_00290972_decklist[];
extern unsigned char data_00290a6c_decklist[];
extern unsigned char data_00290a76_decklist[];
extern unsigned char data_00290aa8_decklist[];
extern unsigned char data_00290ab2_decklist[];
extern unsigned char data_00290abc_decklist[];
extern unsigned char data_00290ac6_decklist[];
/* The sort names, indexed by sort index. */
extern signed char *data_00290fd0_decklist[];
/* The highlight colour used while the second set is held. */
extern unsigned char data_002be8a4_decklist[];
/* Texts: the deck name text and the two action texts. */
extern char data_002bef93_decklist[];
extern char data_002bef98_decklist[];
extern char data_002befab_decklist[];

/* Callees (names from the Japanese build's symbols). lSzDuel_DrawWindow_Frame_Pipe takes its ninth
 * and lSzDuel_DrawWindow_PicMini_Pipe its seventh argument as 64-bit. Assumed: func_0019bef0 is a
 * string length; SzPad_Check (port, kind, mask) non-zero on a matching button;
 * SzData_GetCurrentDeck returns 0..2 for the deck shown. */
extern int SzPad_Check(int port, int kind, int mask);
extern int SzData_GetCurrentDeck(int side);
extern int func_0019bef0(const char *s);
extern DotrDeckListWord lSzDuel_DrawWindow_Frame_Pipe(DotrDeckListWord cursor, int a1, int a2, int a3, int x, int y,
                                                     int w, int h, long long a8);
extern DotrDeckListWord lSzDuel_DrawWindow_PicMini_Pipe(DotrDeckListWord cursor, int id, int x, int y, int w, int h,
                                                       long long z, int bright);
extern DotrDeckListWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrDeckListWord cursor, int colour, int x, int y, int w,
                                                       int h, int back, int kind);
extern DotrDeckListWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrDeckListWord cursor, unsigned char *def, int x, int y,
                                                      int background, int image, int border, int shrink, int flipX,
                                                      int flipY, int bright, unsigned char *rgb, int blend,
                                                      unsigned char alpha);
extern DotrDeckListWord SzDuel_DrawWindow_Font_Pipe(DotrDeckListWord cursor, int x, int y, int align, int flags,
                                                   int n, signed char *text, void *color);
extern DotrDeckListWord SzDuel_DrawWindow_FontDigit_Pipe(DotrDeckListWord cursor, int x, int y, int a, int flags,
                                                        int width, int value);
extern void *lSzDuel_DrawBuildDeck_Card_Pipe(void *cursor, int card, int x, int y, int flag, int flag2);
extern DotrDeckListWord ExSortPrim_Pipe(DotrDeckListWord cursor, DotrDeckListWord packet, int a2, int a3);

/* 0x1bc350 lSzDuel_DrawBuildDeck_Deck_Pipe (name from the Japanese build's symbols): draws the
 * deck panel at (x + 0x280, y) into the packet cursor. The final cursor is not returned. */
void lSzDuel_DrawBuildDeck_Deck_Pipe(DotrDeckListWord cursor, int x, int y);

#endif
