#ifndef DOTR_EE_ORDER_TRIES_H
#define DOTR_EE_ORDER_TRIES_H

/* AI rule functions (below the library block) that take an order record, query its position,
 * optionally test 0x27b200 and then call a stack forwarder up to a per-copy number of times,
 * storing a halfword at +4 by its answer.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The order record: a packed position (row in the high byte, column in the low
 * byte), a halfword at +4 and a word at +8 whose low halfword is also read on
 * its own. Same offsets as lower_rules.h's and position_rules.h's order views;
 * only these fields are shown by this code. */
struct DotrTryOrder {
    unsigned short position;
    unsigned short summonOrder;
    unsigned short unknown_04; /* two mission bytes, at +4 (low) and +5 (high) */
    unsigned short unknown_06;
    union {
        unsigned short half;
        int word; /* the high halfword is the goal location */
    } flags;
};
/* Local list entry: 22 four-byte entries get 0xffff at +0 and 0xff at +2. */
struct DotrTryCell {
    unsigned short id;
    unsigned char kind;
};

/* Callees, declared with the arguments these callers pass; results and
 * parameter types are assumed int unless a caller shows otherwise. Other group
 * headers hold different views of some of them. */
extern int func_001df540_word(int a, int b);
extern int func_0024de70_pair(int a, int b);
extern int SzDuel_GetUnitLoc(int side, int pos);
/* View of the empty function 0x27e770: these callers pass it a 112-byte local. */
extern void func_0027e770_buf(void *buf);
extern int TaLogic_IsRootOpen(int side, int pos, int c, int d, int e, int f, int g, int h, unsigned char maxDistance);
extern int TaLogic_GetWarnHideLvl__0011fa80(int side);
extern int TaLogic_GetWarnHideLvl__0012b1b0(int side);
extern int TaLogic_GetWarnHideLvl__00141b30(int side);
/* View of the no-argument 0x16b0e0: 0x168260 passes it one value. */
extern int func_0016b0e0_arg(int);
/* Views of the stack forwarders (all-int parameters in
 * stack_forwarders.h) with the pointers these callers pass. */
extern int func_0011d380_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_00127a30_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_0013ea20_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_001684d0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
/* Fallbacks. 0x168530 is declared with the order pointer these callers pass; 0x127a90 is
 * defined in position_rules with that group's order view; 0x11d3e0 and 0x13ea80 are below. */
extern void func_00127a90_order(struct DotrTryOrder *order);
extern void TaLogic021_CommandInvadeUnit(struct DotrTryOrder *order);

/* Four copies differing in a count N (4, 5, 2, 4), the side callee, the
 * forwarder and the fallback. Nothing when 0x1df540 of the order's position is
 * 0. Otherwise: side = Ta_GetCurrentSide(position), other = (side == 0) << 8, the
 * 0x1e1090 halfwords of the position and of other, 0x1df540 again, a local
 * list of 22 entries set to 0xffff/0xff and 0x27e770 on a 112-byte local. With
 * flag non-zero and TaLogic_IsRootOpen(row, column, low and high byte of the position's
 * halfword, other's row and column, low and high byte of other's halfword, N)
 * > 0: stores 0x2ff at +4 and other above the +8 halfword. Else for n = 1..N
 * the forwarder is called with (order, 0, second 0x1df540 answer, low and high
 * byte of the position's halfword, Ta_GetCurrentSide(position) == 0, n, side callee of
 * side, list, 0, two zeroed local halfwords, N, the local); answer 0 stores
 * 0x5ff at +4, 1 nothing, 2 stores 0x3ff and 3 stores 0x6ff with the position's
 * halfword above the +8 halfword, each ending the function; after N other
 * answers the fallback is called with the order. No result is assumed. */
void TaLogic_CommandBtlUnit__0011d110(struct DotrTryOrder *order, int flag);
void TaLogic006_CommandBtlUnit(struct DotrTryOrder *order, int flag);
void TaLogic011_CommandBtlUnit(struct DotrTryOrder *order, int flag);
void TaLogic021_CommandBtlUnit(struct DotrTryOrder *order, int flag);

/* Callees of 0x11d3e0. 0x1de540's result is not used. 0x11d4e0 stores a word at +0 and a
 * byte at +4 of its fourth argument, hence the eight-byte record (only the word is read here). */
struct DotrTryPick {
    int word;
    unsigned char kind;
};
extern int SzDuel_GetTurnOwner(void);
extern void TaLogic004_GetNearAtkLineEdge(int unit, int column, int row, struct DotrTryPick *out);
/* The fallback of 0x11d110. Calls 0x1de540 and takes the 0x1e1090 halfword of
 * the position. With TaLogic004_IsUnitOnLine(row, column) negative:
 * TaLogic004_GetNearAtkLineEdge(0x1df540 answer, low byte, high byte of that halfword, &pick), then
 * a non-negative pick word stores 0x6ff at +4 and that word above the +8
 * halfword, a negative one 0x5ff at +4. Otherwise, unless the high byte of the
 * +4 halfword is already 15 or 16, stores 0xfff (high byte of the 0x1e1090
 * halfword below 4) or 0x10ff at +4 and 0xffff above the +8 halfword. */
void TaLogic004_CommandAtkLineUnit(struct DotrTryOrder *order);

/* The fallback of 0x13e7b0. Takes the 0x1e1090 halfword of the position, calls
 * Ta_IsUnitControl(row, column) without using its answer, then
 * TaLogic011_GetNearMountain(0x1df540 answer, low byte, high byte of the halfword): a
 * non-negative answer stores 0x6ff at +4 and the answer above the +8 halfword,
 * a negative one 0x5ff at +4. TaLogic011_GetNearMountain (0x13eb20, cell_picks) is declared
 * with three int arguments and an int result, an assumed prototype inferred from this caller. */
extern int func_00250360_pair(int a, int b);
extern int TaLogic011_GetNearMountain(int word, int column, int row);
void TaLogic011_CommandDefenseUnit(struct DotrTryOrder *order);

/* By the low byte of the SzDuel_GetUnitLoc(a, b) halfword and its high byte (row):
 * 0 with row 0..6 gives 3, 2 with row 2..4 gives 0, 4 with row 2..4 gives 1,
 * 6 with row 0..6 gives 4; anything else -1. Two forwarded int arguments. */
int TaLogic004_IsUnitOnLine(int a, int b);

/* By line index (0, 1, 2, anything else) scans SzDuel_GetMapUnitID(x, y) for x 2, 4, 0, 6 and
 * y 2..4, 2..4, 1..5, 1..5: 0 as soon as a non-negative cell (row in the high
 * byte, column in the low byte) has Ta_IsUnitControl(row, column) equal to side and
 * SzDuel_GetUnitKind(row, column) >> 5 equal to 0; otherwise 1. The line index is masked to a
 * byte on entry, taken as an unsigned char parameter. */
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetUnitKind(int side, int pos);
int TaLogic004_IsEmptyAtkLine(int side, unsigned char lineIndex);

/* A fifth copy without the flag and the fallback. 0x162580 is defined in
 * position_rules; `_try` is a pointer view of the stack forwarder 0x15e8f0, as above. */
extern int TaLogic_GetWarnHideLvl__00162580(int side);
extern int func_0015e8f0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
/* As the four copies with N = 5, except: 0x27e770 comes before the list is
 * filled, 0x27b200 is always asked, 0x24de70 is asked once, and answer 0 stores
 * 0x3ff and the position's halfword like answer 2. After five other answers:
 * 0x6ff at +4 and, above the +8 halfword, other's 0x1e1090 halfword when both
 * rows (high bytes) are below 4 or both are 4 or more, else 0x300 | the low
 * byte of the position's halfword. One pointer argument; no result assumed. */
void TaLogic019_CommandAtkUnit(struct DotrTryOrder *order);

/* Callees of 0x150850: 0x153ab0 and 0x153ed0 are defined in position_rules,
 * 0x1e1030 in state_grid; 0x150be0 is a stack forwarder, viewed with
 * the pointers this caller passes. */
extern int TaLogic015_IsActiveMode(void);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int TaLogic_GetWarnHideLvl__00153ed0(int side);
extern int func_00150be0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
/* As 0x15e030, except: the low byte of TaLogic015_IsActiveMode() is taken first and gives the
 * try count N (5 when non-zero, else 2); SzDuel_GetUnitCardNo(position) == 686 stores 0xcff
 * at +4 and ends before the list is filled; 0x27b200 is asked with 2 and a
 * positive answer stores 0x2ff, or 0xbff when SzDuel_GetUnitKind(position) >> 5 is
 * non-zero; every try asks 0x24de70 again; answer 1 stores 0xbff under the same
 * 0x1e0700 test and otherwise nothing. One pointer argument; no result assumed. */
void TaLogic015_CommandBtlUnit(struct DotrTryOrder *order);

/* Two more of the family. 0x135fb0 and 0x159f80 are stack forwarders,
 * viewed with the pointers these callers pass; 0x15d120 is defined in
 * lower_rules. */
extern int func_00135fb0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_00159f80_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int TaLogic_GetWarnHideLvl__0015d120(int side);
/* As the four copies with N = 2, except: no side callee (the forwarder's
 * eighth argument is 0) and no fallback: after two other answers 0x5ff is
 * stored at +4. */
void TaLogic009_CommandBtlUnit(struct DotrTryOrder *order, int flag);
/* As 0x15e030 with N = 3, except: 0x27e770 comes first, 0x24de70 is asked
 * again on every try, and after three other answers 0x6ff is stored with
 * other's 0x1e1090 halfword above the +8 halfword, whatever the rows. */
void TaLogic018_CommandBtlUnit(struct DotrTryOrder *order);

/* 0x130780 is defined in position_rules; 0x12cb80 is a stack
 * forwarder, viewed with the pointers this caller passes. */
extern int TaLogic_GetWarnHideLvl__00130780(int side);
extern int func_0012cb80_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
/* As the four copies, with a third byte-wide argument and no fallback callee:
 * 0x27e770 comes before the list is filled; 0x27b200 is asked with 5 when the
 * byte is 1, else 2; a zero byte makes two tries passing 2 to the forwarder,
 * any other byte four tries passing 5; after those 0x5ff is stored at +4. */
void TaLogic007_CommandBtlUnit(struct DotrTryOrder *order, int flag, unsigned char deep);

/* Five more of the family. 0x163ec0, 0x1224d0, 0x118b60, 0x143790 and 0x16e4b0
 * are stack forwarders, viewed with the pointers these callers pass; 0x1254a0,
 * 0x11aa10 and 0x1461c0 are defined in position_rules; 0x163f20 is defined
 * there with that group's order view; 0x1676d0 and 0x172310 (also position_rules)
 * are declared with assumed prototypes inferred from these calls (one int argument
 * and an int result); 0x122530 and 0x118bc0 are this group's
 * TaLogic005_CommandRoutineUnit and TaLogic003_CommandAtkLineUnit, called through
 * `_order` views with one order pointer. */
extern int TaLogic_GetWarnHideLvl__001676d0(int side);
extern int TaLogic_GetWarnHideLvl__001254a0(int side);
extern int TaLogic_GetWarnHideLvl__0011aa10(int side);
extern int TaLogic_GetWarnHideLvl__001461c0(int side);
extern int TaLogic_GetWarnHideLvl__00172310(int side);
extern int func_00163ec0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_001224d0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_00118b60_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_00143790_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int func_0016e4b0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern void func_00163f20_order(struct DotrTryOrder *order);
extern void func_00122530_order(struct DotrTryOrder *order);
extern void func_00118bc0_order(struct DotrTryOrder *order);
/* As the four copies with N = 5, except that 0x27e770 comes before the list is
 * filled. */
void TaLogic020_CommandBtlUnit(struct DotrTryOrder *order, int flag);
/* As 0x163c50 with three tries passing 3, except: 0x27b200 is asked with 2 and
 * answer 3 stores 0x2ff at +4 and nothing at +8. */
void TaLogic_CommandBtlUnit__00122270(struct DotrTryOrder *order, int flag);
/* As the four copies with N = 3, except that 0x27e770 is called after the
 * 0x27b200 test (so not at all when that test ends the function). */
void TaLogic_CommandBtlUnit__001188f0(struct DotrTryOrder *order, int flag);
/* No flag: 0x27b200 is always asked, with 2, after 0x27e770 and the list fill.
 * Five tries passing 5; answer 3 stores 0x6ff and the position's halfword as
 * the copies do but goes on to the next try; after the fifth try without
 * answer 0, 1 or 2, 0x5ff is stored at +4. One pointer argument assumed. */
void TaLogic012_CommandBtlUnit(struct DotrTryOrder *order);
/* No flag: 0x27b200 is always asked, with 5. Five tries passing 5 whose sixth
 * argument comes from the first 0x24de70 answer (not asked again); answer 0
 * stores 0x3ff with the position's halfword, as answer 2; after five other
 * answers 0x6ff is stored with other's 0x1e1090 halfword above the +8
 * halfword. One pointer argument assumed. */
void TaLogic022_CommandAtkUnit(struct DotrTryOrder *order);
/* Callees of 0x131b30: 0x1df470 is defined in state_records (viewed here
 * with the side this caller passes), 0x134ae0 in position_rules; 0x131de0 is a
 * stack forwarder viewed with this caller's pointers; 0x131e40's one pointer
 * argument is inferred from this call. */
extern int func_001df470_side(int side);
extern int TaLogic_GetWarnHideLvl__00134ae0(int side);
extern int func_00131de0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern void func_00131e40_order(struct DotrTryOrder *order);
/* As TaLogic_CommandBtlUnit__0011d110 (four tries passing 4, 0x27e770 before the list fill), except:
 * after the fill, SzDuel_GetUnitCardNo(position) == 408 with SzDuel_GetLP(side) <= 1000 stores
 * 0x3ff at +4 and the position's halfword above the +8 halfword and goes on;
 * the fallback is 0x131e40. Two arguments; no result assumed. */
void TaLogic008_CommandBtlUnit(struct DotrTryOrder *order, int flag);
/* Callees of 0x113760: 0x117c30 is defined in lower_rules; 0x113a00 is a
 * stack forwarder viewed with this caller's pointers; 0x113a60's two arguments
 * are inferred from this call. */
extern int TaLogic_GetWarnHideLvl__00117c30(int side);
extern int func_00113a00_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern void func_00113a60_order(struct DotrTryOrder *order, int deep);
/* As 0x11d110, except: 0x27e770 comes after the list fill; 0x27b200 is asked
 * with 2; the try count N is 5 when the third argument is non-zero, else 2,
 * and its low byte is passed to each try; the fallback 0x113a60 also receives
 * the third argument. Three arguments; no result assumed. */
void TaLogic001_CommandBtlUnit(struct DotrTryOrder *order, int tryAimLeader, int deep);
/* Callees of 0x13a400: 0x13a6b0 is a stack forwarder viewed with this
 * caller's pointers; the arguments of 0x13da70 and 0x13a710 are inferred from
 * these calls. */
extern int TaLogic_GetWarnHideLvl__0013da70(int side);
extern int func_0013a6b0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern void func_0013a710_order(struct DotrTryOrder *order, int deep);
/* As 0x113760, except: 0x27e770 comes before the list fill; N is 1 when the
 * third argument is non-zero, else 3; answer 0 stores 0x3ff and the position's
 * halfword, as answer 2; the fallback is 0x13a710. No result assumed. */
void TaLogic010_CommandBtlUnit(struct DotrTryOrder *order, int flag, int deep);
/* Callees of 0x1474f0: 0x14b330 and 0x147e50 are defined in position_rules,
 * 0x147e00 in lower_rules; 0x1477d0 is a stack forwarder viewed with this
 * caller's pointers; 0x147f40 gets no argument set up, and an int result is
 * inferred from the sign test. */
extern int TaLogic_GetWarnHideLvl__0014b330(int side);
extern int func_001477d0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int TaLogic013_IsDefensePoint(int column, int row);
extern int TaLogic013_GetEmptyLine(void);
extern int TaLogic013_GetInvadePoint(int currentSide, int col, int row);
/* No flag: 0x27b200 is always asked, with 2, after the list fill and 0x27e770.
 * Two tries passing 2, each asking 0x24de70 again; answers 0 and 2 store 0x3ff
 * and answer 3 stores 0x6ff, each with the position's halfword above the +8
 * halfword. After two other answers 0x6ff is stored with: the position's
 * halfword when TaLogic013_IsDefensePoint(its low byte, its high byte) == 1; else TaLogic013_GetEmptyLine()
 * when that is not negative; else TaLogic013_GetInvadePoint(side, low byte, high byte). One
 * pointer argument assumed. */
void TaLogic013_CommandBtlUnit(struct DotrTryOrder *order);
/* Callees of 0x14c3b0: 0x14f9b0, 0x14c970 and 0x14ca60 are defined in
 * position_rules; 0x14c6c0 is a stack forwarder viewed with this caller's
 * pointers; 0x14f3a0 gets no argument set up, and an int result is inferred
 * from the comparison with 1. */
extern int TaLogic_GetWarnHideLvl__0014f9b0(int side);
extern int func_0014c6c0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern int TaLogic014_IsLinePoint(int column, int row);
extern int TaLogic014_IsActiveMode(void);
extern int TaLogic014_GetEmptyLine(void);
/* As 0x1474f0, except: 0x27e770 comes before the list fill; 0x27b200 is asked
 * with 3; three tries passing 3. After three other answers: when
 * TaLogic014_IsLinePoint(low byte, high byte of the position's halfword) == 1, 0x2ff with
 * other's row word if TaLogic014_IsActiveMode() == 1, else 0x6ff with the position's
 * halfword; otherwise 0x6ff with TaLogic014_GetEmptyLine() when that is not negative, else
 * 0x2ff with other's row word. One pointer argument assumed. */
void TaLogic014_CommandBtlUnit(struct DotrTryOrder *order);

/* 0x10eda0 is defined in position_rules; 0x10d820 is a stack forwarder
 * viewed with this caller's pointers; 0x10db00's pointer and two int arguments
 * are inferred from the registers set for it. */
extern int TaLogic_GetWarnHideLvl__0010eda0(int side);
extern int func_0010d820_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf);
extern void func_0010db00_order(struct DotrTryOrder *order, int one, int hit);
/* As 0x12c7f0, except: 0x27b200 is always asked with 2; a non-zero byte makes
 * five tries passing 6; answer 0 calls TaLogic_CommandDefenseUnit(order, 1, 1) and running out
 * of tries calls TaLogic_CommandDefenseUnit(order, 1, 0) instead of storing 0x5ff (the second
 * argument is a local set to 1 after every try). */
void TaLogic_CommandBtlUnit__0010d490(struct DotrTryOrder *order, int flag, unsigned char isActive);

/* 0x19a5c8 is library code; no argument and an int result are assumed, as in
 * the other groups that call it. */
extern int func_0019a5c8(void);
/* A byte-wide second argument picks a row: 3 when it is 1, else 4. 0x1e1090 is
 * asked once more at the start (result unused); 0x27e770 comes before the list
 * fill; 0x27b200 is always asked, with 2; two tries passing 2 through 0x16e4b0
 * with TaLogic_GetWarnHideLvl__00172310(side); answers 0 and 2 store 0x3ff and answer 3 stores 0x6ff,
 * each with the position's halfword above the +8 halfword. After two other
 * answers: when the position's row is not the picked row, 0x6ff with
 * (picked row << 8 | column). Otherwise by the high byte already at +4:
 * 16: column >= 2 stores (row << 8) only, else 0xfff with (row << 8 | 5);
 * 15: column < 5 stores (row << 8 | 5) only, else 0x10ff with (row << 8 | 1);
 * other: 0x19a5c8() % 2 == 0 gives 0x10ff with (row << 8 | 1), else 0xfff with
 * (row << 8 | 5). */
void TaLogic022_CommandLineUnit(struct DotrTryOrder *order, unsigned char deep);
/* As 0x16e0e0, with 0x162580 and the forwarder 0x15e8f0. */
void TaLogic019_CommandLineUnit(struct DotrTryOrder *order, unsigned char deep);

/* 0x24df00 is defined in state_queries and 0x113b60 in position_rules. */
extern int CheckLandAgreement2(int field, int side, int pos);
extern int TaLogic001_GetNextGrassPoint(int col, int row);
/* The fallback 0x113760 calls (its second argument is not read here). Asks
 * 0x250360 for the position (result unused) and 0x1e1090 for its halfword,
 * always stores 0x6ff at +4, and puts above the +8 halfword: when
 * CheckLandAgreement2(3, position bytes) == 1, TaLogic001_GetNextGrassPoint(column, row); else 0x0303 when
 * column and row are both below 3; else 0x0305 when column >= row; else
 * 0x0503. One pointer argument assumed. */
void TaLogic001_CommandInvadeUnit(struct DotrTryOrder *order);

/* 0x118dd0 and 0x118e90 are defined in position_rules. */
extern int TaLogic003_GetEmptyAtkLineStart(int a);
extern int TaLogic003_GetEmptyAtkLineEnd(int a);
/* The fallback 0x1188f0 calls. With s = SzDuel_GetTurnOwner(), the position's halfword h
 * from 0x1e1090, p = TaLogic003_GetEmptyAtkLineStart(s) and q = TaLogic003_GetEmptyAtkLineEnd(s):
 * row 0 stores 0xfff at +4 when h == p, else 0x6ff with p above the +8 halfword; row 6 stores
 * 0x10ff when h == q, else 0x6ff with q; any other row, unless the high byte
 * at +4 is already 15 or 16, stores 0xfff (row < 4) or 0x10ff with 0xffff
 * above the +8 halfword. One pointer argument assumed. */
void TaLogic003_CommandAtkLineUnit(struct DotrTryOrder *order);

/* Callees of the fallback 0x13a710: TaLogic010_GetLineHeight (0x13a9a0, defined in
 * position_rules) and TaLogic010_GetEmptyLine (0x13a870, declared below), both viewed
 * with the side this caller passes; int results are
 * assumed from the compare and sign test. */
extern int func_0013a9a0_side(int side);
extern int func_0013a870_side(int side);
/* The fallback of 0x13a400. A second argument whose low byte is 1 probes the
 * cell one row below the position's 0x1e1090 halfword with 0x1e3790: a
 * non-negative answer is stored above the +8 halfword with 0x2ff at +4, else
 * that cell's own bytes with 0x6ff. Otherwise 0x6ff is stored with the
 * halfword itself (its row equals TaLogic010_GetLineHeight(side)), TaLogic010_GetEmptyLine(side) when
 * non-negative, or the cell one row below. No result assumed. */
void TaLogic010_CommandInvadeUnit(struct DotrTryOrder *order, int deep);
/* Callees of the fallback 0x122530: 0x1e0290 is defined in state_grid,
 * 0x122680 in small_misc and 0x122810 in position_rules (viewed here with the
 * arguments this caller passes); TaLogic_GetCyclePoint (0x122970, kind_gates; int argument
 * and result) and TaLogic005_GetNearDefensePoint (0x123500, position_rules; a word, column,
 * row and an out pointer whose first word is read back) have assumed prototypes inferred
 * from these calls. */
extern int func_001e0290_try(int a, int b, int flag);
extern int func_00122680_try(int a, int b);
extern int func_00122970_try(int n);
extern int func_00122810_try(int column, int row);
extern void TaLogic005_GetNearDefensePoint(int word, int column, int row, int *out);
/* The fallback of 0x122270. Nothing for a zero cell word. Stores 0x6ff at +4;
 * with SzDuel_GetUnitAP(position, 0) >= 1500 the word above the +8 halfword is
 * TaLogic_GetCyclePoint((n + 1) % 8) for a non-negative n = TaLogic005_GetUnitOnCycleRoot(position), else
 * TaLogic005_GetNearCyclePoint(column, row); below 1500 it is 0x123500's output word when
 * non-negative, else 0x5ff replaces the +4 halfword. One pointer assumed. */
void TaLogic005_CommandRoutineUnit(struct DotrTryOrder *order);

/* 0x1e36e0 is defined in state_grid; only its null test is used here. */
extern void *func_001e36e0_try(int x, int y);
/* The first empty cell (0x1e36e0 answers null) among columns 3, 4, 2, 5, 1 of
 * the row TaLogic010_GetLineHeight(side) answers (low byte), as (row << 8) | column, else -1.
 * position_rules.h declares this address as a callee of 0x13d280. */
int TaLogic010_GetEmptyLine(int side);

/* The order record as TaLogic013_CommandInvadeUnit views it: the packed position, the command
 * halfword at +4 and the result word at +8 as an unsigned word or two halfwords. Same offsets as
 * DotrTryOrder. MATCHING DEVICE: a separate type (unsigned, word first) that the function
 * reaches through a cast of its parameter. */
struct DotrTryInvadeOrder {
    unsigned short coordinates;
    unsigned char pad2[2];
    unsigned short state;
    unsigned char pad6[2];
    union {
        unsigned int word;
        struct {
            unsigned short low;
            unsigned short high;
        } halves;
    } data;
};
/* TaLogic013_CommandInvadeUnit (0x147830): the flagless shape with up to five tries made
 * directly through TaLogic_GetNearUnit (0x27de20, defined in driver_task under its own types);
 * this 16-argument view is what this caller passes and is an assumption. */
extern int TaLogic_GetNearUnit(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, int s5, int s6, void *buf);
void TaLogic013_CommandInvadeUnit(struct DotrTryOrder *order);

#endif
