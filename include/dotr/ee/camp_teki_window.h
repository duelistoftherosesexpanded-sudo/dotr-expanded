#ifndef DOTR_EE_CAMP_TEKI_WINDOW_H
#define DOTR_EE_CAMP_TEKI_WINDOW_H

/* Taku_Camp_TEKI_WINDOW, the drawer of the campaign map's opponent panel for the selected location
 * (name, face, deck cost, leader details, tooltip).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Views assumed from the access widths and strides, as in camp_dare.h and camp_tikei.h:
 * 38-byte drawing templates (eight halfwords of which w[0], w[1] and w[2] are read here as
 * x, y and a right edge, and a code at +36), 10-byte window records with unsigned halfwords,
 * the fourth word of a message slot entry (its width) and the eight-byte face request.
 * Assumed: every member name. */
struct DotrTekiWindowItem {
    short w[8];
    short unknown_10[10];
    short code;                 /* +36 */
};
/* The same template with its code read unsigned: the leader-kind switch stores the code of
 * the template at 0x2980a0 through this view (the default case's load is `lhu`). */
struct DotrTekiWindowItemU {
    short w[8];
    short unknown_10[10];
    unsigned short code;        /* +36 */
};
struct DotrTekiWindowWindow {
    unsigned short x, y, w, h;
    char unknown_08[2];
};
struct DotrTekiWindowInfo {
    int unknown_00[3];
    int w;                      /* +12 */
};
struct DotrTekiWindowFace {
    unsigned char id, no, kind, mode;
    unsigned short x, y;
};

/* View of the camp state at 0x299e10 (camp_entry.h has a fuller view, camp_system.h the same
 * members under the same names). Assumed: the names; `lv` as one state byte per location
 * (indexed with the selected location, at most 21, and with 11); `h66` and `h130` as the
 * won and lost counts per name number (drawn as two four-digit numbers); `h196` as the name
 * number this function stores (message number - 196); `b198` as the selected location. */
struct DotrTekiWindowBoard {
    signed char b0;             /* +0: side */
    signed char b1;
    signed char b2;
    signed char lv[63];         /* +3 */
    short h66[32];              /* +66 */
    short h130[32];             /* +130 */
    short h194;
    short h196;                 /* +196 */
    signed char b198;           /* +198 */
};
extern struct DotrTekiWindowBoard data_00299e10_tekiwindow;

/* One of the 22 location records at 0x298ba0, 52 bytes (camp_system.h, camp_entry.h and
 * others have other views); only the halfword at +16 is read, compared with 8 to pick the
 * panel's side. Assumed: the name and the type. */
struct DotrTekiWindowRec {
    short unknown_00[8];
    short h10;                  /* +16 */
    char unknown_12[34];
};
extern struct DotrTekiWindowRec data_00298ba0_tekiwindow[];

/* 0x29aef0: 22 string pointers, one per location, and 0x29af50: two templates; both are
 * copied to the stack whole at entry. Assumed: the counts (from the sizes of the copies, 88
 * and 76 bytes) and that the pointers are strings (they go to 0x19bef0). */
struct DotrTekiWindowNames {
    char *s[22];
};
struct DotrTekiWindowPair {
    struct DotrTekiWindowItem it[2];
};
extern struct DotrTekiWindowNames data_0029aef0_tekiwindow;
extern struct DotrTekiWindowPair data_0029af50_tekiwindow;

/* The window records at 0x297b40 (camp_dare.h names the same address
 * camp_window_Teki_data); entries 0 to 5 and 7 are used, and the widths of entries 5 and 7
 * are WRITTEN. 0x297bca is a further window record used on its own; it lies 138 bytes
 * after 0x297b40, which is not a multiple of the 10-byte stride, so it is a separate symbol.
 * Assumed: the extents. */
extern struct DotrTekiWindowWindow camp_window_Teki_data_tekiwindow[];
extern struct DotrTekiWindowWindow data_00297bca_tekiwindow;

/* Template tables and single templates, indexed as the function does: 0x298120 by the
 * panel's side; 0x2983a0 entries 2, 3 and the side; 0x298320 entries 0 to 2; 0x297d00 by
 * (rank - 1) / 3; 0x297da0 entries 0 to 2 (rank marks); 0x298b1c alone (the extra box of
 * location 11); 0x2980a0 alone (the leader-kind icon, see DotrTekiWindowItemU), and the
 * halfword at 0x297ea0 that the switch's default copies into its code. Assumed: the
 * extents and that 0x297ea0 is an unsigned halfword of its own. */
extern struct DotrTekiWindowItem tex_bara_data2_tekiwindow[];
extern struct DotrTekiWindowItem data_002983a0_tekiwindow[];
extern struct DotrTekiWindowItem data_00298320_tekiwindow[];
extern struct DotrTekiWindowItemU teki_syurui_data_tekiwindow;
extern struct DotrTekiWindowItem tex_batti_data2_tekiwindow[];
extern struct DotrTekiWindowItem tex_hosi_data2_tekiwindow[];
extern struct DotrTekiWindowItem data_00298b1c_tekiwindow;
extern unsigned short teki_syurui_tex_no_tekiwindow;

/* 0x2999cc: a four-byte row per name number (camp_dare.h); the column is the byte at
 * 0x1ed5468. 0x30f102, 0x30f1e8 and 0x30f1ef: three strings handed to
 * SzDuel_DrawWindow_Font_Pipe with the lengths 4, 6 and 6. Assumed: the extents. */
extern unsigned char data_002999cc_tekiwindow[][4];
extern char data_0030f102_tekiwindow[];
extern char data_0030f1e8_tekiwindow[];
extern char data_0030f1ef_tekiwindow[];

/* First member of the card record SzDuel_GetCardConstInfo returns: its low five bits + 1
 * pick the leader-kind icon. Assumed: an unsigned byte (state_grid.h declares the function
 * with its own record type). */
struct DotrTekiWindowCard {
    unsigned char b0;
};

/* The work area at the literal address 0x1ed0000 (not a relocated symbol). The function
 * reaches it by offset (macros WB, WH and WU in the source) and the face request at
 * 0x1ed5424 through DotrTekiWindowFace. On the host the base is a fixture of 0x5800 bytes
 * that the test defines. Offsets used, with ASSUMED types and meanings: halfwords 0 and 2
 * (the cursor's screen position), 4 (unsigned, compared with 2), 0x68 (a slide amount, << 8
 * >> 4), 0x6c (the panel's y + 256; 0 hides the panel), 0x224 (handed on to
 * Taku_Window_Set), 0x5444 (the deck cost, written); signed bytes 0x6e, 0x72, 0x18c
 * (camp_entry.h: both sides done), 0x5374, 0x5376, 0x537a, 0x537b, 0x53ab, 0x53c2, 0x5406
 * (the deck's set-up state, written), 0x540a (the face state: 1, 2, 10 request, 100 close;
 * written), 0x5440 (a percentage: x = -640 * it / 100), 0x5446 (asks once for message 249;
 * cleared), 0x5468, 0x5472 (the centred name box), 0x547a, 0x5481, 0x5482, 0x5488 (set to 1
 * when the leader block is drawn). */
#ifdef __MWERKS__
#define DOTR_TEKI_WINDOW_BASE 0x1ed0000
#define DOTR_TEKI_WINDOW_FACE ((struct DotrTekiWindowFace *)0x1ed5424)
#else
union DotrTekiWindowWork {
    unsigned char bytes[0x5800];
    int align;
};
extern union DotrTekiWindowWork dotr_camp_teki_window_work;
#define DOTR_TEKI_WINDOW_BASE (dotr_camp_teki_window_work.bytes)
#define DOTR_TEKI_WINDOW_FACE ((struct DotrTekiWindowFace *)(void *)(dotr_camp_teki_window_work.bytes + 0x5424))
#endif

/* Callees as this function calls them, with int cursors (no other group header is included;
 * the host test stubs every callee). The prototypes are this caller's views: parameter
 * counts and widths are ASSUMED from the argument registers, the stack slots and the
 * narrowing at these call sites, and several differ from the definitions' types
 * (Taku_Obj_Set, Taku_FT4_Set and Taku_G4_Set take this group's templates and return the
 * cursor; ShFace_Request and ShFace_Close this group's request; SzMsg_GetInfo this group's
 * view of the slot entry; SzDuel_GetCardConstInfo this group's first byte).
 * Taku_Window_Set (0x2195e0) takes NINE arguments here, the third, fifth and sixth narrowed to
 * unsigned 16 bits; SzDuel_DrawWindow_Font_Pipe (0x1b6f70) eight; SzMsg_LoadFont_Pipe
 * (0x1fd680) five; SzMsg_DrawRatioX_Pipe (0x1fe660) two float ratios, a colour block and a
 * callback; SzMsg_DrawRect_Pipe (0x1fdfe0) is passed by address only. 0x19bef0 is a routine of
 * the library block 0x17b48c-0x1a6c80 (out of scope: declared only), assumed to return a
 * string's length. */
extern int Taku_Window_Set(int cursor, struct DotrTekiWindowWindow *window, unsigned short flag, int a3,
                           unsigned short x, unsigned short y, int a6, int a7, int a8);
extern int Taku_Obj_Set(int cursor, struct DotrTekiWindowItem *item, short x, short y);
extern int Taku_FT4_Set(int cursor, struct DotrTekiWindowItem *item, short x, short y, int a4);
extern int Taku_G4_Set(int cursor, struct DotrTekiWindowItem *item, short x, short y);
extern int SzMsg_LoadFont_Pipe(int cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern int RhText_SetTextureFlush(int cursor);
extern struct DotrTekiWindowInfo *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern int SzMsg_DrawRatioX_Pipe(int cursor, int index, int x, int y, float sx, float sy, void *e,
                                 void (*function)(void));
extern int ShFace_Request(struct DotrTekiWindowFace *req);
extern void ShFace_Close(struct DotrTekiWindowFace *req);
extern void SzData_SetupFromConstDeck(int side, int deckIndex);
extern int SzData_GetDeckCost(int side, int list);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzData_GetRank(int side, int cardNo);
extern int SzDuel_DrawWindow_FontDigit_Pipe(int cursor, int x, int y, int a, int flags, int width,
                                            int value);
extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int a, int flags, int n,
                                       const char *text, int c);
extern struct DotrTekiWindowCard *SzDuel_GetCardConstInfo(int cardNo);
extern int SzDuel_GetMsgName(int cardNo);
extern int SzEffect_IsUse2(int a, int b);
extern int func_0019bef0(const char *string);

/* 0x2354a0 Taku_Camp_TEKI_WINDOW (name from the Japanese build's symbol table, placed by gap
 * order; see docs/topics/jp-build-symbols.md). Draws the opponent panel of the selected
 * location; see docs/modules/campaign.md. Host callers must keep the selected location within
 * 0..21 and the name number within the fixtures: nothing is bounded here.
 * Assumed: one int parameter (the packet cursor), returned advanced by the callees. */
int Taku_Camp_TEKI_WINDOW(int cursor);

#endif
