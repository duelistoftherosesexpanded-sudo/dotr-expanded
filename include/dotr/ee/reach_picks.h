#ifndef DOTR_EE_REACH_PICKS_H
#define DOTR_EE_REACH_PICKS_H

/* Look-alike routines (0x149690 and copies) that scan the cells within an item's reach of its
 * position and write the best one to a result record; same callees, per-copy side helpers.
 * Also 0x27b9b0, a free-neighbour test for one unit.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

#include "dotr/ee/lower_rules.h"

/* Callees not declared in lower_rules.h, with the arguments these callers pass.
 * TaLogic_IsOpenUnit__0014ae60 and its copies are defined with two parameters (cell_verdicts.h);
 * the callers here also leave the order in a2, so a third is declared.
 * TaLogic_IsRunPassRoot forwards its seventh and eighth arguments (small_misc.h has both as
 * int); here the seventh is the order and the eighth a byte loaded with lbu.
 * func_0027b1b0_mode is TaLogic_InitRogicControl with the unsigned char mode of its definition
 * (small_misc); lower_rules.h's func_0027b1b0_order has int, which makes the callers here
 * widen the byte through v0, and a cast at the call becomes an indirect call. */
extern void func_0027b1b0_mode(struct DotrRuleOrder *order, unsigned char mode);
extern int GetCap(int limit, int value);
extern int TaLogic_IsRunPassRoot(int a, int b, int c, int d, int e, int f, struct DotrRuleOrder *g, unsigned char h);
extern int TaLogic_IsAttackedNow2(int row, int column, int a, int b, int c);
extern int TaLogic_IsOpenUnit__0014ae60(int a, int b, struct DotrRuleOrder *order);
extern int TaLogic_IsOpenUnit__0015cbb0(int a, int b, struct DotrRuleOrder *order);
/* TaLogic_GetWarnHideLvl__00158780 is defined in position_rules with this signature. */
extern int TaLogic_GetWarnHideLvl__00158780(int side);
extern int TaLogic_IsOpenUnit__00158360(int a, int b, struct DotrRuleOrder *order);
/* TaLogic_GetWarnHideLvl__0014f9b0 is defined in position_rules with this signature;
 * TaLogic_IsOpenUnit__0014f490 is called here with a third argument (cell_verdicts declares two). */
extern int TaLogic_GetWarnHideLvl__0014f9b0(int side);
extern int TaLogic_IsOpenUnit__0014f490(int a, int b, struct DotrRuleOrder *order);
/* Callees of 0x152100: TaLogic_GetWarnHideLvl__00153ed0 is defined in position_rules with this
 * signature; TaLogic_IsOpenUnit__00153ae0 (open_unit_reply, two parameters there) is called here
 * with the same three arguments as 0x14f490. */
extern int TaLogic_GetWarnHideLvl__00153ed0(int side);
extern int TaLogic_IsOpenUnit__00153ae0(int a, int b, struct DotrRuleOrder *order);

/* Callees of 0x1607d0 and 0x170940, as called here (one argument; three arguments). 0x162580 and
 * 0x172310 are defined in position_rules (free_spots.h has the `_free` view with the same
 * signature); 0x1620b0 is defined in cell_verdicts with two arguments and is called here with a
 * third; TaLogicCom_IsOpenUnit__0016b610 (open_unit_com) takes three. */
extern int TaLogic_GetWarnHideLvl__00162580(int side);
extern int TaLogic_IsOpenUnit__001620b0(int a, int b, struct DotrRuleOrder *order);
extern int TaLogic_GetWarnHideLvl__00172310(int side);
extern int TaLogicCom_IsOpenUnit__0016b610(int side, int pos, struct DotrRuleOrder *order);
/* Callees of 0x13be60 and 0x115850, same two roles. 0x13da70 and 0x117c30 are declared like this in
 * lower_rules.h; TaLogic_IsOpenUnit__0013d3d0 (open_units) takes the three arguments; 0x117870 is
 * defined in cell_verdicts with three ints and is passed the order as the third here. */
extern int TaLogic_GetWarnHideLvl__0013da70(int side);
extern int TaLogic_IsOpenUnit__0013d3d0(int a, int b, struct DotrRuleOrder *order);
extern int TaLogic_GetWarnHideLvl__00117c30(int side);
extern int TaLogic_IsOpenUnit__00117870(int a, int b, struct DotrRuleOrder *order);
/* Callees of 0x1658f0 and 0x169a10, same two roles. 0x1676d0 is defined in position_rules with
 * this signature; TaLogic_IsOpenUnit__001670a0 (open_units) takes the three arguments. 0x16b0e0 is
 * defined without parameters (low_leaves) and is passed the side here (free_spots.h has the same
 * view); 0x16ae30 is defined in lower_rules with two ints and is passed three values here. */
extern int TaLogic_GetWarnHideLvl__001676d0(int side);
extern int TaLogic_IsOpenUnit__001670a0(int a, int b, struct DotrRuleOrder *order);
extern int func_0016b0e0_free(int side);
extern int func_0016ae30_pick(int a, int b, struct DotrRuleOrder *order);

int TaLogic_GetMinimumTask_RunMission__00149690(struct DotrRuleOrder *order, int side, struct DotrRuleResult *task, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__0015b250(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__00157480(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__0014df80(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__00152100(struct DotrRuleOrder *order, int side, struct DotrRuleResult *minTask, unsigned char goalId);
int TaLogic_GetMinimumTask_RunMission__001607d0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__00170940(struct DotrRuleOrder *order, int side, struct DotrRuleResult *task, unsigned char goalId);
/* 0x13be60 passes 2 where the other copies pass 3 to 0x276cc0; 0x115850 passes 1 to 0x27e360 on
 * every path. Callers in lower_rules see 0x115850 as func_00115850_rest (int mode). */
int TaLogic_GetMinimumTask_RunMission__0013be60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__00115850(struct DotrRuleOrder *order, int side, struct DotrRuleResult *task, unsigned char mode);
/* 0x1658f0 and TaLogic_GetMinimumTask_RunMission__00169a10 (1476 bytes) pass 2 to 0x276cc0 like
 * 0x13be60, and 1 where the other copies pass a final 0 to 0x276cc0, 0x27e360, 0x27bbc0 and 0x27be10. */
int TaLogic_GetMinimumTask_RunMission__001658f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__00169a10(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* 0x124f60 and TaLogic_GetMinimumTask_RunMission__001336a0 (1332 bytes) are the same routine
 * without the 0x1e2100 gate (no kind-14 exit). 0x124f60 passes 2 to 0x276cc0, 0x1336a0 passes 3.
 * Their callees in the same two roles: 0x1254a0 and 0x134ae0 are defined in position_rules (int
 * side); 0x126380 and 0x134b40 (open_units; the latter declared there with two parameters) are
 * called with the three arguments. 0x124f60 ends with 0x126940 where the others call 0x27b1b0:
 * func_00126940_mode is that function as called here (order, mode byte); pick_copies.h has it with a struct
 * DotrPickMask pointer. */
extern int func_001254a0_free(int side);
extern int TaLogic_IsOpenUnit__00126380(int a, int b, struct DotrRuleOrder *order);
extern void func_00126940_mode(struct DotrRuleOrder *order, unsigned char mode);
extern int TaLogic_GetWarnHideLvl__00134ae0(int side);
extern int TaLogic_IsOpenUnit__00134b40(int a, int b, struct DotrRuleOrder *order);
int TaLogic_GetMinimumTask_RunMission__00124f60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__001336a0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* 0x144ec0 and TaLogic_GetMinimumTask_RunMission__001402f0 (1332 bytes) are 0x1336a0 with other
 * callees in the same two roles (both pass 3 to 0x276cc0). 0x1461c0 and 0x141b30 are defined in
 * position_rules (int side); 0x146220 and 0x141b90 are defined in cell_verdicts with two arguments
 * and are called here with the order as a third. */
extern int TaLogic_GetWarnHideLvl__001461c0(int side);
extern int TaLogic_IsOpenUnit__00146220(int a, int b, struct DotrRuleOrder *order);
extern int TaLogic_GetWarnHideLvl__00141b30(int side);
extern int TaLogic_IsOpenUnit__00141b90(int a, int b, struct DotrRuleOrder *order);
int TaLogic_GetMinimumTask_RunMission__00144ec0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__001402f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* 0x1291c0 and TaLogic_GetMinimumTask_RunMission__0011a4d0 (1332 bytes) are 0x1336a0 again with
 * their own side and tail callees and, in the role of 0x27b1b0, 0x12b830 and 0x11c3e0: those are
 * defined in pick_copies with a DotrPickMask pointer, and the `_mode` views are the same functions
 * as called here (order, mode byte). 0x12b1b0 and 0x11aa10 are defined in position_rules (int
 * side); 0x11c140 is defined in cell_verdicts with an unsigned record pointer, so
 * func_0011c140_order is that function as called here; TaLogic_IsOpenUnit__0012b210 (open_units)
 * takes the three arguments. 0x11a4d0 passes 1 where the others pass 0 as the fourth argument of
 * 0x27e360 for a pick away from the item's own cell. */
extern int TaLogic_GetWarnHideLvl__0012b1b0(int side);
extern int TaLogic_IsOpenUnit__0012b210(int a, int b, struct DotrRuleOrder *order);
extern void func_0012b830_mode(struct DotrRuleOrder *order, unsigned char mode);
extern int TaLogic_GetWarnHideLvl__0011aa10(int side);
extern int func_0011c140_order(int a, int b, struct DotrRuleOrder *order);
extern void func_0011c3e0_mode(struct DotrRuleOrder *order, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__001291c0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__0011a4d0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* TaLogic_GetMinimumTask_RunMission__0011f540 (1332 bytes) is 0x11a4d0 with its own side and tail
 * callees and, in the role of 0x27b1b0, 0x1215e0 (defined in pick_copies as
 * TaLogic004_InitRogicControl with a mask pointer; func_001215e0_mode is that function as called
 * here, as in atk_missions.h). 0x11fa80 is defined in position_rules (int side) and 0x1211b0 in
 * cell_verdicts with these parameters. Unlike 0x11a4d0 it passes 0 as the fourth argument of
 * 0x27e360 for a pick away from the item's own cell. */
extern int TaLogic_GetWarnHideLvl__0011fa80(int side);
extern int TaLogic_IsOpenUnit__001211b0(int x, int y, struct DotrRuleOrder *unused);
extern void func_001215e0_mode(struct DotrRuleOrder *order, unsigned char mode);
int TaLogic_GetMinimumTask_RunMission__0011f540(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* TaLogic_GetMinimumTask_RunMission__0012e820 (1356 bytes) is 0x1336a0 with a fifth argument, a
 * byte it keeps on the stack and passes as the fourth argument of its tail callee 0x130940.
 * 0x130780 is defined in position_rules (int side); 0x130940 is defined in cell_verdicts as (x, y,
 * unused, int flag), so func_00130940_order is that function as called here (the order pointer
 * third, the byte fourth). */
extern int TaLogic_GetWarnHideLvl__00130780(int side);
extern int func_00130940_order(int a, int b, struct DotrRuleOrder *order, unsigned char flag);
int TaLogic_GetMinimumTask_RunMission__0012e820(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode,
                  unsigned char tail_flag);

/* TaLogic_GetMinimumTask_RunMission__001374d0 (1308 bytes) is 0x1336a0 without the side callee: it
 * never reads its second argument, passes 0 and 1 as the last two arguments of 0x276cc0 and 1 as
 * the last argument of 0x27e360, 0x27bbc0 and 0x27be10. Its tail callee
 * TaLogic_IsOpenUnit__00139180 (open_units, two parameters there) is called with three arguments.
 * lower_rules.h declares this function as func_001374d0_rest (int mode) for its callers. */
extern int TaLogic_IsOpenUnit__00139180(int a, int b, struct DotrRuleOrder *order);
int TaLogic_GetMinimumTask_RunMission__001374d0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* TaLogic_GetMinimumTask_RunMission__0010fa60 (1420 bytes) is 0x1336a0 with the side routine
 * expanded in place: 0x1df470 above 1500, then 0x1e0150 at least 4, each outcome giving 0 as the
 * fifth argument of 0x276cc0; the away pick passes 1 to 0x27e360. 0x1df470 gets the side in a0 here
 * (func_001df470_side, as in cell_verdicts.h). Its tail callee 0x10ee00 is defined in cell_verdicts
 * with an unsigned record pointer; func_0010ee00_order is that function given the order. */
extern int func_001df470_side(int side);
extern int SzDuel_GetUnitMonsterCount(int side);
extern int func_0010ee00_order(int x, int y, struct DotrRuleOrder *order);
int TaLogic_GetMinimumTask_RunMission__0010fa60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* TaLogic_GetMinimumTask_ReviveMission__0012ed70 (1492 bytes) is 0x1336a0 with two early exits
 * before the first score: a non-negative answer of TaLogic007_IsRevivePlay (defined in grid_scans)
 * gives a kind-12 result through 0x27dbc0 (state_queries) with the constant 30, and bit 4 of
 * 0x1e2100 gives the kind-14 result without asking for the kind. It has no tail callee: the word at
 * +0x0c is the constant 2 (1 on the kind-12 path). Side callee 0x130780. */
extern int TaLogic007_IsRevivePlay(int side);
extern int TaLogic_GetGraveEntryAfterScreen(int side, int target, int mode);
int TaLogic_GetMinimumTask_ReviveMission__0012ed70(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x27da40 as defined in state_queries; the packed result (index in the top byte, target in the
 * next) is read from the uses in 0x129700. */
extern int TaLogic_GetMaxAPGraveCard(int side, int mask);
int TaLogic_GetMinimumTask_ReviveMission__00129700(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* 0x27b9b0. 1 when the unit wUnit (high byte, low byte) has a location, status bit 0
 * clear and a free orthogonal neighbour cell on the 7x7 map that is not field 8 without
 * bit 4 of its move word (0x2464f0) and not field 9 with 0x24fe40 answering 1; else 0.
 * Prototype and locals from the Japanese build's debug information; the callee
 * declarations are the int views of lower_rules.h. Ta_IsVirus_Chk3 (0x24fe40) is
 * declared with one parameter in state_queries.h; this caller passes three, hence the
 * view name. */
extern int func_0024fe40_rule(int item, int x, int y);
int TaLogic_IsUnitReadyMove(unsigned short wUnit);

#endif
