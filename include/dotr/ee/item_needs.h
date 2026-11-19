#ifndef DOTR_EE_ITEM_NEEDS_H
#define DOTR_EE_ITEM_NEEDS_H

/* Functions that read the halfword pairs at +0x08..+0x2b of an item and weigh them against a
 * callee answer (first member 0x1e0c80).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Item view for SzDuel_GetUnitLeaderAbility3 (the 0x5c-byte item other groups call DotrStateItem;
 * its two leading words are pointers there, so this group keeps its own
 * pointer-free view). Nine words at +0x08, each read as a low and a high
 * halfword; the halfword at +0x3c and the byte at +0x44 are passed to
 * 0x215d30. Other fields are unknown. */
struct DotrNeedItem {
    int unknown_00[2];
    unsigned int leaderAbilities[9];
    unsigned char unknown_2c[0x10];
    unsigned short cardNo;
    unsigned char unknown_3e[6];
    unsigned char team;
    unsigned char unknown_45[0x17];
};
typedef char DotrNeedItemMustBe5c[(sizeof(struct DotrNeedItem) == 0x5c) ? 1 : -1];

/* Callees, declared as state_grid.h and side_table.h do. */
extern int SzGame_GetMenuNum(int unknown);
extern int SzData_GetRank(int side, int cardNo);

/* 0x1e0c80. Clears a non-null *extraDataOut. Answers 0 for a null unit, for an id
 * (+0x3c) of 854 or more or equal to 671, and, when `ignoreMenu` is 0, when bit
 * (+0x44) of SzGame_GetMenuNum(28) is clear. Then switches on `ability` through the
 * 17-entry table at 0x2bed90: case 2k reads the high halfword and case 2k+1
 * the low halfword of the word at +0x08 + 4k. A halfword of 0xffff answers
 * 0. Cases 0 and 1 answer whether the halfword is not 0. The others split
 * it into rankRequirement = halfword >> 8 (cases 7, 8, 10 and 14: the low byte,
 * value = high byte; case 2: value = low byte; value stays 0 elsewhere),
 * store value through a non-null extraDataOut and answer 1 when
 * SzData_GetRank(+0x44, +0x3c) is at least rankRequirement, else 0. Other `ability` answer 0.
 * Assumed: int ability / ignoreMenu / result, int *extraDataOut. */
int SzDuel_GetUnitLeaderAbility3(struct DotrNeedItem *unit, int ability, int *extraDataOut, int ignoreMenu);

#endif
