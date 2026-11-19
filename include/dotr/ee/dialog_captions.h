#ifndef DOTR_EE_DIALOG_CAPTIONS_H
#define DOTR_EE_DIALOG_CAPTIONS_H

/* Build-deck and trade screen drawers that each draw one captioned window or title through the
 * window pipe at fixed offsets from the screen position, some with a message and a YES / NO
 * choice.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#ifdef __MWERKS__
typedef unsigned int DotrCaptionWord;
typedef int DotrCaptionTitleWord;
#else
#include <stdint.h>
typedef uintptr_t DotrCaptionWord;
typedef intptr_t DotrCaptionTitleWord;
#endif

/* func_0019bef0 (library block): the length of a text, as other groups use it. This view
 * returns unsigned long for the dialog drawers (the result is passed on as a 64-bit stack
 * argument); the title drawer uses the int form. */
extern unsigned long func_0019bef0_caption(const char *string);
extern int func_0019bef0(const char *string);

/* lSzDuel_DrawDialog_Window_Pipe (0x1b0ce0). ASSUMED prototype: declared
 * VARIADIC here only so that the three stack arguments (length, text, 1) are stored with
 * `sd` as in the original; the callee reads them with `lw` as fixed arguments, so its real
 * prototype is unknown. The first eight arguments are cursor, 2, -1, a texture number, the
 * window position and size. MATCHING DEVICE: the variadic declaration. */
extern DotrCaptionWord func_001b0ce0_caption(DotrCaptionWord cursor, int a1, int a2, int texture, int x, int y, int w, int h, ...);

/* lSzDuel_DrawWindow_Base_Pipe (0x1b62f0), lSzDuel_DrawWindow_Frame_Pipe (0x1b4780) and
 * SzDuel_DrawWindow_Font_Pipe (0x1b6f70); signatures as ex_board.h,
 * window_font.h and window_panels.h use them (the meaning of most arguments is assumed). */
extern DotrCaptionTitleWord func_001b62f0_caption(DotrCaptionTitleWord cursor, int x, int y, int cols, int rows);
extern DotrCaptionTitleWord func_001b4780_caption(DotrCaptionTitleWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8);
extern DotrCaptionTitleWord func_001b6f70_caption(DotrCaptionTitleWord cursor, int x, int y, int font, int flags, int n, const char *text, int c);

/* Caption strings, each named by an address two bytes BELOW its text and used as base + 2:
 * "PASSWORD" at 0x2bf002, "CAUTION" at 0x2bef67, "TRADE" at 0x2bf00b, "NOTICE" at 0x2bf040.
 * MATCHING DEVICE: the base + offset spelling keeps the address from being folded into a
 * single constant load, as a pooled string literal is. The bytes before each text are not
 * examined. */
extern char data_002bf000_caption[];
extern char data_002bef65_caption[];
extern char data_002bf009_caption[];
extern char data_002bf03e_caption[];

/* 0x1c0ee0 lSzDuel_DrawBuildDeck_PasswordFail_Pipe: dialog window texture 150 at
 * (x + 320, y + 141), 460 x 166, caption "PASSWORD". */
DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordFail_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c0f70 lSzDuel_DrawBuildDeck_PasswordDone_Pipe: the same with texture 151. */
DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordDone_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c1000 lSzDuel_DrawBuildDeck_Caution_Pipe: texture 124 at (x + 320, y + 163), 320 x 122,
 * caption "CAUTION". */
DotrCaptionWord lSzDuel_DrawBuildDeck_Caution_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c5ce0 lSzDuel_DrawTrade_Notice_Pipe: texture 128 at (x + 320, y + 163), 320 x 122,
 * caption "NOTICE". */
DotrCaptionWord lSzDuel_DrawTrade_Notice_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c3d50 lSzDuel_DrawTrade_Title_Pipe: a 6 x 1 window base and a frame at
 * (x + 224, y + 32), then the text "TRADE" at (x + 320, y + 39) in font 2, flags 3. */
DotrCaptionTitleWord lSzDuel_DrawTrade_Title_Pipe(DotrCaptionTitleWord cursor, int x, int y);

/* --- Exit / Do confirmation drawers (0x1cc380, 0x1c5980, 0x1c5b30) --- */

/* SzDuel_DrawWindow_Font_Pipe (0x1b6f70) as the confirmation drawers use it: the same
 * arguments as func_001b6f70_caption, but the cursor as DotrCaptionWord and the last argument
 * a pointer (the colour data at 0x2be944 or 0). Assumed: c overrides the text colour. */
extern DotrCaptionWord func_001b6f70_choice(DotrCaptionWord cursor, int x, int y, int font, int flags, int n, const char *text, char *c);
/* "EXIT" at 0x2bef98, "YES" at 0x2bef6f and "NO" at 0x2bef73, used as base + 2 like the
 * captions above (same MATCHING DEVICE). */
extern char data_002bef96_caption[];
extern char data_002bef6d_caption[];
extern char data_002bef71_caption[];
/* The 12 bytes at 0x2be944 (they look like three RGBA colours), passed to one of the two
 * choices. lSzDuel_ActCustomDuelProc (custom_duel_proc) confirms YES when the byte at 0xa9c08e is non-zero,
 * so in 0x1cc380 the colour goes to the choice that is not selected (a dimmed colour,
 * assumed). Contents not examined. */
extern char data_002be944_caption[];
/* The byte at 0xa9c08e that 0x1cc380 reads: non-zero while YES is selected (the `yes`
 * byte of custom_duel_proc.h). */
extern unsigned char data_00a9c08e_caption;
/* The duel control block at 0x317490 (other views: trade_proc.h as DotrTradeProcControl,
 * trade_scene.h, build_deck.h) with the members these drawers read: the byte at +0xd36
 * (0x3181c6), the choice cursor that decides which choice gets 0x2be944; the password
 * drawer uses it as the cursor column (0..7 assumed) and the reincarnation-done drawer as a
 * fade level (0..255, 128 and above counting as fully shown; assumed); for the reincarnation
 * drawer, the card-list pointer at +0x6d8, the list cursor halfword at +0xc8a and the list top
 * byte at +0xd5e (names assumed from use; the card id is read at list_cards[list_cursor +
 * list_top]); for the build-deck drawers, the work address word at +0x6d0 (three card id
 * halfwords at work + 32 are read), the picture depth word at +0xc58, the eight password
 * characters at +0xcc2, the step byte at +0xd57 and the side byte at +0xd5c (names from the
 * build_deck.h view); for the build-deck screen drawer (0x1baea0, group build_deck_draw), the
 * deck card-list pointer at +0x6dc, the deck list cursor halfword at +0xc8e, the signed panel
 * slide halfword at +0xc92, the mode byte at +0xd5d, the deck list top byte at +0xd5f and the
 * card grid top, column and row bytes at +0xd60..+0xd62 (build_deck.h calls them deck, deckTop,
 * slide, mode, deckCur, gridTop, gridCol, gridRow; the pairing of cursor and top names follows
 * this view's list_cursor / list_top, assumed). The block has a pointer member, so these
 * offsets hold for the target build only. The work word holds an address in an int on the
 * target and in an intptr_t on the host. */
#ifdef __MWERKS__
typedef int DotrCaptionAddress;
#else
typedef intptr_t DotrCaptionAddress;
#endif
struct DotrCaptionControl {
    char unknown_000[0x6d0];
    DotrCaptionAddress work;
    char unknown_6d4[4];
    unsigned short *list_cards;
    unsigned short *deck_cards;
    char unknown_6e0[0xc58 - 0x6e0];
    int pic_z;
    char unknown_c5c[0xc8a - 0xc5c];
    unsigned short list_cursor;
    char unknown_c8c[2];
    unsigned short deck_cursor;
    char unknown_c90[2];
    short slide;
    char unknown_c94[0xcc2 - 0xc94];
    char pass[8];
    char unknown_cca[0xd36 - 0xcca];
    unsigned char choice;
    char unknown_d37[0xd57 - 0xd37];
    unsigned char step;
    char unknown_d58[0xd5c - 0xd58];
    unsigned char side;
    unsigned char mode;
    unsigned char list_top;
    unsigned char deck_top;
    unsigned char grid_top;
    unsigned char grid_col;
    unsigned char grid_row;
};
extern struct DotrCaptionControl data_00317490_caption;
/* Message-slot calls as the trade drawers use them: SzMsg_LoadFont_Pipe (0x1fd680),
 * RhText_SetTextureFlush (0x20f290, packet_builds), SzMsg_GetInfo (0x1feab0,
 * slot_table), SzMsg_DrawX_Pipe (0x1fe7d0, slot_table) and SzMsg_Free (0x1feaf0, slot_table).
 * Argument meanings are assumed from the call: slot 1, message 127, a3 0, size 22; the slot
 * info has the text width and height at +12 and +16; the draw takes a slot, a position, a
 * pointer (0 here) and a callback. */
struct DotrCaptionMsgInfo {
    int unknown_00[3];
    int width;
    int height;
};
extern DotrCaptionWord func_001fd680_caption(DotrCaptionWord cursor, int index, int message, int a3, int size);
extern DotrCaptionWord func_0020f290_caption(DotrCaptionWord cursor);
extern struct DotrCaptionMsgInfo *func_001feab0_caption(int index);
extern DotrCaptionWord func_001fe7d0_caption(DotrCaptionWord cursor, int index, int x, int y, void *e, void (*function)(void));
extern void func_001feaf0_caption(int index);
/* The routine at 0x1fdfe0, only passed as the draw callback. */
extern void func_001fdfe0_caption(void);

/* --- Reincarnation confirmation (0x1c05f0) --- */

/* "REINCARNATION" at 0x2bef9d, used as base + 2 like the captions above. */
extern char data_002bef9b_caption[];
/* SzDuel_GetMsgName (0x1de210, resource_support) as this drawer uses it: the message (name)
 * of a card id, passed on as the dialog window's fourth argument. */
extern int func_001de210_caption(int card);
/* 0x1c05f0 lSzDuel_DrawBuildDeck_Reincarnation_Pipe: the card id at
 * list_cards[list_cursor + list_top]; dialog window (2, card, name of card) at (x + 320, y + 72),
 * -460 x 304, caption "REINCARNATION" (last stack argument 0); message 146 into slot 1 (size
 * 22), flush, drawn centred at (x + 320 - width / 2, y + 340 - (height + 12)) with callback
 * 0x1fdfe0, slot 1 freed; then "YES" at (x + 205, y + 340) and "NO" at (x + 435, y + 340), font
 * 2 flags 3, the 0x2be944 colour on YES when the byte at +0xd36 is 0, else on NO. Assumed:
 * the second and third window arguments are a mode and the card id (the other dialogs pass
 * -1 and a texture number there). */
DotrCaptionWord lSzDuel_DrawBuildDeck_Reincarnation_Pipe(DotrCaptionWord cursor, int x, int y);

/* --- Password accepted (0x1c0ce0) --- */

/* The record data_00317b60 points at, seen only for the card id halfword at +32 (blend_slots.h
 * types the same pointer as struct DotrBlendSlot *: an incompatible view, hence the distinct
 * symbol). Assumed: +32 holds the id of the card the password gave. */
struct DotrCaptionRecord {
    char unknown_00[32];
    unsigned short id;
};
extern struct DotrCaptionRecord *data_00317b60_caption;
/* The depth word at 0x3180e8 (as cursor_packets.h declares it). */
extern int data_003180e8;
/* lSzDuel_DrawWindow_PicItem_Pipe (0x1b7d60, cursor_packets) with this group's cursor word. */
extern DotrCaptionWord func_001b7d60_caption(DotrCaptionWord cursor, int id, int x, int y, int width, int height, unsigned int z);
/* SzMsg_DrawRatioX_Pipe (0x1fe660, cursor_packets) as this caller passes
 * it: slot, position, the two scale factors, a null pointer and the draw callback. */
extern DotrCaptionWord func_001fe660_caption(DotrCaptionWord cursor, int index, int x, int y, float sx, float sy, void *e,
                                            void (*function)(void));
/* The float-to-int routine at 0x1a22c0 (library block, declared only; cursor_packets.h uses
 * it the same way). */
extern int func_001a22c0(float value);
/* 0x1c0ce0 lSzDuel_DrawBuildDeck_PasswordSuccess_Pipe: dialog window (2, -1, -1) at
 * (x + 320, y + 65), 460 x 318, caption "PASSWORD" (last stack argument 1); the picture item
 * of the card id at (x + 256, y + 181), 128 x 128, depth from 0x3180e8; message 149 into slot 1
 * (size 22) and the card's name message into slot 2 (size 66); flush; slot 1 centred at
 * (x + 320 - width / 2, y + 125); slot 2 at y + 321, scaled horizontally by 436 / width when
 * wider than 436 and centred by func_001a22c0(width * scale / 2); both slots freed; returns
 * the cursor. */
DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordSuccess_Pipe(DotrCaptionWord cursor, int x, int y);

/* 0x1cc380 lSzDuel_DrawCustomDuel_Exit_Pipe: dialog window texture 145 at (x + 320, y + 148),
 * 320 x 152, caption "EXIT" (last stack argument 0); then "YES" at (x + 240, y + 264) and "NO"
 * at (x + 400, y + 264), font 2 flags 3, the 0x2be944 colour on YES when the byte at
 * 0xa9c08e is 0, else on NO; returns the cursor. */
DotrCaptionWord lSzDuel_DrawCustomDuel_Exit_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c5980 lSzDuel_DrawTrade_Exit_Pipe: texture 126 at (x + 320, y + 131), 320 x 186, caption
 * "EXIT"; loads message 127 into slot 1 (size 22), flushes, draws it centred at
 * (x + 320 - width / 2, y + 281 - (height + 12)) with callback 0x1fdfe0, frees slot 1; then
 * YES / NO at (x + 240 / x + 400, y + 281), the byte at 0x3181c6 choosing which gets 0x2be944.
 * MATCHING DEVICE: a local pointer to the control block declared first. */
DotrCaptionWord lSzDuel_DrawTrade_Exit_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c5b30 lSzDuel_DrawTrade_Do_Pipe: the same with texture 125 at (x + 320, y + 142),
 * 320 x 164, caption "TRADE" and the message and choices at y + 270. */
DotrCaptionWord lSzDuel_DrawTrade_Do_Pipe(DotrCaptionWord cursor, int x, int y);

/* --- Build-deck title, password entry and reincarnation done (0x1bb6d0, 0x1c0a30, 0x1c07e0) --- */

/* "BUILD DECK" at 0x2bef82, used as base + 2 like the captions above. */
extern char data_002bef80_caption[];
/* The icon definition table at 0x290080 (as window_panels.h and scaled_sprite.h declare it);
 * these drawers pass base + 2050, + 2160, + 2330, + 2480, + 2490 and + 2500. Contents not
 * examined. */
extern char gSzDuel_Icon_caption[];
/* lSzDuel_DrawWindow_ExIcon_Pipe (0x1b3cc0) with fourteen integer arguments as
 * scaled_sprite.h uses it, in a title-word and a dialog-word view; argument meanings assumed
 * (icon definition, position, then the constants 0, 1, 0, 0, 0, 0, 1, 0, -1, 128 here). */
extern DotrCaptionTitleWord func_001b3cc0_title(DotrCaptionTitleWord cursor, char *def, int x, int y, int a4, int a5, int a6,
                                                int a7, int a8, int a9, int a10, int a11, int a12, int a13);
extern DotrCaptionWord func_001b3cc0_caption(DotrCaptionWord cursor, char *def, int x, int y, int a4, int a5, int a6, int a7,
                                             int a8, int a9, int a10, int a11, int a12, int a13);
/* lSzDuel_DrawWindow_Frame_Pipe (0x1b4780) with the dialog cursor word (same arguments as
 * func_001b4780_caption). */
extern DotrCaptionWord func_001b4780_choice(DotrCaptionWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8);
/* SzDuel_DrawWindow_FontDigit_Pipe (0x1b6ea0, small_misc) with the title cursor word: a
 * number of `width` digits at (x, y); a and flags as the caller passes them (1, 0). */
extern DotrCaptionTitleWord func_001b6ea0_caption(DotrCaptionTitleWord cursor, int x, int y, int a, int flags, int width, int value);
/* SzData_GetCurrentDeck (0x2158b0), SzData_GetDeckCost (0x215f00) and SzData_GetDeckCount
 * (0x215f90), defined in side_table with these types. */
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetDeckCost(int side, int list);
extern int SzData_GetDeckCount(int side, int deckNo);
/* ExGetRegVal (0x208680) and ExClampMode_Pipe (0x2084d0), defined in draw_state; the
 * clamp mode takes the dialog cursor word here (cursor_packets.h uses the same form). */
extern long ExGetRegVal(int a, int b);
extern DotrCaptionWord func_002084d0_caption(DotrCaptionWord cursor, long value);

/* 0x1bb6d0 lSzDuel_DrawBuildDeck_Title_Pipe: for side = byte +0xd5c and its current deck: a
 * 6 x 1 window base and a frame (-1, 115, 4) 21 x 2 at (x + 224, y + 32), "BUILD DECK" at
 * (x + 320, y + 39) in font 2 flags 3, a frame (12, -1, 11) -128 x -48 at (x + 432, y + 16),
 * icons +2330 at (x + 2202, y + 1848) and +2160 at (x + 2172, y + 1866), the deck cost in four
 * digits at (x + 464, y + 44), icon +2050 at (x + 2236, y + 1866) and the deck count in two
 * digits at (x + 528, y + 44). MATCHING DEVICE: all locals assigned before use with the two
 * coordinate pairs reassigned per call (qy = y + 44 assigned twice). */
DotrCaptionTitleWord lSzDuel_DrawBuildDeck_Title_Pipe(DotrCaptionTitleWord cursor, int x, int y);
/* 0x1c0a30 lSzDuel_DrawBuildDeck_Password_Pipe: dialog window (2, -1, 148) at
 * (x + 320, y + 142), 346 x 164, caption "PASSWORD" (last stack argument 0L); the eight
 * password characters at (x + 320, y + 236), font 2 flags 2; a frame (-1, -1, 7) -26 x -28 at
 * (x + 230 + 22 * column, y + 231); messages 170 (slot 1, size 22) and 173 (slot 2, size 44),
 * flush; slot 1 centred on x + 234 at y + 272 with icons +2490 and +2480 at x + 1914 and
 * x + 1938 less half its width, y + 2096; slot 2 centred on x + 406 at y + 272 with icon +2500
 * at x + 2110 less half its width, y + 2096. Slots are not freed here.
 * MATCHING DEVICE: the coordinates kept in px / py, reassigned before each call. */
DotrCaptionWord lSzDuel_DrawBuildDeck_Password_Pipe(DotrCaptionWord cursor, int x, int y);
/* 0x1c07e0 lSzDuel_DrawBuildDeck_ReincarnationDone_Pipe: with ids = the halfwords at
 * work + 32 and the fade byte at +0xd36: dialog window (2, -1, name) at (x + 320, y + 93),
 * -460 x 262, caption "REINCARNATION", last stack argument 0L below fade 128 and 1L from it;
 * name is -1 below fade 128, else the name of ids[step] for step < 3, else 147. Then the
 * pictures ids[0..2] at (x + 116 + 140 i, y + 187), depth from +0xc58: picture i is hidden
 * (0 x 0) while step < i, faded in (ExClampMode with fade << 14 | 0xa | fade << 34, size
 * fade x fade) when step == i, else 128 x 128. When step < 3 the clamp mode is reset to
 * ExGetRegVal(0, 4). MATCHING DEVICE: the control block pointer declared first; the size
 * assignments as `w = h = ...` / `h = w = 128`; the last stack argument as a long
 * conditional. */
DotrCaptionWord lSzDuel_DrawBuildDeck_ReincarnationDone_Pipe(DotrCaptionWord cursor, int x, int y);

#endif
