#ifndef DOTR_EE_CAMP_ST_WINDOW_H
#define DOTR_EE_CAMP_ST_WINDOW_H

/* Taku_Camp_ST_WINDOW, the drawer of the campaign map's status panel for side 0 (side windows,
 * deck cost, leader rank marks, location counters, leader name).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Views assumed from the access widths and strides, as in camp_tikei.h and
 * camp_teki_window.h: 38-byte drawing templates (eight halfwords, w[0] and w[1] unused here,
 * a code at +36), 10-byte window records with SIGNED halfwords (x and y of entry 2 are read
 * with `lh`), 52-byte location records whose halfword at +16 is compared with 8, and the
 * fourth word of a message slot entry (its width). Assumed: every member name. */
struct DotrStWindowItem {
    short w[8];
    short unknown_10[10];
    short code;                 /* +36 */
};
struct DotrStWindowWindow {
    short x, y, w, h;
    char unknown_08[2];
};
struct DotrStWindowRec {
    char unknown_00[16];
    short kind;                 /* +16 */
    char unknown_12[34];
};
struct DotrStWindowInfo {
    int unknown_00[3];
    int w;                      /* +12 */
};

/* View of the camp state at 0x299e10 (camp_teki_window.h and camp_entry.h have other views
 * under their own symbols). Assumed: the names; `mode` as the side shown (it indexes the
 * templates at 0x2980d0 and is the window flag; this function toggles it); `seen` as one
 * state byte per location 0..21 (counted when at least 3 or exactly 3); `wins` as the two
 * numbers drawn at x 253 and 349 (camp_teki_window.h: won and lost counts at +66/+130 are
 * other members; these are the halfwords at +62 and +64); `kind` as the selected location. */
struct DotrStWindowBoard {
    signed char mode;           /* +0 */
    signed char unknown_01[2];
    signed char seen[59];       /* +3 */
    short wins[2];              /* +62 */
    char unknown_42[132];
    signed char kind;           /* +198 */
};
extern struct DotrStWindowBoard data_00299e10_stwindow;

/* The 22 location records at 0x298ba0 (camp_tikei.h: data_00298ba0_tikei). */
extern struct DotrStWindowRec data_00298ba0_stwindow[];

/* Window records at 0x297b20 (entries 0 to 2 and 16 are used; entry 2's x and y place the
 * leader name) and template tables indexed as the function does: 0x2980d0 by the side;
 * 0x297e20 by the current deck; 0x297be0 by (rank - 1) / 3; 0x297c80 entries 0 to 2 (rank
 * marks); 0x298210 entries 0 and 1 (counter marks); 0x298170 entries 0 to 3. 0x30f0f7 and
 * 0x30f0fc: two strings handed to SzDuel_DrawWindow_Font_Pipe with lengths 4 and 5.
 * Assumed: the extents. */
extern struct DotrStWindowWindow camp_window_My_data_stwindow[];
extern struct DotrStWindowItem tex_bara_data_stwindow[];
extern struct DotrStWindowItem tex_deck_data_stwindow[];
extern struct DotrStWindowItem tex_batti_data_stwindow[];
extern struct DotrStWindowItem tex_hosi_data_stwindow[];
extern struct DotrStWindowItem camp_card_stwindow[];
extern struct DotrStWindowItem camp_window_data_stwindow[];
extern char data_0030f0f7_stwindow[];
extern char data_0030f0fc_stwindow[];

/* The work area at the literal address 0x1ed0000 (not a relocated symbol), reached by offset
 * as in camp_teki_window.h. On the host the base is a fixture of 0x5800 bytes that the test
 * defines. Offsets used, with ASSUMED types and meanings: halfwords 0x6c (compared with 256:
 * toggles the side), 0x70 (slide: y = 128 - (it * 128 >> 8)), 0x226 (handed to
 * Taku_Window_Set; read once through a base pointer and once by its address), 0x5442 (the
 * deck cost, written); signed bytes 0x18c (as in camp_teki_window.h) and 0x53c2 (compared
 * with 2 for location 11). */
#ifdef __MWERKS__
#define DOTR_ST_WINDOW_BASE 0x1ed0000
#else
union DotrStWindowWork {
    unsigned char bytes[0x5800];
    int align;
};
extern union DotrStWindowWork dotr_camp_st_window_work;
#define DOTR_ST_WINDOW_BASE (dotr_camp_st_window_work.bytes)
#endif
/* The halfword at +0x226 as the function reads it through its base pointer. */
struct DotrStWindowBank {
    char unknown_000[0x226];
    short depth;                /* +0x226 */
};

/* Callees as this function calls them, with int cursors (no other group header is included;
 * the host test stubs every callee). The prototypes are this caller's views: parameter
 * counts and widths are ASSUMED from the argument registers, the stack slots and the
 * narrowing at these call sites. Taku_Window_Set (0x2195e0) takes NINE arguments, the third
 * and fifth narrowed to unsigned 16 bits (as camp_tikei.h has it); Taku_Obj_Set (declared in
 * small_state.h as returning nothing) passes its v0 on as the cursor, as camp_tikei.h and
 * camp_teki_window.h assume; SzDuel_DrawWindow_Font_Pipe (0x1b6f70) eight arguments;
 * SzMsg_LoadFont_Pipe (0x1fd680) five; SzMsg_DrawRatioX_Pipe (0x1fe660) two float ratios, a
 * colour block and a callback; SzMsg_DrawRect_Pipe (0x1fdfe0) is passed by address only. The
 * SzData_* views take the side first (always 0 here). */
extern int Taku_Window_Set(int cursor, struct DotrStWindowWindow *window, unsigned short flag, int a3,
                           unsigned short x, unsigned short a5, int a6, int a7, int a8);
extern int Taku_Obj_Set(int cursor, struct DotrStWindowItem *item, short x, short y);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetDeckCost(int side, int list);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzData_GetRank(int side, int cardNo);
extern int SzDuel_GetMsgName(int cardNo);
extern int SzMsg_LoadFont_Pipe(int cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern int RhText_SetTextureFlush(int cursor);
extern struct DotrStWindowInfo *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern int SzMsg_DrawRatioX_Pipe(int cursor, int index, int x, int y, float sx, float sy, void *e,
                                 void (*function)(void));
extern int SzDuel_DrawWindow_FontDigit_Pipe(int cursor, int x, int y, int a, int flags, int width,
                                            int value);
extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int a, int flags, int n,
                                       const char *text, int c);

/* 0x2347d0 Taku_Camp_ST_WINDOW (name from the Japanese build's symbol table). Draws the
 * campaign map's status panel; see docs/modules/campaign.md. Host callers must
 * keep the side, the selected location, the current deck and the rank within the fixtures:
 * nothing is bounded here. Assumed: one int parameter (the packet cursor), returned advanced
 * by the callees. */
int Taku_Camp_ST_WINDOW(int cursor);

#endif
