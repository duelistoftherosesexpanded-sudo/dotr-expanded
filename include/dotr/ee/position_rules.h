#ifndef DOTR_EE_POSITION_RULES_H
#define DOTR_EE_POSITION_RULES_H

/* AI rule functions between 0x10d880 and 0x172310 that return packed (row << 8 | column)
 * position constants, test a position or state_grid query result against fixed limits, or
 * write such positions into an order record.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Halfword pairs are read as (row << 8 | column); "row" and "column" are
 * assumed names for the high and low byte. */

/* Record written by 0x127a90: only these three fields are established. */
#include "dotr/ee/position_order.h"

/* Callees, declared with the argument counts these callers pass; int results
 * and parameters are assumed. Other group headers hold other views. */
extern int TaHand_GetCardKind(void);
extern int Ta_GetHandEnchant(int, int);
extern int TaLogic_GetCyclePoint(int);
extern int SzDuel_GetUnitLoc(int, int);
extern int SzDuel_GetLP(void);
extern int SzDuel_GetUnitMonsterCount(int);
extern int SzDuel_GetTurnOwner(void);
extern int TaLogic_GetSommonPiecePreference__0014da30(int, int);
extern int SzDuel_GetUnitCount(int);
extern int TaLogic_GetSommonPiecePreference__001604a0(int, int);
/* Callees of the 0x118dd0 cluster. 0x118ce0's second argument is narrowed to a
 * byte by its callers; 0x1e36e0's result is only tested against zero here;
 * 0x250360 is called with two arguments (its definition takes none). */
struct DotrStateItem;
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
extern int TaLogic017_GetLine(int, int);
extern int func_00250360_pair(int a, int b);
/* Callees of the 0x118ce0 cluster; 0x13a870 takes one int (assumed). */
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetField(int x, int y);
extern int TaLogic010_GetEmptyLine(int);
extern int func_001df540_word(int a, int b);

int TaLogic002_IsRefuseEmptyCombo(int id);
int TaLogic005_GetNearCyclePoint(int column, int row);
int TaLogic005_GetDefensePoint(int index);
void TaLogic006_CommandInvadeUnit(struct DotrPositionOrder *order);
int TaLogic011_IsUseBraveHourGlass(int side);
int TaLogic013_IsDefensePoint(int column, int row);
int TaLogic013_GetDefensePoint(int index);
/* The first of 0x147eb0's five positions whose 0x1e36e0 cell is empty, else
 * -1. No argument is set up by its caller 0x1474f0. */
int TaLogic013_GetEmptyLine(void);
int TaLogic014_IsLinePoint(int column, int row);
int TaLogic014_GetLine(int index);
/* 0x24e410: (side, index) and an int result assumed. */
extern int Ta_GetSommonPiecePreference(int a, int direction);
/* At the first of 0x14c9d0's three positions whose 0x1e36e0 cell is empty,
 * asks Ta_GetSommonPiecePreference(SzDuel_GetTurnOwner(), 0 / 3 / 4 by index); returns 0 when that answer is
 * non-negative, 1 when it is negative or no cell is empty. */
int TaLogic014_IsActiveMode(void);
int TaLogic015_IsActiveMode(void);
int TaLogic019_IsLinePoint(int column, int row);
int TaLogic019_GetLine(int index);
int TaLogic020_GetLinePiece(int index);

/* Fourteen copies of one shape: a fixed value when SzDuel_GetLP() is at or below a
 * per-copy limit, otherwise one of two values by whether SzDuel_GetUnitMonsterCount(side) reaches
 * a per-copy count. The last four return 0 on every path. "side" is assumed. */
int TaLogic_GetWarnHideLvl__0011fa80(int side);
int TaLogic_GetWarnHideLvl__001254a0(int side);
int TaLogic_GetWarnHideLvl__0012b1b0(int side);
int TaLogic_GetWarnHideLvl__00130780(int side);
int TaLogic_GetWarnHideLvl__00134ae0(int side);
int TaLogic_GetWarnHideLvl__0014b330(int side);
int TaLogic_GetWarnHideLvl__0014f9b0(int side);
int TaLogic_GetWarnHideLvl__00153ed0(int side);
int TaLogic_GetWarnHideLvl__00158780(int side);
int TaLogic_GetWarnHideLvl__00162580(int side);
int TaLogic_GetWarnHideLvl__0010eda0(int side);
int TaLogic_GetWarnHideLvl__0011aa10(int side);
int TaLogic_GetWarnHideLvl__00141b30(int side);
int TaLogic_GetWarnHideLvl__001461c0(int side);
/* Two more copies of that shape (0x13da70 returns 0 on every path) and the
 * empty one in the same slot of its caller: 0x1676d0 only returns 0. */
int TaLogic_GetWarnHideLvl__0013da70(int side);
int TaLogic_GetWarnHideLvl__00172310(int side);
int TaLogic_GetWarnHideLvl__001676d0(int side);

/* 0x118dd0: the position for the first i in 0..3 whose TaLogic003_IsEmptyAtkLine(a, i) is
 * non-zero (0x102, 0x104, 0x100, 0x106), else -1. 0x118e90: the same over the
 * first zero answer (0x602, 0x604, 0x600, 0x606). */
int TaLogic003_GetEmptyAtkLineStart(int a);
int TaLogic003_GetEmptyAtkLineEnd(int a);
/* Position by the second argument (0..3, anything else as 3); the first
 * argument is not read. */
int TaLogic004_GetAtkLineStart(int a, int lineIndex);
int TaLogic004_GetAtkLineEnd(int a, int k);
/* 1 when (b << 8 | a) equals TaLogic017_GetLine(i, magicType) for some i in 0..6, else 0. */
int TaLogic017_IsLinePoint(int a, int b, int magicType);
/* 0x302 when SzDuel_GetMapUnit(2, 3) is zero, else 0x304 when SzDuel_GetMapUnit(4, 3) is zero, else -1. */
int TaLogic017_GetEmptyTrapPiece(void);
/* The TaLogic019_GetLine(i) position of the first i in 0..3 whose 0x1e36e0 cell is zero, else -1. */
int TaLogic019_GetEmptyLine(void);
/* As 0x15ee40 over three slots (0, 3, 4 of 0x14da30): the position of the first
 * slot whose 0x1e36e0 cell is empty, asked again from 0x14da30; -1 when none. */
int TaLogic014_GetEmptyLine(void);
/* As 0x127a90, with 0x250360 called first and the high halfword taken from 0x163f90. */
void TaLogic020_CommandInvadeUnit(struct DotrPositionOrder *order);

/* 0x602 when row >= 5, else 0x206 when col >= 5, else 0x105 when col >= row, else 0x501. */
int TaLogic001_GetNextGrassPoint(int col, int row);
/* Column 2, 4, 0, 6 for lineIndex 0, 1, 2, other; 0 when a row 1..5 cell of that
 * column (0x1e3790, non-negative) has Ta_IsUnitControl(hi, lo) == side and
 * SzDuel_GetUnitKind(hi, lo) >> 5 == 0, else 1. lineIndex is read as a byte. */
int TaLogic003_IsEmptyAtkLine(int side, unsigned char lineIndex);
/* TaLogic009_GetEmptyDefensePoint() >= 0: kind 0x3ff and that value in the high halfword at +8;
 * else kind 0x5ff. */
void TaLogic009_CommandDefenseUnit(struct DotrPositionOrder *order);
/* 0x202 or 0x205 for the first of the cells (2, 2), (5, 2) whose 0x1e36e0 is
 * zero and whose 0x1e22b0 is not 8, else -1. */
int TaLogic009_GetEmptyDefensePoint(void);
/* The largest (SzDuel_GetUnitLoc(side, i) >> 8) & 0xff over i in 1..5 with a non-zero
 * SzDuel_GetUnit(side, i) word, and at least 2. */
int TaLogic010_GetLineHeight(int side);
/* 1 when TaLogic010_GetEmptyLine(SzDuel_GetTurnOwner()) is negative, else 0. */
int TaLogic010_IsActiveMode(void);
/* TaLogic017_GetLine(i, magicType) of the first i in 0..6 whose 0x1e36e0 cell is zero (asked again), else -1. */
int TaLogic017_GetEmptyLine(int magicType);
/* Position by row band (0, 1..5, above) and by a >= 3. */
int TaLogic020_GetNextGrassPoint(int a, int b);

/* 1 when the 0x1e36e0 cell at TaLogic005_GetDefensePoint(index)'s position is empty, else 0. */
int TaLogic005_IsEmptyDefensePoint(int index);
/* Twelve local bytes whose two nibbles are both set to 15 before use, and the
 * record 0x123500 writes: a position word and a one-byte cost. Offsets only;
 * 0x122530 reads the word back and reserves eight bytes for the record. */
struct DotrSpotNibbles {
    unsigned char low : 4;
    unsigned char high : 4;
};
struct DotrSpotBest {
    int spot;
    unsigned char cost;
};
/* TaLogic_GetRoot2 (0x2792c0, driver_scans; small_misc.h declares it with int arguments):
 * this caller passes the nibble bytes second and a halfword seventh. */
extern int func_002792c0_spot(int word, struct DotrSpotNibbles *bytes, int column, int row, int x, int y, unsigned short id, int n);
/* Writes (-1, 0xff) to the record, then for each of the three 0x123670
 * positions that 0x1236c0 reports empty asks 0x2792c0 for a cost (passing the
 * 0x1e3790 id of that cell, 0xffff when negative, and 7); the first cost that
 * is non-negative and below the record's is stored with the position and ends
 * the walk. No result assumed. */
void TaLogic005_GetNearDefensePoint(int word, int column, int row, struct DotrSpotBest *best);

/* The 0x154eb0 cluster. Callees declared with the argument counts these
 * callers pass; int results and parameters are assumed. 0x24de70, 0x1df470 and
 * 0x24ca40 have other views in other group headers. */
extern int func_0024de70_pair(int a, int b);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int func_001df470_side(int side);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
/* 1 when a 0x1e3790 cell at one of the seven TaLogic017_GetLine(i, magicType) positions
 * equals (a << 8 | b), magicType being SzDuel_GetUnitKind(a, b) >> 5, or failing that at one
 * of the positions of the other card type (1 for type 0, else 0); 0 at the first
 * negative cell or when none matches. */
int TaLogic017_IsLineTop(int a, int b);
/* Writes kind 0x7ff and a position into the high halfword at +8: the order's
 * own cell when 0x155670 answers 1, (column | 0x600) when neither 0x1554b0
 * scan has the cell and its row is below 6, else the first free 0x1555f0
 * position; kind 0x6ff and 0x503 when there is none. Nothing is written when
 * 0x1df540 gives a zero word. */
void TaLogic017_CommandBtlUnit(struct DotrPositionOrder *order);
/* By 0x1e1030 kind: 54..57 write kind 0x6ff and the 0x1e1090 cell; 64 and 95
 * run 0x154eb0 and then set kind 0x3ff when a count or total test passes;
 * everything else is 0x154eb0 alone. */
void TaLogic017_CommandEfcMonster(struct DotrPositionOrder *order);
/* By 0x1e0700 >> 5 (a byte): 1..3 as the body of 0x154eb0 past its gate; 4
 * writes 0x6ff and a free position for kinds 820 and 823, else 0x3ff and the
 * own cell; any other value writes kind 0x5ff only. */
void TaLogic017_CommandSpellUnit(struct DotrPositionOrder *order);

/* Order writers that choose between try functions of other groups. The
 * `_order` names are views of functions defined in order_tries with its
 * own record type; 0x16e000 and 0x15e440 are defined in pick_copies;
 * 0x15e2e0, 0x27d720 and 0x27c770 are declared with int results and the
 * argument counts passed here (assumed); 0x250740's result is only read as
 * one word. */
extern int func_001e0290_try(int a, int b, int flag);
extern int TaLogic022_IsEmptyCntrGuard(int side, int pos, unsigned char c);
extern int TaLogic019_IsEmptyCntrGuard(int side, int pos, unsigned char c);
extern void func_0016dda0_order(struct DotrPositionOrder *order);
extern void func_0016e0e0_order(struct DotrPositionOrder *order, unsigned char deep);
extern void func_0015e030_order(struct DotrPositionOrder *order);
extern void func_0015e520_order(struct DotrPositionOrder *order, unsigned char deep);
extern void func_00159d00_order(struct DotrPositionOrder *order);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int TaLogic_CommandGunnerUnit(struct DotrPositionOrder *order, int a);
extern unsigned *func_00250740_word(int a, int b, int n);
extern int TaLogic_GetLandChange(int, int, int, int, int, int);
/* Past the 0x1df540 gate: 0x16dda0 when SzDuel_GetUnitAP(hi, lo, 0) >= 2500, else
 * TaLogic022_CommandLineUnit(order, 1) when TaLogic022_IsEmptyCntrGuard(hi, lo, 1) is 1, else 0x16dda0. */
void TaLogic022_CommandBtlUnit(struct DotrPositionOrder *order);
/* As 0x16dca0 with 0x15e030, 0x15e440 and 0x15e520; when the first test fails
 * a non-negative TaLogic019_IsEmptyTrapGuard(hi, lo) is written with kind 0x6ff, else the test
 * is repeated with 2. */
void TaLogic019_CommandBtlUnit(struct DotrPositionOrder *order);

/* Ta_IsUnitControl(row, column) is called and dropped; the cell SzDuel_GetUnitLoc(row, column)
 * is written as kind 0x3ff with the cell in the upper flag half (the same in
 * both arms of the TaLogic019_IsLinePoint(cell & 0xff, cell >> 8) == 1 test); then a
 * non-negative TaLogic019_GetEmptyLine() replaces the cell, else TaLogic019_CommandBtlUnit(order) runs.
 * Assumed: int coordinates, no result, the order type. */
void TaLogic019_CommandLDGuardUnit(int row, int column, struct DotrPositionOrder *order);

/* Kind 0x5ff when Ta_GetUnitEffect(position, 0) is null. Otherwise by the byte
 * SzDuel_GetUnitKind(position) >> 5. For 1, by the word's upper bits: 20 gives 0xcff; 4
 * asks TaLogic019_GetUseThunderBolt(side) and writes 0x6ff with the column of that cell's 0x1e1090
 * under the row of the order's own cell (negative: 0x5ff); 37 writes 0x6ff with
 * 0x0406 when SzDuel_GetUnitCardNo(position) is 749, else 0xbff when TaLogic_CommandGunnerUnit(order, 0) is
 * 1; 31 gives 0xdff for 732. For 3 and 4: 0x6ff with 0x0600 for 749; else the
 * body of 0x15e440 for column 3 (expanded in place) sends TaLogic019_CommandLineUnit(order, 1);
 * else a non-negative TaLogic019_IsEmptyTrapGuard(position) is written with 0x6ff; else the same
 * for column 4 sends TaLogic019_CommandLineUnit(order, 2); else 0xbff when
 * TaLogic_CommandGunnerUnit(order, 0) is 1. Everything else is 0x5ff.
 * Assumed: no result; TaLogic019_GetUseThunderBolt (defined in lower_rules) with one int. */
extern int TaLogic019_GetUseThunderBolt(int side);
void TaLogic019_CommandSpellUnit(struct DotrPositionOrder *order);
/* By 0x1e1030 kind 659, 647, 655: returns after TaLogic_CommandGunnerUnit(order, 0) == 1 (659
 * only with a zero SzDuel_GetUnitStatus(hi, lo, 4); 647 also sets kind 0xbff), or writes
 * kind 0x3ff and the own cell when the 0x24ca40 list has more than five
 * entries; every other path ends in 0x159d00. */
void TaLogic018_CommandEfcMonster(struct DotrPositionOrder *order);
/* Kind 0x5ff on every path; before it, when the 0x250740 word's bits from 8
 * up are 2, 0x14f3a0 is 1 and 0x27c770 is non-negative, kind 0x6ff and that
 * position are stored (the kind is then overwritten). */
void TaLogic014_CommandSpellUnit(struct DotrPositionOrder *order);
/* By 0x1e1030 kind: 695 writes kind 0x6ff and a non-negative 0x27c770
 * position whose row is below 4; 687 and 732 write kinds 0xcff and 0xdff
 * with 0xffff; anything else kind 0x5ff. */
void TaLogic009_CommandSpellUnit(struct DotrPositionOrder *order);

/* More order writers. Callees declared with the argument counts these callers
 * pass; int results and parameters are assumed. 0x131f80, 0x142040,
 * 0x14fa10, 0x27de20, 0x27bbc0, 0x15a5f0, 0x159930 and 0x159be0 are
 * declared as these callers use them; the `_word`, `_side`, `_order`, `_buf` and `_block`
 * names are views of functions defined in other groups with other types. The cell
 * list and the 96-byte block are only known as far as these callers use them. */
struct DotrPositionCell {
    unsigned short id;   /* +0 */
    unsigned char kind;  /* +2 */
    unsigned char unknown_03;
};
struct DotrPositionBlock {
    unsigned char unknown_00[0x3c];
    unsigned short unknown_3c; /* +0x3c, an identifier: read by 0x156b40 */
    unsigned char unknown_3e;  /* +0x3e, read >> 5 by 0x159760 */
    unsigned char unknown_3f[0x21];
};
extern unsigned *func_001e0b70_word(int a, int b, int n);
extern int func_001349a0_side(int side);
extern int func_0016cbb0_word(int side, unsigned *record);
extern void func_0027e770_buf(void *buf);
extern int TaLogic011_IsUseLifeHourGlass(int side);
extern void func_0013e7b0_order(struct DotrPositionOrder *order, int flag);
extern void func_0014c3b0_order(struct DotrPositionOrder *order);
extern int func_0027de20_order(struct DotrPositionOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7,
                               struct DotrPositionCell *cells, int s1, unsigned short *a, unsigned short *b, int n,
                               int s5, int s6, void *buf);
extern void func_0011d110_order(struct DotrPositionOrder *order, int flag);
extern int TaLogic_IsAttackedNow(int a, int b, int c);
extern int SzDuel_InqHandPhase(int side);
extern int func_0015a5f0_block(int side, struct DotrPositionBlock *block);
extern int func_0015ab10_block(int side, unsigned char *item, struct DotrPositionBlock *block);
extern int func_001e02f0_block(struct DotrPositionBlock *block, int unknown);
extern int TaLogic018_GetVirusMove(int a, int b);
/* By the byte 0x1e0700 >> 5: 3 writes kind 0x3ff and a non-negative
 * TaLogic008_GetNextInvadePoint(word, column, row) position, kind 0x5ff when it is negative; 1
 * returns on a null 0x1e0b70 word, else by the word's bits from 8 up: 2 and 72
 * write 0x6ff and the 0x1349a0 position (6 when negative), 37 as level 3;
 * everything else kind 0x5ff. */
void TaLogic008_CommandSpellUnit(struct DotrPositionOrder *order);
/* By the 0x250740 word's bits from 8 up: 20 writes kind 0xcff; 4 writes 0x6ff
 * and a non-negative 0x16cbb0 answer (as the column under the own row when
 * the word's low six bits are zero); 2 writes 0x6ff and a non-negative
 * 0x27c770 position; 37 with low bits 33 runs the body of 0x16dca0; 31 with
 * 0x1e1030 kind 732 writes 0xdff; everything else kind 0x5ff. */
void TaLogic022_CommandSpellUnit(struct DotrPositionOrder *order);
/* Past the 0x1df540 gate, by 0x1e1030 kind: 386 (0x141f30 is 1) and 388 (own
 * 0x1df470 total below the other side's) write kind 0x6ff and the own cell;
 * 377 (0x142040 is 1) and 390 (0x142220 is 1) write 0xbff when
 * TaLogic_CommandGunnerUnit(order, 0) is 1; every other path ends in TaLogic011_CommandBtlUnit(order, 1). */
void TaLogic011_CommandEfcMonster(struct DotrPositionOrder *order);
/* By 0x1e1030 kind: 612 and 551 write kind 0x3ff and the own cell when
 * TaLogic014_IsUnitNaturalEffectUse(side, cardNo) is 1; 570 returns when 0x14f3a0 and
 * TaLogic_CommandGunnerUnit(order, 0) are 1; 575 as 570 with a zero SzDuel_GetUnitStatus(hi, lo, 4)
 * between; every other path ends in 0x14c3b0. */
void TaLogic014_CommandEfcMonster(struct DotrPositionOrder *order);
/* Past the 0x1df540 gate, by 0x1e1030 kind: 496 and 510 ask 0x27de20 with a
 * third-from-stack byte of 1 and then 2 (answers 0, 2, 3 write kind 0x3ff and
 * the own cell, 1 returns), then write the same when the 0x24ca40 list has
 * more than three entries; 509 writes 0xbff when TaLogic_CommandGunnerUnit(order, 0) is 1;
 * every other path ends in TaLogic_CommandBtlUnit__0011d110(order, 0). */
void TaLogic_CommandEfcMonster__0011d9b0(struct DotrPositionOrder *order);
/* 0x127b00: the 0x11d9b0 shape for kinds 108 and 111 with the 0x12b1b0 key, limit 2000 and no
 * list count after the two tries; func_001277c0_order is TaLogic006_CommandBtlUnit (order_tries)
 * seen with this group's order type (assumed the same record). */
extern void func_001277c0_order(struct DotrPositionOrder *order, int flag);
void TaLogic006_CommandEfcMonster(struct DotrPositionOrder *order);
/* 0x12cbe0 and 0x10d880: the same shape with a `deep` argument passed on. The `_pos` externs
 * are 0x12c7f0, TaLogic_CommandBtlUnit__0010d490 (order_tries) and TaLogic_CommandDefenseUnit
 * (kind_gates) seen with this group's order type and plain int arguments (assumed); 0x10d880's
 * second argument is never read. */
extern void func_0012c7f0_pos(struct DotrPositionOrder *order, int flag, int deep);
extern void func_0010d490_pos(struct DotrPositionOrder *order, int flag, int deep);
extern void func_0010db00_pos(struct DotrPositionOrder *order, int one, int hit);
void TaLogic007_CommandEfcMonster(struct DotrPositionOrder *order, int deep);
void TaLogic_CommandEfcMonster__0010d880(struct DotrPositionOrder *order, int unused, int deep);
/* Kind 0x50a / 0x5ff (SzDuel_InqHandPhase(row) is 1 or not) when TaLogic_IsAttackedNow(hi, lo, 0) is
 * 1. Otherwise kind 0x60a / 0x6ff and the TaLogic018_GetVirusMove(hi, lo) position; before
 * that, a non-negative 0x15a5f0 answer (0 also runs 0x15ab10) with the block
 * byte +0x3e below 0x20 and SzDuel_GetUnitAP2(block, 0) above 1500 writes the
 * TaLogic018_GetSummonPoint(row, column, row) position, which is then overwritten. */
void TaLogic018_CommandLDUnit(struct DotrPositionOrder *order);
/* Walks the SearchUnitByRange(0, 0, side, 3, 3, 14) list: 0 at the first entry whose
 * Ta_IsUnitControl(hi, lo) equals side; otherwise 1 when TaLogic_IsJudgeUnitDanger(hi, lo, 0) answered
 * exactly 1 for at least one entry (a byte-wide count), else 0. */
int TaLogic011_IsUseBindChain(int side);
/* View of halfword_lists' 0x15a120 that keeps the 0x15a320 result its tail
 * leaves in v0 (halfword_lists declares it void). */
extern int func_0015a120_count(int a, int b, unsigned short *out);
/* Takes the SzDuel_GetUnitLoc(side == 0, 0) cell and the TaLogic018_GetOutVirusZone(lo, hi, list) list of
 * a byte-wide count: -1 when empty; otherwise the last entry with a zero
 * SzDuel_GetUnit(lo, hi) word whose |lo - x| + |hi - y| is below the best so far
 * (starting at 14), else the cell itself. x / y are assumed names. */
int TaLogic018_GetSummonPoint(int side, int x, int y);
/* Two stages, each skipped when SearchUnitByRange(0, 0, Ta_IsUnitControl(a, b), 1, key, 14) is
 * zero: key 0x33b001c with the SzDuel_GetMapUnitID(0, 5) cell answers 0x500, key
 * 0x2ed001c with the SzDuel_GetMapUnitID(6, 3) cell answers 0x306, unless the cell is
 * non-negative, differs from (a << 8) | b and has the 0x250360 answer of
 * (a, b). -1 when neither stage answers. The key words are not interpreted. */
int TaLogic019_IsEmptyTrapGuard(int a, int b);
/* Card 612: 1 when the SearchUnitByRange(0, 0, side, 1, 16, 14) list has more than
 * three 0x24e910 entries. Card 551: 0 when the (.., 1, 15, 14) list has any
 * entry, else 1 when TaLogic_IsJudgeUnitDanger(hi, lo, 0) answers exactly 1 for at least two
 * entries of the (.., 2, 15, 14) list (a byte-wide count). Otherwise 0. */
int TaLogic014_IsUnitNaturalEffectUse(int side, int cardNo);
/* Views used by 0x131f80: 0x250370 with an int word, 0x250a60 as (word, x, y)
 * and 0x27a130 with the nibble list and the two stack arguments it is given
 * (7 and the low halfword of 0x134ae0). Argument meanings are assumed. */
extern int func_00250370_word(int word);
extern int func_00250a60_spot(int word, int x, int y);
extern int func_0027a130_spot(int word, struct DotrSpotNibbles *bytes, int column, int row, int x, int y, int g, int h,
                              unsigned char i, unsigned short key);
/* -1 for a zero word. Otherwise tries up to three positions with
 * TaLogic_GetInvadeRoot2(word, list, column, row, lo, hi, -1, 0, 7,
 * TaLogic_GetWarnHideLvl__00134ae0(Ta_IsUnitControl2(word))) and returns the first it accepts
 * (non-negative answer): (1) column >= 3 and row 0, 1, 4 or 5: column 6 of the next even row plus
 * one step; column < 3 and row 2 or 3: 0x400; (2) column < 3, row 0, 1, 4 or 5 and a 0x250a60
 * answer below 1500: column 0 of that row; column >= 3, row 2 and
 * TaUtil_GetUnitAP2(word, 6, 3) below 1500: 0x406; else the position of (1) again;
 * (3) the SzDuel_GetUnitLoc(other side, 0) cell. Failing all, that cell's high byte
 * with the column argument as its low byte. */
int TaLogic008_GetNextInvadePoint(int word, int column, int row);
/* Order writer for a SzDuel_GetUnitEffect(.., 1) word whose value above the low byte is 8
 * (kind 0x2ff, bit 24 set when 0x250360 is 0); otherwise kind 0x6ff with the
 * 0x131f80 position, or 0x5ff without one. The 12 nibble pairs it fills are
 * not read. Signature from the register use only (its caller is 0x131b30 in
 * order_tries). */
void TaLogic008_CommandInvadeUnit(struct DotrPositionOrder *order);

/* 0x113bb0, a callee of lower_rules' 0x113140: nothing without a 0x1df540
 * word; otherwise by the 0x1e1030 word of the position: 83 stores 0x3ff and
 * the 0x1e1090 cell when TaLogic001_IsUseLordOfDragon(side) is 1, else 0x5ff; 137 runs i = 1..5:
 * TaLogic_GetNearUnitL2(previous answer, word, cell low, cell high, side == 0 ? 1 : 0, i,
 * 0, TaLogic_GetWarnHideLvl__00117c30(side), cells, &c, &a, &b, buf), then
 * TaLogic_GetNearUnitL3(word, cell low, cell high, cells, 5, 52, 0, 2, 0,
 * TaLogic_GetWarnHideLvl__00117c30(side), c), whose first answer of 0 or more is stored with 0x2ff;
 * 191 stores 0xbff when TaLogic_CommandGunnerUnit(order, 0) is 1; 377 and 390 the same after
 * TaLogic001_IsUseLifeHourGlass(side) / TaLogic001_IsUseBraveHourGlass(side) answer 1. Everything
 * else ends in TaLogic001_CommandBtlUnit(order, 1, deep). Assumed: the signature of
 * TaLogic_GetNearUnitL3 (eleven arguments as passed) and 0x1173e0; `_cells` is a view of
 * small_misc's 0x2770e0 with pointer types and an int eighth parameter (no narrowing here), `_spot` a
 * view of order_tries' 0x113760; 0x117600, 0x117550 and 0x117c30 are declared
 * as in lower_rules.h; the int second parameter is only passed on. */
extern int TaLogic001_IsUseLordOfDragon(int side);
extern int TaLogic001_IsUseBraveHourGlass(int side);
extern int TaLogic_GetWarnHideLvl__00117c30(int side);
extern int TaLogic001_IsUseLifeHourGlass(int side);
extern void func_00113760_spot(struct DotrPositionOrder *order, int flag, int deep);
extern int func_002770e0_cells(int a, int b, int c, int d, int e, int f, int g, int h,
                               struct DotrPositionCell *cells, unsigned short *j, unsigned short *k,
                               unsigned short *l, unsigned char *buf);
extern int TaLogic_GetNearUnitL3(int a, int b, int c, struct DotrPositionCell *cells, int e, int f, int g, int h, int i, int j,
                         unsigned short k);
void TaLogic001_CommandEfcMonster(struct DotrPositionOrder *order, int deep);

/* 0x118f50 and TaLogic_CommandTrapUnit__0011dc00 (same instructions; key callee 0x11aa10 / 0x11fa80):
 * nothing without a 0x1df540 word; otherwise store kind 0xbff, fill the 22
 * cells with 0xffff / 0xff and run two passes of i = 1..2. First pass:
 * TaLogic_GetNearUnitL2(previous answer, word, cell low, cell high, 0x24de70 of the
 * position == 0 ? 1 : 0, i, 0, key(side), cells, &c, &a, &b, buf), then
 * TaLogic_GetTrapNearCard(column, row, cell low, cell high, cells, 3, 31, 1 then 0, 2, 0,
 * key(side), c, 1). Second pass: 0x2770e0 with the 0x24de70 answer itself as
 * fifth argument, then TaLogic_GetFollowNearUnit(cell low, cell high, cells, 3, 34, 2, 2, c),
 * (.., 31, 1, 2, c) and (.., 31, 0, 2, c). The first answer of 0 or more is
 * stored in the upper half of the flags word and the kind stays 0xbff; with
 * none the kind becomes 0x5ff.
 * Assumed: the signatures of TaLogic_GetTrapNearCard (thirteen arguments
 * as passed) and TaLogic_GetFollowNearUnit (eight), int results; the order type is the
 * position_rules view (lower_rules declares both with its own). */
extern int TaLogic_GetTrapNearCard(int a, int b, int c, int d, struct DotrPositionCell *cells, int f, int g, int h, int i,
                         int j, int k, unsigned short l, int m);
extern int TaLogic_GetFollowNearUnit(int a, int b, struct DotrPositionCell *cells, int d, int e, int f, int g, unsigned short h);
void TaLogic_CommandTrapUnit__00118f50(struct DotrPositionOrder *order);
void TaLogic_CommandTrapUnit__0011dc00(struct DotrPositionOrder *order);

/* 0x12ce50: the two-pass scan of 0x11dc00 with loops of two, a cell count of 5 and 0x130780
 * as key. A 0x277cd0 answer of 0 or more is stored in the upper half of the flags word only
 * when 0x1e1580 and 0x1e1640 both answer 0 for its two bytes; a 0x2774f0 answer is stored as
 * it is. With no answer the side from 0x24de70 goes to the top byte of the flags word and the
 * kind stays 0xbff. Assumed: the signatures of 0x277cd0 and 0x2774f0 as above; int results
 * for 0x1e1580 and 0x1e1640. */
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
void TaLogic_CommandTrapUnit__0012ce50(struct DotrPositionOrder *order);

/* 0x164020: the 0x113bb0 shape with two codes: 297 runs the five-pass
 * 0x2770e0/0x277160 loop (seventh and ninth arguments 1, key TaLogic_GetWarnHideLvl__001676d0(side)),
 * 79 marks 0x2ff with the cell when SzDuel_GetLP(side) < 2000; then TaLogic020_CommandBtlUnit(order, 1).
 * 0x163c50 is declared with a DotrTryOrder pointer in order_tries; the same
 * order object is assumed and passed here as a DotrPositionOrder. */
extern void TaLogic020_CommandBtlUnit(struct DotrPositionOrder *order, int flag);
void TaLogic020_CommandEfcMonster(struct DotrPositionOrder *order);

/* 0x11d7b0 with the signature order_tries.h declares. */
extern int TaLogic004_IsEmptyAtkLine(int side, unsigned char lineIndex);

/* Like 0x123500 without the early return: stores spot -1 and cost 0xff, then
 * for k = 0..3 with TaLogic004_IsEmptyAtkLine(0, k) == 1 costs both TaLogic004_GetAtkLineStart(0, k) and
 * TaLogic004_GetAtkLineEnd(0, k) through TaLogic_GetRoot2(unit, bytes, column, row, x, y, id, 7) (id
 * from 0x1e3790, 0xffff when negative) and keeps every cost that is not
 * negative and below the stored one, with its spot.
 * Assumed: as for TaLogic005_GetNearDefensePoint (int word, DotrSpotBest view of the fourth argument). */
void TaLogic004_GetNearAtkLineEdge(int unit, int column, int row, struct DotrSpotBest *best);

#endif
