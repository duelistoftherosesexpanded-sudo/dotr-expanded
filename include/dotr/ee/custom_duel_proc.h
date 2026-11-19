#ifndef DOTR_EE_CUSTOM_DUEL_PROC_H
#define DOTR_EE_CUSTOM_DUEL_PROC_H

/* lSzDuel_ActCustomDuelProc, the per-frame input handler of one player's custom-duel setup menu
 * over the settings block at 0xa9c000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* View of the duel control block at 0x317490 with the three places 0x1c6170 uses: each
 * side's deck byte and the two reload bytes. Same layout as DotrCustomDuelControl in
 * small_misc.h (the view of SzDuel_InitCustomDuel). Assumed: every name; `reload` as "this
 * side's deck text must be loaded again" (set here after a deck change, cleared after
 * SzText_LoadMerge). */
struct DotrCustomProcControlSide {
    unsigned char unknown_000[0x33c];
    unsigned char deck;                 /* 0x33c */
    unsigned char unknown_33d[0x13];
};
struct DotrCustomProcControl {
    struct DotrCustomProcControlSide side[2];
    unsigned char unknown_6a0[0x692];
    unsigned char reload[2];            /* 0xd32, 0xd33 */
};
/* The function reaches the block by casting a byte-array symbol. On the host the symbol is
 * a cast of one fixture object. */
#ifdef __MWERKS__
extern signed char data_00317490_customproc[];
#else
extern struct DotrCustomProcControl dotr_custom_proc_control;
#define data_00317490_customproc ((signed char *)&dotr_custom_proc_control)
#endif

/* The custom-duel settings at the literal address 0xa9c000, as this function uses them
 * with fixed indexes. Same layout as DotrCustomDuel in small_misc.h, with the life points
 * signed (they are read with sign extension here) and three more bytes named. Index 0 and
 * 1 are the two players. Assumed: every name. `menu`: item in the low nibble (0 deck,
 * 1 life points, 2 summon charge, 3 and 4 two on/off settings, 5 a submenu, 6 memory card,
 * 7 map), level in the high nibble (0 browsing, 1 editing the item, 2 waiting for the
 * other player, 3 the quit yes/no box, 4 picking from the list). `cur`: cursor cell in the
 * 2x5 item grid. `f8`/`fa` and `fc`/`fe`: the edited and the accepted value of the two
 * on/off settings. `lp`: edited life points, `lp2`: accepted. `list`: entries of the pick
 * list (class in the high byte, number in the low byte), `count` of them, `last` the
 * cursor. `map` of `maps`: chosen map. `yes`: cursor of the quit box. */
struct DotrCustomProc {
    unsigned char cur[2];               /* 0x00 */
    unsigned char menu[2];              /* 0x02 */
    unsigned char deck[2];              /* 0x04 */
    unsigned char charge[2];            /* 0x06 */
    unsigned char f8[2];                /* 0x08 */
    unsigned char fa[2];                /* 0x0a */
    unsigned char fc[2];                /* 0x0c */
    unsigned char fe[2];                /* 0x0e */
    short lp[2];                        /* 0x10 */
    short lp2[2];                       /* 0x14 */
    unsigned short list[56];            /* 0x18 */
    unsigned char mode;                 /* 0x88 */
    unsigned char last;                 /* 0x89 */
    unsigned char count;                /* 0x8a */
    unsigned char f8b;                  /* 0x8b */
    unsigned char map;                  /* 0x8c */
    unsigned char maps;                 /* 0x8d */
    unsigned char yes;                  /* 0x8e */
};

/* The same block as the function reaches it for "this player": the original adds the
 * player index to the block's address once and then uses fixed offsets, so every per-side
 * byte is reached at its array's offset from a pointer that is the block's address plus
 * the index. MATCHING DEVICE: under the target compiler that is spelled as a structure with
 * 0xa9c000 bytes of padding and the index itself (for the two halfword arrays, twice the
 * index) cast to a pointer to it. The host build has no such address: there the same
 * macros add the index to the address of one fixture object, dotr_custom_proc_work (a
 * struct DotrCustomProc), and the padding is absent. The host uses a second structure for
 * the halfwords so that the byte view, which is used at odd addresses for player 1, has no
 * alignment requirement. */
#ifdef __MWERKS__
struct DotrCustomProcAt {
    unsigned char pad[0xa9c000];
    unsigned char cur;
    unsigned char pad01;
    unsigned char menu;
    unsigned char pad03;
    unsigned char deck;
    unsigned char pad05;
    unsigned char charge;
    unsigned char pad07;
    unsigned char f8;
    unsigned char pad09;
    unsigned char fa;
    unsigned char pad0b;
    unsigned char fc;
    unsigned char pad0d;
    unsigned char fe;
    unsigned char pad0f;
    short lp;
    short pad12;
    short lp2;
};
#define DOTR_CUSTOM_PROC ((struct DotrCustomProc *)0xa9c000)
#define DOTR_CUSTOM_PROC_BYTES_AT(i) ((struct DotrCustomProcAt *)(i))
#define DOTR_CUSTOM_PROC_WORDS_AT(i) ((struct DotrCustomProcAt *)((i) * 2))
#else
struct DotrCustomProcAt {
    unsigned char cur;
    unsigned char pad01;
    unsigned char menu;
    unsigned char pad03;
    unsigned char deck;
    unsigned char pad05;
    unsigned char charge;
    unsigned char pad07;
    unsigned char f8;
    unsigned char pad09;
    unsigned char fa;
    unsigned char pad0b;
    unsigned char fc;
    unsigned char pad0d;
    unsigned char fe;
    unsigned char pad0f;
};
struct DotrCustomProcWordsAt {
    unsigned char pad[0x10];
    short lp;
    short pad12;
    short lp2;
};
extern struct DotrCustomProc dotr_custom_proc_work;
#define DOTR_CUSTOM_PROC (&dotr_custom_proc_work)
#define DOTR_CUSTOM_PROC_BYTES_AT(i) ((struct DotrCustomProcAt *)((unsigned char *)&dotr_custom_proc_work + (i)))
#define DOTR_CUSTOM_PROC_WORDS_AT(i) ((struct DotrCustomProcWordsAt *)((unsigned char *)&dotr_custom_proc_work + (i) * 2))
#endif

/* What SzData_GetData(side) returns, as far as the map item reads it: 7x7 cell bytes per
 * map from +0xfea3. Assumed: the names and the element type; the first dimension is 1
 * because the number of maps is not known from this function (it indexes up to
 * `maps` - 1). */
struct DotrCustomProcSave {
    unsigned char pad[0xfea3];
    unsigned char map[1][7][7];
};

/* The 2x5 item grid by mode (index 0: mode 0, index 1: any other mode): the item in each
 * cursor cell, 255 for no cell. Read-only data of the executable at 0x2beac0, just before
 * this function's two jump tables. Assumed: the dimensions (from the indexing only). */
extern unsigned char SzDuel_CustomDuelMenu_customproc[2][2][5];

/* Callees as this function calls them. SzPad_Check(pad, kind, mask) as in build_deck.h
 * (kind 0 pressed now, 2 with repeat: assumed). The others are declared as in the groups
 * that define them (side_table.h, state_records.h, state_grid.h, menu.h,
 * movie_operations.h, kind_dispatch.h, memcard_proc.h), except SzData_GetData, which
 * small_misc.h declares with another structure pointer as its result and which is
 * declared here with a plain pointer result. For SzText_LoadMerge two int
 * arguments are assumed, as in build_deck.h. */
extern int SzData_GetCurrentDeck(int side);
extern int SzFile_InqRequestEx(void);
extern int SzText_LoadMerge(int a, int b);
extern int SzPad_Check(int pad, int kind, int mask);
extern int SzSound_PlaySE(int a, int soundEffectNumber);
extern int SzData_GetDeckCount(int side, int deckNo);
extern int SzData_GetLeader(int side, int deckNo);
extern void SzData_SetCurrentDeck(int side, int deckNo);
extern void SzText_Free(int a, int b);
extern int SzDuel_GetLP(int side);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern int SzDuel_GetSummonCharge(int side);
extern void SzDuel_SetSummonCharge(int side, int value);
extern int SzGame_ActMemCardProc(void);
extern void *SzData_GetData(int side);
extern int SzDuel_SetField(int x, int y, int value);
extern int SzGame_SetMenuNum(int index, int value);
extern void SzGame_Push(int a, int b);
extern void SzGame_Pop(void);
extern int SzData_GetMapEditFlag(int side);
extern void SzGame_InitMemCardProc(int kind);

/* 0x1c6170 lSzDuel_ActCustomDuelProc (name from the Japanese build's symbol table;
 * prototype as small_misc.h declares it for its caller SzDuel_ActCustomDuel). One frame of
 * player `side`'s custom-duel setup menu (side is 0 or 1 and is also the pad asked).
 * Returns 1 only when a choice from the pick list is confirmed (level 4), otherwise 0. */
int lSzDuel_ActCustomDuelProc(int side);

#endif
