#ifndef DOTR_EE_FREE_SPOTS_H
#define DOTR_EE_FREE_SPOTS_H

/* Per-logic copies of the summon position picker (TaLogicNNN_GetSummonPiece and unnamed
 * siblings): each picks one of eight 0x24e410 positions for a unit, the cheapest reachable
 * empty one, else (by copy, the first or the last) one whose 0x1e3790 answer is negative.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The unit these copies are handed: only the kind byte at +0x3e is read here
 * (its top three bits must be zero for the first stage), and 0x115340 also
 * reads the card number at +0x3c. */
struct DotrFreeBlock {
    unsigned char unknown_00[0x3c];
    unsigned short cardNo;
    unsigned char kind;
};
/* Twelve local bytes whose two nibbles are both set to 15 before use (the
 * layout position_rules.h uses for the same callees). */
struct DotrFreeNibbles {
    unsigned char low : 4;
    unsigned char high : 4;
};
struct DotrStateItem;

/* Shared callees, declared with the arguments these callers pass. The
 * `_free` names are views of functions defined elsewhere with other
 * parameter types (0x24fe40, 0x278510, 0x2792c0, 0x27afc0, and 0x27a130 with
 * ten arguments, the last two read from the stack).
 * Argument meanings are assumed. */
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_GetSommonPiecePreference(int a, int direction);
extern int SzDuel_GetField(int x, int y);
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int func_0024fe40_free(struct DotrFreeBlock *unit, int x, int y);
extern int func_00278510_free(struct DotrFreeBlock *unit, int ownerId, int c, int d, int e, int skip);
extern int func_0027a130_free(struct DotrFreeBlock *unit, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                              int g, int h, unsigned char i, unsigned short key);
extern int func_002792c0_free(struct DotrFreeBlock *unit, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                              unsigned short id, int n);
extern int func_0027afc0_free(struct DotrFreeNibbles *bytes);
/* The per-copy key callees: declared elsewhere as int (int side). */
extern int func_0015d120_free(int side);
extern int func_00162580_free(int side);
extern int func_00172310_free(int side);

/* -1 for a null unit. First stage, only when the unit's byte +0x3e is below
 * 0x20 and ownerId is not negative: for each of the eight Ta_GetSommonPiecePreference(side, i)
 * positions that is not negative, whose 0x1e22b0 answer is not 8, is not 9
 * with Ta_IsVirus_Chk3(unit, x, y) == 1, and whose 0x1e36e0 cell is null, a cost
 * comes from TaLogic_GetInvadeRoot2 (when TaLogic_IsUnitWin4(unit, ownerId, key(side), here) is
 * positive) or from 0x2792c0; the position with the lowest cost below 255 for
 * which TaLogic_GetMinGoalCount(list) is positive is returned. Otherwise the first position
 * passing the first three tests whose 0x1e3790 answer is negative is
 * returned, else -1. The second argument is not read; its type is taken from
 * the step_picks callers. The three differ only in the key callee:
 * 0x15d120, 0x162580 and 0x172310. */
int TaLogic018_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);
int TaLogic019_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);
int TaLogic022_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* Same first stage with fixed arguments in place of a key callee (0x278510 is
 * given 0 and 1, 0x27a130 gets 1, 2 and 0, 0x2792c0 gets 2). The second stage
 * differs: it returns the LAST position passing the first three tests whose
 * 0x1e3790 answer is negative, else -1. Parameter types assumed as above. */
int TaLogic009_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* The level is the unit's byte +0x3e shifted right by five (stored in the
 * otherwise unread second parameter). Level 0 or 5: the first stage when
 * ownerId is not negative (key callee 0x16b0e0; 0x278510 gets 1, 0x27a130 gets
 * 1 and 4, 0x2792c0 gets 4), then SzDuel_GetUnitLoc(ownerId) is asked again with its
 * answer unused and the FIRST position passing the three tests with a negative
 * 0x1e3790 answer is returned, else -1. Any other level: the LAST such
 * position whose row (spot >> 8) is below 8, else -1. */
extern int func_0016b0e0_free(int side);
int TaLogic021_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* 0x115340: -1 for a null unit. When the unit's level (+0x3e >> 5) is 0
 * and its +0x3c word is not 83: the 0x169610 first stage with the key
 * TaLogic_GetWarnHideLvl__00117c30(side), skip flag 0 and count 5; when that takes nothing (or ownerId
 * is negative), every free position with a negative 0x1e3790 id is costed
 * with TaLogic_GetRoot2 (count 7) towards (3, 3), or, when CheckLandAgreement(3, unit) is 1,
 * towards the corner the body of 0x113b60 gives for the position, passing the
 * id found there (0xffff when negative); the lowest cost from 0 to 13 wins,
 * else -1. Otherwise: the last free position of kind neither 8 nor 9 with a
 * negative id and a row below 8, else -1.
 * Assumed: the 0x169610 signature (kind is overwritten before use); the
 * +0x3c word's type; 0x24deb0 is handed the unit as its item (cast at the
 * use). MATCHING DEVICE: both later stages are written as inline helpers, which
 * gives the original register assignment. */
extern int TaLogic_GetWarnHideLvl__00117c30(int side);
/* 0x1676d0: asked with the side; its result is handed on as a key (position_rules.h declares it). */
extern int TaLogic_GetWarnHideLvl__001676d0(int side);
/* Assumed like 0x115340: the second argument is overwritten by the level bits before use. */
int TaLogic020_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);
extern int CheckLandAgreement(int field, struct DotrStateItem *unit);
int TaLogic001_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* 0x144a60: the key callee is called with the side; declared here as position_rules.h has it. */
extern int TaLogic_GetWarnHideLvl__001461c0(int side);
int TaLogic012_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* 0x12e3f0 TaLogic007_GetSummonPiece (name from the Japanese build's symbols): logic 007's copy.
 * Assumed: the same parameters as its siblings; the second parameter is not read. The key
 * callee is declared in position_rules.h and again here for this caller. */
extern int TaLogic_GetWarnHideLvl__00130780(int side);
int TaLogic007_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* 0x123010: the shape of 0x144a60 with this copy's callees. A negative ownerId value
 * takes the position whose 0x123500 answer byte (+4 of an eight-byte local) is lowest
 * and below 14, else the first position of kind 4. 0x123500 is handed the unit where
 * position_rules.h declares a word; the key callee is called with the side. */
struct DotrFreeSpotBest {
    int spot;
    unsigned char cost;
};
extern int TaLogic_GetSommonPiecePreference__00124f50(int side, int slot);
extern int func_001254a0_free(int side);
extern void func_00123500_free(struct DotrFreeBlock *unit, int column, int row, struct DotrFreeSpotBest *best);
int TaLogic005_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId);

/* 0x11e270: 0x123010 without the ownerId argument and without the kind-4 pass. Only a
 * +0x3e value of 0 runs the first stage; the ownerId value is 0x276f10's answer for the
 * side's SzDuel_GetUnitLoc(side, 0) position (ten arguments as step_picks.h declares them, the
 * unit first), and both cost callees are handed the count 4. */
extern int TaLogic_GetSommonPiecePreference__0011f530(int side, int slot);
extern int func_0011fa80_free(int side);
extern void func_0011d4e0_free(struct DotrFreeBlock *unit, int column, int row, struct DotrFreeSpotBest *best);
extern int func_00276f10_free(struct DotrFreeBlock *unit, int a, int b, int c, int d, int e, int f, int g, int h, int i);
int TaLogic004_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit);

/* 0x119520: 0x11e270 with the 0x11a4c0 slots and the 0x11aa10 key, the count 3 and no
 * negative cost taken in the first stage. A negative ownerId answer costs every free
 * position with a negative 0x1e3790 id towards the place 0x118dd0's inlined body picks
 * (0x2792c0, id 0xffff, count 7) and returns the cheapest below 14. Any other +0x3e
 * value skips kind 9 without asking 0x24fe40. Types assumed as for 0x11e270; the key
 * callee is declared as lower_rules.h does. */
extern int TaLogic_GetSommonPiecePreference__0011a4c0(int side, int slot);
extern int func_0011aa10_key(int key);
extern int TaLogic003_IsEmptyAtkLine(int side, unsigned char lineIndex);
int TaLogic003_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit);

/* 0x10e890 TaLogic002_GetSummonPiece: logic 002's copy. It asks 0x1e1090 for (side, 0)
 * and drops the answer; kinds 0, 3 and 4 (+0x3e >> 5) take the costed stage (count 2,
 * 0x278510 asked once before the loop and tested `>= 0`, the twelve bytes reset for
 * every position, a negative cost never taken) or, with a negative ownerId value, up to
 * six (kind 0) or four line places from TaLogic002_GetLinePiece (mode 1 for kind 0, else 2), each
 * costed with count 7 and answered at the first round with a cost below 14. Other kinds
 * return the last free position that is not field 8 or 9 and has a negative 0x1e3790
 * id. `kind` is written before it is read, so its incoming type is assumed (int here:
 * the comparisons use the register without masking again). */
extern int TaLogic_GetWarnHideLvl__0010eda0(int side);
extern int TaLogic_GetSommonPiecePreference__0010ed90(int side, int slot);
extern int TaLogic002_GetLinePiece(unsigned char lineType, unsigned char lineIndex);
int TaLogic002_GetSummonPiece(int side, int kind, struct DotrFreeBlock *unit, int ownerId);
/* 0x151cf0 and 0x1491b0: the level byte goes through SzDuel_Kind2Type. Their second
 * parameter is never read; `long` is what reproduces the original's register copy. */
extern int SzDuel_Kind2Type(int kind);
extern int TaLogic_GetWarnHideLvl__00153ed0(int side);
int TaLogic015_GetSummonPiece(int side, long kind, struct DotrFreeBlock *unit, int ownerId);
extern int TaLogic_GetWarnHideLvl__0014b330(int side);
/* Assumed: returns a packed place (row << 8 | column) for index 0..4. */
extern int TaLogic013_GetDefensePoint(int index);
int TaLogic013_GetSummonPiece(int side, long kind, struct DotrFreeBlock *unit, int ownerId);

#endif
