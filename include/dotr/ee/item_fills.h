#ifndef DOTR_EE_ITEM_FILLS_H
#define DOTR_EE_ITEM_FILLS_H

/* SzDuel_InitCardInfo (0x1dcd70): fills one 92-byte item block (the DotrStateItem layout) with
 * its defaults and the per-id table values. Other groups declare it with a 96-byte block
 * parameter; this header has its own typed view.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* View of the object SzDuel_GetCardConstInfo answers (state_grid.h declares it as a 20-byte
 * entry): the bytes this group reads. */
struct DotrFillSource {
    unsigned char kind;
    unsigned char kindOfs;
    unsigned char unknown_02_low : 4;
    unsigned char unknown_02_high : 4;
    unsigned char deckCost;
    unsigned short effectId;
};
struct DotrFillPair {
    int unknown_words[2];
};
/* The 92-byte item (state_records.h: DotrStateItem) with every member SzDuel_InitCardInfo
 * stores. +0x56 holds four two-bit fields, +0x57 eight one-bit fields, +0x58
 * four one-bit fields (byte accesses) followed by a six-bit field at bits 4-9
 * that is accessed as a halfword, hence the union. The leading pointers make
 * host offsets differ from the target's. */
struct DotrFillPairs {
    signed char unknown_bits_0_1 : 2;
    signed char unknown_bits_2_3 : 2;
    signed char unknown_bits_4_5 : 2;
    signed char unknown_bits_6_7 : 2;
};
struct DotrFillBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bit_4 : 1;
    signed char unknown_bit_5 : 1;
    signed char unknown_bit_6 : 1;
    signed char unknown_bit_7 : 1;
};
struct DotrFillFlags {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
};
struct DotrFillWide {
    short unknown_bits_0_3 : 4;
    short unknown_bits_4_9 : 6;
};
struct DotrFillItem {
    struct DotrFillSource *source;
    void *cardModel;
    struct DotrFillPair pairs[5];
    int unknown_30;
    short xOffsetTerrain;
    short yOffsetTerrain;
    short apBoost;
    short dpBoost;
    short id;
    unsigned char kind;
    unsigned char attr;
    unsigned char unknown_40[4];
    unsigned char team;
    unsigned char slot;
    unsigned char modelId;
    unsigned char unknown_47[6];
    unsigned char cardHighlight;
    unsigned char ctlModeC;
    unsigned char unknown_4f;
    unsigned char unknown_50[3];
    unsigned char unknown_53[3];
    struct DotrFillPairs unknown_56;
    struct DotrFillBits status;
    union {
        struct DotrFillFlags flags;
        struct DotrFillWide wide;
    } modelStatusFlags;
    unsigned char unknown_5a[2];
};
#ifdef __MWERKS__
typedef char DotrFillItemMustBe5c[(sizeof(struct DotrFillItem) == 0x5c) ? 1 : -1];
#endif

/* SzDuel_GetCardConstInfo (0x1dd0e0) and the tables at 0x2c9df0 (five pairs per row) and
 * 0x2cc5f0 (one pair per row) under this group's types; state_grid.h has the other
 * declarations. */
extern struct DotrFillSource *func_001dd0e0_fill(int id);
extern struct DotrFillPair EffIdx_M_fill[][5];
extern struct DotrFillPair EffIdx_S_fill[];

/* 0x1dcd70. Sets the item to its defaults (pointers 0, the five pairs -1,
 * id 999, the fixed bytes, all bit fields 0 and the signed six-bit field all
 * ones, written -1). Id 671 counts as 999. For 0 <= id < 854: stores the id and
 * the SzDuel_GetCardConstInfo source, copies its first byte and the low nibble of its third; unless
 * the source's halfword at +4 is 0xffff, copies row [halfword] of the 0x2c9df0 table (five
 * pairs) for id < 683, else the single pair of the 0x2cc5f0 table into the
 * first pair.
 * Assumed: the item parameter type (other groups declare a 96-byte block),
 * int id, no result. */
void SzDuel_InitCardInfo(struct DotrFillItem *unit, int cardId);

#endif
