#ifndef DOTR_EE_CUSTOM_LIFE_SET_H
#define DOTR_EE_CUSTOM_LIFE_SET_H

/* The custom-duel menu's life-point page drawer: title, life value, gradient gauge and button
 * hints into the packet cursor.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The packet cursor and words that carry addresses are 32-bit words in the game; a host
 * pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrLifeSetWord;
#else
typedef long DotrLifeSetWord;
#endif

/* The custom-duel settings block at the literal address 0xa9c000 (see custom_duel_proc.h):
 * +0x02 per player the menu byte (item in the low nibble), +0x10 per player the life points
 * (signed halfwords), +0x88 the two-player layout byte. The original reaches the menu byte
 * by adding the player to the address 0xa9c002. MATCHING DEVICE: under the target compiler
 * the macros spell the literal addresses; on the host they index one fixture object. */
#ifdef __MWERKS__
#define DOTR_LIFE_SET_MENU(player) (((unsigned char *)(player))[0xA9C002])
#define DOTR_LIFE_SET_WIDE (*(unsigned char *)0xA9C088)
#define DOTR_LIFE_SET_LIFE ((short *)0xA9C010)
#else
struct DotrLifeSetBlock {
    unsigned char bytes[0x10];
    short life[2];
    unsigned char rest[0x78];
};
extern struct DotrLifeSetBlock dotr_life_set_block;
#define DOTR_LIFE_SET_MENU(player) (dotr_life_set_block.bytes[(player) + 2])
#define DOTR_LIFE_SET_WIDE (dotr_life_set_block.rest[0x88 - 0x14])
#define DOTR_LIFE_SET_LIFE (dotr_life_set_block.life)
#endif

/* The packet header ExSortPrim_Pipe takes. Assumed: field
 * meanings from the values stored here (8, 0x14, 0x41414141). */
struct DotrLifeSetTag {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
};

struct DotrLifeSetColor {
    unsigned char r, g, b, a;
    float q;
};

/* A 64-bit GS vertex word: x and y (12.4 fixed point), z and fog. The game's code uses 64-bit
 * bit-fields; on the host the same little-endian layout is spelled with 32-bit units. */
#ifdef __MWERKS__
typedef unsigned long long DotrLifeSetBits;
#else
typedef unsigned int DotrLifeSetBits;
#endif
struct DotrLifeSetVertex {
    DotrLifeSetBits x : 16;
    DotrLifeSetBits y : 16;
    DotrLifeSetBits z : 24;
    DotrLifeSetBits fog : 8;
};

struct DotrLifeSetPoint {
    struct DotrLifeSetColor color;
    struct DotrLifeSetVertex xyz;
};

/* The gauge packet built on the stack: header and four coloured points (a gouraud strip). */
struct DotrLifeSetPacket {
    unsigned long long tag[2];
    struct DotrLifeSetPoint points[4];
    unsigned long long end;
};

/* The two gauge colours at 0x2914a0, stored b, g, r, a. */
struct DotrLifeSetSource {
    unsigned char b, g, r, a;
};

struct DotrLifeSetPair {
    struct DotrLifeSetSource c[2];
};

/* The duel control block at 0x317490, read here as bytes: +0xc58 the draw depth word. */
extern unsigned char data_00317490_lifeset[0xd70];
/* Icon definitions (10 bytes each) drawn by lSzDuel_DrawWindow_ExIcon_Pipe, declared as
 * words. */
extern int data_00290a1c_lifeset;
extern int data_00290896_lifeset;
extern int data_002908a0_lifeset;
extern int data_00290a58_lifeset;
extern int data_00290a62_lifeset;
extern int data_00290a6c_lifeset;
extern struct DotrLifeSetPair data_002914a0_lifeset;
/* The two button hint texts, and a colour passed as a word. */
extern signed char data_002bf05b_lifeset[];
extern signed char data_002bef93_lifeset[];
extern char data_002be944_lifeset[];

/* Callees (names from the Japanese build's symbols): lSzDuel_DrawWindow_Frame_Pipe takes its ninth
 * argument as 64-bit here. Assumed: func_0019bef0 is a string length; SzMsg_GetInfo returns the
 * loaded message's info words with the width at [3]. SzMsg_DrawRect_Pipe is passed by address only. */
extern int func_0019bef0(const char *s);
extern int *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern DotrLifeSetWord SzMsg_LoadFont_Pipe(DotrLifeSetWord cursor, int msgInfoIndex, int stringIndex, int a3, int a4);
extern DotrLifeSetWord RhText_SetTextureFlush(DotrLifeSetWord cursor);
extern DotrLifeSetWord SzMsg_DrawX_Pipe(DotrLifeSetWord cursor, int index, int x, int y, int a4,
                                       void (*function)(void));
extern DotrLifeSetWord lSzDuel_DrawWindow_Frame_Pipe(DotrLifeSetWord cursor, int a1, int a2, int a3, int x, int y,
                                                     int w, int h, long long a8);
extern DotrLifeSetWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrLifeSetWord cursor, void *def, int x, int y, int a4,
                                                      int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                                                      int a12, int a13);
extern DotrLifeSetWord SzDuel_DrawWindow_FontDigit_Pipe(DotrLifeSetWord cursor, int x, int y, int a, int flags,
                                                        int width, int value);
extern DotrLifeSetWord SzDuel_DrawWindow_Font_Pipe(DotrLifeSetWord cursor, int x, int y, int a, int flags, int n,
                                                   signed char *text, DotrLifeSetWord color);
extern DotrLifeSetWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrLifeSetWord cursor, int colour, int x, int y, int w,
                                                       int h, int a6, int a7);
extern DotrLifeSetWord ExSortPrim_Pipe(DotrLifeSetWord cursor, DotrLifeSetWord packet, int a2, int a3);

/* 0x1c92c0 lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe (name from the Japanese build's symbols):
 * draws player `player`'s (0 or 1) life-point page at (x, y) into the packet cursor and
 * returns the new cursor. */
DotrLifeSetWord lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(DotrLifeSetWord arg0, int player, int x, int y);

#endif
