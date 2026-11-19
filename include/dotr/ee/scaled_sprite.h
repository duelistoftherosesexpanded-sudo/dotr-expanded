#ifndef DOTR_EE_SCALED_SPRITE_H
#define DOTR_EE_SCALED_SPRITE_H

/* Functions that queue a sprite through SzMsg_DrawRatioX_Pipe with a scale computed from a
 * slot-table entry's width and height (first member 0x1bf180).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor kept as a 32-bit EE word (pointer on the target). */
typedef int DotrSpriteWord;

/* Slot-table entry as read here: two ints at 0xc/0x10, used as width and height. */
struct DotrSpriteEntry {
    unsigned char unknown_00[0xc];
    int unknown_0c;
    int unknown_10;
};

/* Passed by address as SzMsg_DrawRatioX_Pipe's fifth argument; contents not examined. */
extern unsigned char gSzDuel_aicolMsg_bytes[];

extern void SzMsg_DrawRect_Pipe(void);
extern DotrSpriteWord SzMsg_LoadFont_Pipe(DotrSpriteWord cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern DotrSpriteWord RhText_SetTextureFlush(DotrSpriteWord cursor);
extern struct DotrSpriteEntry *SzMsg_GetInfo(int index);
/* x and y are passed here without narrowing; draw_state.h's view has shorts. */
extern DotrSpriteWord SzMsg_DrawRatioX_Pipe(DotrSpriteWord cursor, int index, int x, int y, void *e, void (*function)(void),
                                    float sx, float sy);
extern void SzMsg_Free(int index);

DotrSpriteWord lSzDuel_DrawBuildDeck_Detail_Special_Pipe(DotrSpriteWord cursor, int id, int x, int y, int flag);

/* Nine arguments as passed by 0x1b3640; the ninth is stored as a 64-bit stack word. Meanings unknown. */
extern DotrSpriteWord lSzDuel_DrawWindow_Frame_Pipe(DotrSpriteWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8);
/* Float to int, used as a rounding conversion (assumed). */
extern int func_001a22c0(float value);

DotrSpriteWord lSzDuel_DrawWindow_Field_Pipe(DotrSpriteWord cursor, int id, int x, int y);

/* 0x1af5c0: when SzGame_GetMode(1) gives 1, the byte at 0x317490+0xd45 is set and
 * the one at +0xd47 is clear, walks the 0x1df110 count of the side byte at
 * 0x3181e4: for each SzDuel_GetUnit(2, i + 1) result with a pointer at +4 to two
 * floats and a non-negative 0x1df0c0 word, draws the 0x1b4780 frame when the
 * byte at +0xd4f equals i and, unless the low 12 bits are 999 or bits 12-15
 * are 0, a 0x1b3c80 piece and those bits through 0x1b6ea0. The control block
 * is read as bytes only; the two floats, the 0x1df540 result view and the
 * char data at 0x2902a6 (passed by address) are assumed. 0x1b3c80 is
 * declared void with int arguments in its own group; this caller keeps its result and
 * stores the last three arguments as 64-bit stack words, so
 * func_001b3c80_cursor is a second view of the same address. */
struct DotrSpritePoint {
    float x;
    float y;
};
struct DotrSpriteMark {
    int unknown_00;
    struct DotrSpritePoint *unknown_04;
};
extern unsigned char data_00317490_bytes[];
extern unsigned char turnOwner[];
extern char data_002902a6[];
extern int SzGame_GetMode(int a);
extern int SzDuel_GetHandCount(int side);
extern int SzDuel_InqHand(int side, int position);
extern struct DotrSpriteMark *func_001df540_mark(int a, int b);
extern DotrSpriteWord func_001b3c80_cursor(DotrSpriteWord cursor, char *b, int c, int d, int e, int f, int g, int h,
                                           long i, long j, long k);
extern DotrSpriteWord SzDuel_DrawWindow_FontDigit_Pipe(DotrSpriteWord cursor, int x, int y, int a, int flags, int width, int value);
DotrSpriteWord SzDuel_DrawHandCursor_Pipe(DotrSpriteWord cursor);

/* 0x1c8dc0 lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe (original name).
 * Assumed: the block at 0xa9c000 is reached through an integer constant (the original loads
 * the address with lui/ori and folds it into the indexed loads), with two bytes per side at
 * +2 and +4 and the flag at +0x88 (other headers declare 0xa9c088 as customDuelOpponentId); the
 * icon table at 0x290080 in 10-byte rows; the two captions as character data at their
 * addresses; lSzDuel_DrawWindow_ExIcon_Pipe with fourteen integer arguments and a cursor
 * result (small_misc.h's view is void with fewer arguments); the other callees from
 * this call site. */
struct DotrDeckMenuBlock {
    unsigned char unknown_00[2];
    unsigned char unknown_02[2];
    unsigned char unknown_04[2];
    unsigned char unknown_06[0x82];
    unsigned char unknown_88;
};
#ifdef __MWERKS__
#define DOTR_DECK_MENU ((struct DotrDeckMenuBlock *)0x00a9c000)
#else
extern struct DotrDeckMenuBlock dotr_deck_menu_00a9c000;
#define DOTR_DECK_MENU (&dotr_deck_menu_00a9c000)
#endif
extern char gSzDuel_Icon_bytes[];
extern char data_002bf05b[];
extern char data_002bef93[];
extern int SzData_GetDeckCount(int side, int deckNo);
extern int SzData_GetLeader(int side, int deckNo);
extern int func_0019bef0(const char *string);
extern DotrSpriteWord SzDuel_DrawWindow_Font_Pipe(DotrSpriteWord cursor, int x, int y, int a, int flags, int n, const char *text, int c);
extern DotrSpriteWord lSzDuel_DrawWindow_PicMini_Pipe(DotrSpriteWord cursor, int id, int x, int y, int width, int height, unsigned int z, int bright);
extern DotrSpriteWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrSpriteWord cursor, char *def, int x, int y, int a4, int a5, int a6, int a7,
                                                     int a8, int a9, int a10, int a11, int a12, int a13);
extern DotrSpriteWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrSpriteWord cursor, int side, int x, int y, int w, int h, int a6, int a7);
/* MATCHING DEVICE: the side's bytes are reached with the side number used as a byte pointer
 * indexed by the absolute address (side[0xa9c002], side + 0xa9c000), and the flag through its
 * literal address; under __MWERKS__ these macros expand to exactly those expressions; on the host
 * they index the fixture block. The three icon rows at 0x290a58, 0x290a62 and 0x290a6c (rows
 * 252..254 of gSzDuel_Icon_bytes), and 0x290896, 0x2908a0, 0x2908e6 (rows 207, 208, 215), are named by
 * their own addresses. */
#ifdef __MWERKS__
#define DOTR_SELDECK_BYTE(side, address) (((unsigned char *)(side))[(address)])
#define DOTR_SELDECK_PTR(side, address) ((unsigned char *)(side) + (address))
#define DOTR_SELDECK_FLAG (*(unsigned char *)0xA9C088)
extern char data_00290a58[];
extern char data_00290a62[];
extern char data_00290a6c[];
extern char data_00290896[];
extern char data_002908a0[];
extern char data_002908e6[];
#else
#define DOTR_SELDECK_BYTE(side, address) \
    (((unsigned char *)&dotr_deck_menu_00a9c000)[(side) + ((address) - 0xA9C000)])
#define DOTR_SELDECK_PTR(side, address) ((unsigned char *)&dotr_deck_menu_00a9c000 + (side) + ((address) - 0xA9C000))
#define DOTR_SELDECK_FLAG (dotr_deck_menu_00a9c000.unknown_88)
#define data_00290a58 (gSzDuel_Icon_bytes + 252 * 10)
#define data_00290a62 (gSzDuel_Icon_bytes + 253 * 10)
#define data_00290a6c (gSzDuel_Icon_bytes + 254 * 10)
#define data_00290896 (gSzDuel_Icon_bytes + 207 * 10)
#define data_002908a0 (gSzDuel_Icon_bytes + 208 * 10)
#define data_002908e6 (gSzDuel_Icon_bytes + 215 * 10)
#endif
/* Passed (as an int) as the colour of the captions when the hint flag is set; never here. */
extern unsigned char data_002be944[];
#ifdef __MWERKS__
#define DOTR_SELDECK_HINT_COLOUR ((int)data_002be944)
#else
#include <stdint.h>
#define DOTR_SELDECK_HINT_COLOUR ((int)(intptr_t)data_002be944)
#endif
DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(DotrSpriteWord cursor, int side, int x, int y);
/* 0x1c9c60 lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe (original name): the same window with
 * the side's summon-charge byte at 0xa9c006 between icons 207 and 208 (rows of gSzDuel_Icon_bytes at
 * 0x290896 / 0x2908a0) and icon 215 (0x2908e6); same macros and assumptions as above. */
DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(DotrSpriteWord cursor, int side, int x, int y);

/* 0x1bf2a0 lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe (name from the Japanese build's symbol
 * table). Lists the enchant cards of monster `id` in the deck detail panel: nothing for an
 * id of 683 or more (683 is the monster count; it is the only bound on the id), otherwise
 * one line per entry TaEnc_GetEnchantLst returns (its count taken as a byte), each a board
 * 262 x 26 with the name scaled to 254 wide. The last parameter is forwarded as
 * lSzDuel_DrawWindow_ExBoard_Pipe's second argument; its meaning is assumed.
 * Views of this caller (they link by address): 0x249410 TaEnc_GetEnchantLst with an unsigned char result,
 * 0x1feab0 SzMsg_GetInfo as an int pointer (word 3 is the width), 0x1fe660 SzMsg_DrawRatioX_Pipe
 * with the two scales before the fifth word and the callback. SzDuel_GetMsgName as in
 * resource_support.h. Assumed: these types. MATCHING DEVICE: the fifth word is the address
 * of gSzDuel_aicolMsg_bytes cast to an integer. */
extern unsigned char func_00249410_enchant(unsigned short id, unsigned short *list);
extern int *func_001feab0_enchant(int index);
#ifdef __MWERKS__
typedef int DotrSpriteAddr;
#else
typedef long DotrSpriteAddr; /* host: wide enough for the address of gSzDuel_aicolMsg_bytes */
#endif
extern DotrSpriteWord func_001fe660_enchant(DotrSpriteWord cursor, int index, int x, int y, float sx, float sy,
                                           DotrSpriteAddr e, void (*function)(void));
extern int SzDuel_GetMsgName(int cardNo);
DotrSpriteWord lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(DotrSpriteWord arg0, int cardId, int arg2, int arg3, int arg4, int arg5);

/* 0x1bf470 lSzDuel_DrawBuildDeck_Detail_Ability_Pipe (name from the Japanese build's symbol table).
 * Lists the
 * leader abilities of monster `id` in the deck detail panel: nothing for an id of 683 or more (683
 * is the monster count; the only bound on the id). It fills a card record for the id, gives it the
 * side byte at 0x3181ec and the monster's five float pairs from the table at 0x2c3338 (40 bytes per
 * monster, directly after the card table), loads the thirteen ability names (strings 101-113 into
 * slots 1-13), and for each ability 4-16 the record has draws an icon, a board 236 x 26 and the
 * name scaled to at most 226 wide, one row of 26 per ability shown. Assumed: the record as 0x60
 * bytes with the pairs at +8 and the side at +0x44 (state_records.h has the same record with other
 * member types); the last parameter as in the Enchant function. */
struct DotrSpriteAbilityPair {
    float unknown_00;
    float unknown_04;
};
struct DotrSpriteAbilityItem {
    void *unknown_00;
    void *unknown_04;
    struct DotrSpriteAbilityPair pair[5];
    unsigned char unknown_30[0x14];
    unsigned char owner;
    unsigned char unknown_45[0x1b];
};
extern struct DotrSpriteAbilityPair EffIdx_L_ability[][5];
extern unsigned char DeckBuildingSide_ability;
extern void func_001dcd70_ability(struct DotrSpriteAbilityItem *item, int id);
extern int func_001e0c80_ability(struct DotrSpriteAbilityItem *item, int which, int *out, int flag);
/* The function also declares SzMsg_DrawRatioX_Pipe in its body as func_001fe660_ability (int, int,
 * int, int, float, float, int, void (*)(void)), with the scales before the colour. */
DotrSpriteWord lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(DotrSpriteWord cursor, int cardNo, int x, int y, int flag, int side);

/* 0x1bff00 lSzDuel_DrawBuildDeck_BagCursor_Pipe (name from the Japanese build's symbols): the
 * bag cursor pane of the deck builder, row `cur` (0x36 pixels per row) at (x, y). Reads the
 * halfword at 0x317490 + 0xc96 (nothing past the outer frame when it is non-zero) and the byte
 * at 0x3181ed (== 3 picks the second caption). Icons are rows 251, 253, 252, 255 and 202 of
 * gSzDuel_Icon_bytes (0x290a4e, 0x290a62, 0x290a58, 0x290a76, 0x290864), named by their own
 * addresses under __MWERKS__ as above; the captions are character data at 0x2befe7, 0x2befec,
 * 0x2beff3 and 0x2beffb. Assumed: the meaning of the halfword and the byte; the parameter
 * meanings (build_deck_draw.h's caller view names them x, y, cur). */
extern char data_002befe7[];
extern char data_002befec[];
extern char data_002beff3[];
extern char data_002beffb[];
extern unsigned char BuildDeckHorizontalCursorIndex_bag;
#ifdef __MWERKS__
extern char data_00290a4e[];
extern char data_00290a76[];
extern char data_00290864[];
#else
#define data_00290a4e (gSzDuel_Icon_bytes + 251 * 10)
#define data_00290a76 (gSzDuel_Icon_bytes + 255 * 10)
#define data_00290864 (gSzDuel_Icon_bytes + 202 * 10)
#endif
DotrSpriteWord lSzDuel_DrawBuildDeck_BagCursor_Pipe(DotrSpriteWord id, int arg1, int arg2, int arg3);

/* 0x1ca0e0 lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe and 0x1ca450
 * lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe (original names): near twins of the SelectDeck
 * window with two choice captions (0x2bf066 / 0x2bf062 and 0x2bf056 / 0x2bf051) coloured by the
 * side's byte at 0xa9c008 or 0xa9c00c. Here the side parameter itself is a byte pointer
 * (declared as an int, the same code compiles with other registers and
 * a larger frame), and it is passed on as an int. MATCHING DEVICE: DotrSelDeckSide is that
 * pointer under __MWERKS__ and a host integer otherwise (kind_calls.h's caller view passes an
 * int); DOTR_SELDECK_SIDE_BYTE indexes it directly under __MWERKS__ (a cast, even to the same
 * type, changes the allocation) and the fixture block on the host. SzMsg_DrawX_Pipe (0x1fe7d0,
 * slot_table) is called with an integer fifth argument. Assumed: argument meanings. */
#ifdef __MWERKS__
typedef unsigned char *DotrSelDeckSide;
#define DOTR_SELDECK_SIDE_BYTE(side, address) ((side)[(address)])
#else
typedef long DotrSelDeckSide;
#define DOTR_SELDECK_SIDE_BYTE(side, address) DOTR_SELDECK_BYTE(side, address)
#endif
extern char data_002bf051[];
extern char data_002bf056[];
extern char data_002bf062[];
extern char data_002bf066[];
extern DotrSpriteWord func_001fe7d0_sprite(DotrSpriteWord cursor, int index, int x, int y, int e, void (*function)(void));
DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe(DotrSpriteWord arg0, DotrSelDeckSide arg1, int x, int y);
DotrSpriteWord lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe(DotrSpriteWord arg0, DotrSelDeckSide arg1, int x, int y);

#endif
