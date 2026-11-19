#ifndef DOTR_EE_BUILD_DECK_CARD_H
#define DOTR_EE_BUILD_DECK_CARD_H

/* The build-deck screen's card panel drawer: one card's frame, icons, numbers and name drawn into
 * the packet cursor.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The packet cursor the draw functions pass along (and text addresses passed as integers)
 * are 32-bit words in the game; a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrBuildCardWord;
#else
typedef long DotrBuildCardWord;
#endif

/* The packet header ExSortPrim_Pipe takes: count, flags, a pad word and the GS register list.
 * Assumed: field meanings from the values stored here. */
struct DotrBuildCardTag {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
};

struct DotrBuildCardColor {
    unsigned char r, g, b, a;
    float q;
};

/* A 64-bit GS vertex word: x and y (12.4 fixed point), z and fog. The game's code uses 64-bit
 * bit-fields; on the host the same little-endian layout is spelled with 32-bit units. */
#ifdef __MWERKS__
typedef unsigned long long DotrBuildCardBits;
#else
typedef unsigned int DotrBuildCardBits;
#endif
struct DotrBuildCardVertex {
    DotrBuildCardBits x : 16;
    DotrBuildCardBits y : 16;
    DotrBuildCardBits z : 24;
    DotrBuildCardBits fog : 8;
};

/* The dimming box packet built on the stack: header, colour and q, two vertices. */
struct DotrBuildCardBox {
    unsigned long long tag[2];
    struct DotrBuildCardColor color;
    struct DotrBuildCardVertex vertex0;
    struct DotrBuildCardVertex vertex1;
    unsigned long long end;
};

/* The card constant record SzDuel_GetCardConstInfo returns, as this drawer reads it: kind in
 * the top 3 bits of +0 (attribute/type in the low 5), the attribute and level nibbles at +2,
 * the star (cost) byte at +3, a link word at +4 (0xffff none), attack and defense in the low
 * 13 bits of +8 and +10. Assumed: field meanings from their use here. */
struct DotrBuildCardCard {
    unsigned char kind;
    unsigned char pad1;
    unsigned char attribute : 4;
    unsigned char level : 4;
    unsigned char stars;
    unsigned short link;
    unsigned char pad6[2];
    unsigned short attack;
    unsigned short defense;
};

/* The duel control block at 0x317490, read here as bytes: +0xc58 the draw depth word. */
extern unsigned char data_00317490_buildcard[0xd70];
/* 0x3181ec: the current leader (side) byte. */
extern unsigned char DeckBuildingSide_buildcard;
/* Icon definitions (10 bytes each) drawn by lSzDuel_DrawWindow_ExIcon_Pipe: the kind icons
 * from 0x290080 (also entries 0xe3 + deck), the attribute icons from 0x290020 and single
 * definitions; the single definitions are declared as words. */
extern unsigned char gSzDuel_Icon_buildcard[];
extern unsigned char data_00290020_buildcard[];
extern int data_00290a12_buildcard;
extern int data_002908e6_buildcard;
extern int data_002908be_buildcard;
extern int data_002908c8_buildcard;
extern int data_002908f0_buildcard;
extern int data_0029086e_buildcard;
extern int data_00290878_buildcard;
extern int data_0029097c_buildcard;
extern int data_00290986_buildcard;
extern int data_00290990_buildcard;
/* A text drawn in the deck-count mode, and three colours (passed as words). */
extern signed char data_002befb7_buildcard[];
extern char gSzDuel_aicolMsg_buildcard[];
extern char data_002be944_buildcard[];
extern char data_002be948_buildcard[];

/* Callees (names from the Japanese build's symbols); the cursor and words that carry addresses are
 * DotrBuildCardWord. Assumed: func_0019bef0 is a string length; SzDuel_GetCardConstInfo returns
 * null for no card. SzMsg_DrawRect_Pipe is passed by address only. */
extern unsigned char *SzDuel_GetCardConstInfo(int cardNo);
extern int SzData_GetCurrentDeck(int side);
extern int SzDuel_Kind2Type(int kind);
extern int SzDuel_Type2Attr(int type, int attribute);
extern int SzDuel_GetMsgName(int cardNo);
extern int SzData_IsBagNew(int side, int cardNo);
extern int SzData_GetRank(int side, int cardNo);
extern int SzData_GetBagCount(int side, int cardNo, int kind);
extern int func_0019bef0(const char *s);
extern int *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern int ExAssignAlpha(void *packet, long long alpha);
extern long long ExGetAlphaVal(int index);
extern DotrBuildCardWord SzMsg_LoadFont_Pipe(DotrBuildCardWord cursor, int msgInfoIndex, int stringIndex, int a3, int a4);
extern DotrBuildCardWord RhText_SetTextureFlush(DotrBuildCardWord cursor);
extern DotrBuildCardWord SzMsg_DrawRatioX_Pipe(DotrBuildCardWord cursor, int index, int x, int y, float sx, float sy,
                                               DotrBuildCardWord color, void (*function)(void));
extern DotrBuildCardWord lSzDuel_DrawWindow_Frame_Pipe(DotrBuildCardWord cursor, int a1, int a2, int a3, int x, int y,
                                                       int w, int h, int a8);
extern DotrBuildCardWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrBuildCardWord cursor, void *def, int x, int y, int a4,
                                                        int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                                                        int a12, int a13);
extern DotrBuildCardWord SzDuel_DrawWindow_FontDigit_Pipe(DotrBuildCardWord cursor, int x, int y, int a, int flags,
                                                          int width, int value);
extern DotrBuildCardWord lSzDuel_DrawWindow_Emblem_Pipe(DotrBuildCardWord cursor, int x, int y, int rank, int a4,
                                                        int a5);
extern DotrBuildCardWord SzDuel_DrawWindow_Font_Pipe(DotrBuildCardWord cursor, int x, int y, int a, int flags, int n,
                                                     signed char *text, DotrBuildCardWord color);
extern DotrBuildCardWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrBuildCardWord cursor, int colour, int x, int y, int w,
                                                         int h, int a6, int a7);
extern DotrBuildCardWord ExSortPrim_Pipe(DotrBuildCardWord cursor, DotrBuildCardWord packet, int a2, int a3);

/* 0x1bce20 lSzDuel_DrawBuildDeck_Card_Pipe (name from the Japanese build's symbols): draws
 * card `card`'s panel at (x, y) into the packet cursor and returns the new cursor. flag selects
 * the deck-count layout; flag2 dims the panel when the card's bag count of kind 3 is 0 or less. */
void *lSzDuel_DrawBuildDeck_Card_Pipe(void *cursor, int card, int x, int y, int flag, int flag2);

#endif
