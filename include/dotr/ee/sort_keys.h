#ifndef DOTR_EE_SORT_KEYS_H
#define DOTR_EE_SORT_KEYS_H

/* Comparators at 0x1ba630-0x1bae6c over pairs of unsigned short ids, in the form the sort
 * routine 0x199bc0 calls, and the sort call at 0x1ba610.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The entry SzDuel_GetCardConstInfo (0x1dd0e0) returns for an id, as these comparators read it (state_grid.h
 * has it as five opaque words): a byte whose top three bits are one key, two
 * nibbles at +2, a byte at +3, and 13-bit fields in the halfwords at +8 and +10. */
struct DotrSortEntry {
    unsigned char kind;
    unsigned char kindOfs;
    unsigned char attr : 4;
    unsigned char level : 4;
    unsigned char deckCost;
    unsigned short effectId;
    unsigned short xaxId;
    unsigned short apWithFlags;
    unsigned short dpWithFlags;
};
struct DotrStateEntry20;
extern struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int cardNo);
/* SzDuel_Kind2Attr (resource_support.h has it as (value, fallback)). */
extern int SzDuel_Kind2Attr(int type, int fallback);
typedef int (*DotrSortCompare)(unsigned short *a, unsigned short *b);
/* 0x199bc0, called as a sort: base, element count, element size, comparator. */
extern void func_00199bc0(void *base, int count, int size, DotrSortCompare compare);
/* The address word at 0x317b6c (+0x6dc of the block at 0x317490 that other groups
 * view as a whole; the target reads this word by its own address): a list of
 * halfword ids whose entries 1..40 are sorted. */
extern unsigned short *data_00317b6c;
/* A missing entry sorts last in the ascending comparators. The constant has type
 * long in the target (each key is narrowed to 32 bits after the choice). */
#define DOTR_SORT_MISSING 0x7fffffffL
/* SzData_GetRank (side_table.h): a count for an id on a side. */
extern int SzData_GetRank(int side, int cardNo);
/* The byte at 0x3181ec, passed to SzData_GetRank and SzData_IsBagNew as the side. */
extern unsigned char DeckBuildingSide;

/* SzData_IsBagNew (side_table.h): a yes/no answer for an id on a side. */
extern int SzData_IsBagNew(int side, int cardNo);
/* The block at 0x317490 as the comparators read it: a pointer to halfword ranks
 * indexed by id at +1748, and two lists of three comparator indices at +3254 and
 * +3257. owner_draws.h declares the object; it is cast to this view at the use. */
struct DotrSortBlock {
    unsigned char unknown_000[1748];
    unsigned short *ranks;
    unsigned char unknown_6d8[3254 - 1752];
    unsigned char keys_a[3];
    unsigned char keys_b[3];
};
struct DotrOwnerDrawControl;
extern struct DotrOwnerDrawControl data_00317490;
/* The table at 0x291330: comparators indexed by the bytes of a key list. Its length
 * is not established. */
extern DotrSortCompare data_00291330[];

/* 0x1ba610. Sorts 40 halfwords starting one past the list address with the
 * comparator lSzDuel_SortFunc2 (a tail call). No arguments or result assumed. */
void lSzDuel_SortDeck(void);
/* 0x1ba630 / 0x1ba710: run the comparators named by the three key bytes at +3254
 * / +3257 in order, skipping a key byte equal to an earlier one, and answer the
 * first nonzero result (0 if none). */
int lSzDuel_SortFunc1(unsigned short *a, unsigned short *b);
int lSzDuel_SortFunc2(unsigned short *a, unsigned short *b);
/* 0x1ba7f0: ascending on 0 when SzData_IsBagNew(side byte, id) is nonzero, 1 when it is
 * zero, 2 for the id 999 (not passed on). */
int lSzDuel_NewComerCmp(unsigned short *a, unsigned short *b);
/* 0x1ba930: ascending on the halfword rank of the id; the id 999 is missing. */
int lSzDuel_NameCmp(unsigned short *a, unsigned short *b);
/* The comparators below take two pointers to halfword ids and answer 1, -1 or 0.
 * "Ascending" answers 1 when a's key is the larger; "descending" the reverse.
 * 0x1ba8a0: ascending on the entry's byte +0; a missing entry has the largest key. */
int lSzDuel_ClassCmp(unsigned short *a, unsigned short *b);
/* 0x1ba9c0: descending on the low 13 bits of the halfword +8; missing is -1. */
int lSzDuel_APCmp(unsigned short *a, unsigned short *b);
/* 0x1baa30: descending on the low 13 bits of the halfword +10; missing is -1. */
int lSzDuel_DPCmp(unsigned short *a, unsigned short *b);
/* 0x1baaa0: ascending on SzDuel_Kind2Attr(byte +0 >> 5, low nibble of byte +2). */
int lSzDuel_AttrCmp(unsigned short *a, unsigned short *b);
/* 0x1bab60: ascending on the high nibble of byte +2. */
int lSzDuel_LevelCmp(unsigned short *a, unsigned short *b);
/* 0x1bac00: ascending on byte +3. */
int lSzDuel_DeckCostCmp(unsigned short *a, unsigned short *b);
/* 0x1bac90: ascending on SzData_GetRank(side byte, id); an id of 683 or more is missing
 * and is not passed on. */
int lSzDuel_RankCmp(unsigned short *a, unsigned short *b);
/* 0x1bad40: ascending on byte +0 >> 5, plus 6 unless that is zero and the
 * halfword +4 is not 0xffff. */
int lSzDuel_CardColorCmp(unsigned short *a, unsigned short *b);
/* 0x1bae40: ascending on the ids themselves. */
int lSzDuel_NumberCmp(unsigned short *a, unsigned short *b);

#endif
