#ifndef DOTR_EE_LIST_RESET_H
#define DOTR_EE_LIST_RESET_H

/* Functions that rebuild the work lists at the literal address 0x1ed0000 for one side from the
 * side_table queries and reset the related control-block fields at 0x317490.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Callees as declared in side_table.h and small_misc.h (those headers are not included
 * here). lSzDuel_SortDeck (0x1ba610, sort_keys) is assumed to take no arguments and its
 * result is not used here. */
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetOption(int side, int which);
extern int SzData_GetBagCount(int side, int cardNo, int kind);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzData_GetCardNo(int side, int deckNo, int cardSlot);
extern int SzData_CalcDeckCost(int side, int deckNo);
extern void lSzDuel_SortBag(void);
extern void lSzDuel_SortDeck(void);

/* View of the control block at 0x317490 with the fields 0x1b8520 writes. Field names
 * are offsets except where the function shows a use: the four pointers it stores, the
 * two counters it increments and the side byte. On the host the pointers are wider, so
 * the offsets hold for the target build only. */
struct DotrListControl {
    unsigned char unknown_000[0x6d0];
    char *marks;
    unsigned char *table;
    short *ids;
    short *entries;
    unsigned char unknown_6e0[0xc58 - 0x6e0];
    int word_c58;
    unsigned char unknown_c5c[0xc8a - 0xc5c];
    unsigned short count_c8a;
    unsigned short id_count;
    unsigned short count_c8e;
    unsigned short entry_count;
    unsigned short count_c92;
    unsigned short count_c94;
    unsigned short count_c96;
    unsigned short count_c98;
    unsigned char unknown_c9a[0xcb6 - 0xc9a];
    unsigned char byte_cb6, byte_cb7, byte_cb8, byte_cb9, byte_cba, byte_cbb, byte_cbc, byte_cbd;
    unsigned char unknown_cbe[0xd32 - 0xcbe];
    unsigned char byte_d32;
    unsigned char unknown_d33[0xd5c - 0xd33];
    unsigned char side, byte_d5d, byte_d5e, byte_d5f, byte_d60, byte_d61, byte_d62;
};
/* A view symbol of its own (same address as the other data_00317490_* views): the
 * function writes +0xc58 through the symbol and everything else through a pointer,
 * and a cast of the byte declaration does not reproduce that. */
extern struct DotrListControl data_00317490_lists;
#define DOTR_LIST_CONTROL (&data_00317490_lists)

/* Stored in the control block's table pointer; contents not examined. */
extern unsigned char gSzDuel_NameIndex1[];
extern unsigned char gSzDuel_NameIndex0[];

/* The work area at the literal address 0x1ed0000 (the function uses lui/ori
 * constants, not relocated symbols): 854 halfword ids, then 41 halfword entries at
 * +0x6ac, then eight 4-byte records at +0x6fe. The host build uses an object. */
#ifdef __MWERKS__
#define DOTR_LIST_IDS ((short *)0x1ed0000)
#define DOTR_LIST_ENTRIES ((short *)0x1ed06ac)
#define DOTR_LIST_MARKS ((char *)0x1ed06fe)
#else
extern short dotr_list_work_01ed0000[];
#define DOTR_LIST_IDS (dotr_list_work_01ed0000)
#define DOTR_LIST_ENTRIES (dotr_list_work_01ed0000 + 0x6ac / 2)
#define DOTR_LIST_MARKS ((char *)dotr_list_work_01ed0000 + 0x6fe)
#endif

/* 0x1b8520: owner = SzData_GetCurrentDeck(side); control word +0xc58 = 0xfffff8; table pointer =
 * 0x2cd1f4 when SzData_GetOption(0, 0) is not zero, else 0x2ccb48; stores the three work-area
 * addresses; ids[i] = i for each id below 854 with SzData_GetBagCount(side, i, -1) > 0 (counted
 * in +0xc8c), else 999; entries[0] = SzData_GetLeader(side, owner), entries[1..40] =
 * SzData_GetCardNo(side, owner, 0..39) (41 counted in +0xc90); clears byte 2 of the eight
 * records; resets the listed control fields (+0xcb7 and +0xcba = 4, +0xcb8 and
 * +0xcbb = 10, +0xd5d = 3, +0xd5c = side, the rest 0); calls lSzDuel_SortBag(), lSzDuel_SortDeck(),
 * SzData_CalcDeckCost(side, owner); clears +0xd32. Assumed: one int argument, no result. */
void SzDuel_InitBuildDeck(int side);

#endif
