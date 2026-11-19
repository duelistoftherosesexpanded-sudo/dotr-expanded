#ifndef DOTR_EE_CUSTOM_MENU_TOP_H
#define DOTR_EE_CUSTOM_MENU_TOP_H

/* The custom-duel menu's top page drawer: the item list in one or two columns, the cursor frame
 * and button hints.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The packet cursor (and the player, passed as a pointer-typed word) are 32-bit words in the
 * game; a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrMenuTopWord;
#else
typedef long DotrMenuTopWord;
#endif

/* The custom-duel settings block at the literal address 0xa9c000 (see custom_duel_proc.h):
 * +0x02 per player the menu byte (item in the low nibble), +0x88 the two-player layout byte.
 * MATCHING DEVICE: under the target compiler the macros are the literal addresses; on the host
 * they name one fixture object. */
#ifdef __MWERKS__
#define DOTR_MENU_TOP_BLOCK ((unsigned char *)0xA9C000)
#define DOTR_MENU_TOP_WIDE (*(unsigned char *)0xA9C088)
#else
extern unsigned char dotr_menu_top_block[0x90];
#define DOTR_MENU_TOP_BLOCK (dotr_menu_top_block)
#define DOTR_MENU_TOP_WIDE (dotr_menu_top_block[0x88])
#endif

/* The item lists of the one-column and two-column layouts: two rows of five item numbers
 * (0xff: no item). Assumed: from their use here. */
extern unsigned char SzDuel_CustomDuelMenu_menutop[2][10];
/* Icon definitions (10 bytes each), declared as words, the three
 * button hint texts and a colour passed as a word. */
extern int data_00290a58_menutop;
extern int data_00290a62_menutop;
extern int data_00290a6c_menutop;
extern int data_00290a76_menutop;
extern signed char data_002bf05b_menutop[];
extern signed char data_002bef98_menutop[];
extern signed char data_002bf03a_menutop[];
extern char data_002be944_menutop[];

/* Callees (names from the Japanese build's symbols); lSzDuel_DrawWindow_Frame_Pipe takes nine
 * words. Assumed: func_0019bef0 is a string length; SzMsg_GetInfo returns the loaded message's info
 * words with the width at [3]; SzData_GetMapEditFlag non-zero when the map item (7) is available.
 * SzMsg_DrawRect_Pipe is passed by address only. */
extern int func_0019bef0(const char *s);
extern int *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern int SzData_GetMapEditFlag(int side);
extern DotrMenuTopWord SzMsg_LoadFont_Pipe(DotrMenuTopWord cursor, int msgInfoIndex, int stringIndex, int a3, int a4);
extern DotrMenuTopWord RhText_SetTextureFlush(DotrMenuTopWord cursor);
extern DotrMenuTopWord SzMsg_DrawRatioX_Pipe(DotrMenuTopWord cursor, int index, int x, int y, float sx, float sy,
                                             int a6, void (*function)(void));
extern DotrMenuTopWord lSzDuel_DrawWindow_Frame_Pipe(DotrMenuTopWord cursor, int a1, int a2, int a3, int x, int y,
                                                     int w, int h, int a8);
extern DotrMenuTopWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrMenuTopWord cursor, void *def, int x, int y, int a4,
                                                      int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                                                      int a12, int a13);
extern DotrMenuTopWord SzDuel_DrawWindow_Font_Pipe(DotrMenuTopWord cursor, int x, int y, int a, int flags, int n,
                                                   signed char *text, DotrMenuTopWord color);
extern DotrMenuTopWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrMenuTopWord cursor, int colour, int x, int y, int w,
                                                       int h, int a6, int a7);

/* 0x1c8690 lSzDuel_DrawCustomDuel_Menu_Top_Pipe (name from the Japanese build's symbols): draws
 * the top page of the custom-duel menu for the player arg1 (0 or 1, passed in a pointer-typed
 * argument) at (x, y) into the packet cursor and returns the new cursor. */
DotrMenuTopWord lSzDuel_DrawCustomDuel_Menu_Top_Pipe(DotrMenuTopWord arg0, unsigned char *arg1, int x, int y);

#endif
