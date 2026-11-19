#ifndef DOTR_EE_TRADE_PROC_H
#define DOTR_EE_TRADE_PROC_H

/* The trade screen's per-frame input handler for one side: one state machine over that side's state
 * byte in the duel control block.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* One side's first shown row and row count. Assumed: both names. */
struct DotrTradeProcPair {
    unsigned short top;
    unsigned short count;
};

/* View of the duel control block at 0x317490 with the fields 0x1c2720 uses; index 0 and 1 are
 * the two sides. Names are this function's uses of them; build_deck.h, trade_scene.h,
 * list_reset.h and small_misc.h have other views of the same block under their own symbols.
 * The block has pointer members, so the offsets hold for the target build only (4-byte
 * pointers); the host view is wider. Assumed: every name; lists as unsigned halfwords (the
 * function reads them with a zero-extending load); the extent of 854 count bytes per side
 * (from the summing loops only); keys as three sort keys per side, of which this function
 * touches the first two. */
struct DotrTradeProcControl {
    unsigned char pad000[0x6d8];
    unsigned short *lists[2];           /* 0x6d8: card ids in list order, 999 for an empty row */
    unsigned char *counts[2];           /* 0x6e0: copies offered, by card id */
    unsigned char pad6e8[0xc8a - 0x6e8];
    struct DotrTradeProcPair pairs[2];  /* 0xc8a */
    short slide[2];                     /* 0xc92: 0 the list, 320 the detail panel */
    unsigned short idle[2];             /* 0xc96: 90-frame idle countdown */
    unsigned char padc9a[0xcb6 - 0xc9a];
    unsigned char keys[2][3];           /* 0xcb6 */
    unsigned char tab[2];               /* 0xcbc */
    unsigned char state[2];             /* 0xcbe */
    unsigned char modes[2];             /* 0xcc0: which list is shown (0-2) */
    unsigned char padcc2[0xd36 - 0xcc2];
    unsigned char confirm;              /* 0xd36: yes/no cursor of the two confirm boxes */
    unsigned char padd37[0xd5e - 0xd37];
    unsigned char rows[2];              /* 0xd5e: cursor row, 0-4 */
};
extern struct DotrTradeProcControl data_00317490_tradeproc;
#define DOTR_TRADE_PROC data_00317490_tradeproc

/* Callees as this function calls them (argument counts from the call sites; results used as
 * shown). SzPad_Check(pad, kind, mask): kind 0, 1 and 2 are three ways of asking about the
 * buttons in mask (assumed, as in build_deck.h: pressed now, held, pressed with repeat).
 * lSzDuel_SetupTrade, lSzDuel_SortTrade and lSzDuel_TradeDo are declared as in small_misc.h,
 * SzGame_InitMemCardProc as in kind_dispatch.h (the groups that define them). */
extern int SzPad_Check(int pad, int kind, int mask);
extern int SzSound_PlaySE(int a, int soundEffectNumber);
extern int SzPad_IsAnalog(int pad);
extern int SzPad_GetAnalogInfo(int pad, int which);
extern int SzData_GetBagCount(int side, int cardNo, int kind);
extern void lSzDuel_SetupTrade(int flag);
extern void lSzDuel_SortTrade(int flag);
extern void SzGame_Pop(void);
extern int lSzDuel_TradeDo(void);
extern void SzGame_InitMemCardProc(int kind);

/* 0x1c2720 lSzDuel_ActTradeProc (name from the Japanese build's symbol table; prototype as
 * pad_toggles.h declares it for its caller SzDuel_ActTrade). One frame of the trade screen
 * for side `which` (0 or 1, also the pad asked): a state machine over state[which]. 0 and 2
 * slide the panel, 1 is the card list, 3 the detail tab, 4 waits for the other side, 5 and 6
 * are the leave and trade yes/no boxes, 7 and 8 leave. Returns 1 only when a confirmed trade
 * reports 0 or less from lSzDuel_TradeDo (after SzGame_InitMemCardProc(5)), otherwise 0. */
int lSzDuel_ActTradeProc(int which);

#endif
