#ifndef DOTR_EE_MOVE_MISSIONS_H
#define DOTR_EE_MOVE_MISSIONS_H

/* Copies of TaLogic_GetMinimumTask_MoveMission (one per TaLogicNNN unit): same body, each
 * calling its own unit's IsOpenUnit and InitRogicControl.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */
#include "dotr/ee/reach_picks.h"

/* TaLogic_GetRoot (0x279230, small_misc) as these callers show it: the third argument is the cell array.
 * small_misc.h declares that parameter as int; with int the array address is formed before the other
 * arguments, which the original does not do. */
int TaLogic_GetRoot(int a, int b, struct DotrRuleNibbles *cells, int d, int e, int f, int g, int h, unsigned char i);

/* Arguments: the order, the side, the result record, the mode (1: the goal is the upper half of the order's
 * word at +8, otherwise the halfword there) and a flag handed to TaLogic_IsDefStyleUnit. Both byte
 * parameters are stored with sb, so they are unsigned char here. The result is always 0. */
int TaLogic_GetMinimumTask_MoveMission__00137fb0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
/* 0x1459e0: IsOpenUnit (0x146220) and GetWarnHideLvl (0x1461c0) come from reach_picks.h. It hands 0 as
 * IsDefStyleUnit's last argument and (3, 1, hide level) as GetUnitWarning's last three. */
int TaLogic_GetMinimumTask_MoveMission__001459e0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
/* 0x140e10, 0x1341c0, 0x12a2e0: as 0x1459e0 with their own unit's IsOpenUnit and GetWarnHideLvl
 * (reach_picks.h); GetUnitWarning's fourth argument is 3, 4 and 5. 0x12a2e0 resets the order
 * through its unit's InitRogicControl (0x12b830, func_0012b830_mode in reach_picks.h). */
int TaLogic_GetMinimumTask_MoveMission__00140e10(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__001341c0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__0012a2e0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
/* 0x12f930 stores a sixth byte parameter (sb t1) and hands it to its unit's four-argument IsOpenUnit
 * (0x130940 as func_00130940_order in reach_picks.h, a view with a byte flag; with the int flag
 * cell_verdicts.h declares, this copy compiles to different code). */
int TaLogic_GetMinimumTask_MoveMission__0012f930(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag, unsigned char open_flag);
/* 0x1106f0: IsOpenUnit is TaLogic_IsOpenUnit__0010ee00 (func_0010ee00_order in reach_picks.h); its unit's
 * GetWarnHideLvl is expanded in place (life points over 1500, then the monster count, every path 0)
 * and GetUnitWarning's fourth argument is 2. */
int TaLogic_GetMinimumTask_MoveMission__001106f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
/* 0x125b70: as 0x1106f0 with its unit's IsOpenUnit (0x126380) and InitRogicControl (0x126940, both
 * declared in reach_picks.h); the hide level expanded in place is 2000 up to 1000 life points, else
 * 500 with three or more monsters and 1000 with fewer, of which GetUnitWarning receives the low
 * byte. */
int TaLogic_GetMinimumTask_MoveMission__00125b70(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);

/* The 0x868-byte copies: as 0x1459e0, with one more exit before the route search: a unit whose
 * status bit 4 is set and whose kind (>> 5) is 1 or 5 has that status cleared (SetUnitStatus(.., 0,
 * 1)) and the result is the unit's own position with kind 14. Each copy calls its unit's IsOpenUnit
 * and GetWarnHideLvl (reach_picks.h; of the two GetWarnHideLvl below, 0x15d120 is defined in lower_rules and
 * 0x14b330 in position_rules, both with this signature). */
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int TaLogic_GetWarnHideLvl__0015d120(int side);
extern int TaLogic_GetWarnHideLvl__0014b330(int side);
int TaLogic_GetMinimumTask_MoveMission__0014eb30(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__001664a0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__0015be00(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__00161380(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__0014a240(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__001162f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__001714f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__0016a5c0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__0013ca10(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);

/* 0x152cd0: a 0x868-byte copy (IsOpenUnit 0x153ae0, GetWarnHideLvl 0x153ed0) that first asks
 * TaLogic015_IsActiveMode and keeps the low byte: GetUnitWarning's fourth argument is 5 when it is
 * set, else 2. */
extern int TaLogic015_IsActiveMode(void);
int TaLogic_GetMinimumTask_MoveMission__00152cd0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
/* 0x157a50: a 0x868-byte copy (IsOpenUnit 0x158360, GetWarnHideLvl 0x158780) with a sixth byte
 * argument, assumed from the t1 read at entry: when it is 1 the route comes from TaLogic_GetInvadeRoot (goal
 * unit 0xffff, 0, 30, the hide level as 16 bits) instead of TaLogic_GetRoot. */
int TaLogic_GetMinimumTask_MoveMission__00157a50(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag, unsigned char route);

/* 0x11b680, 0x1206f0: the 0xabc-byte copies. As
 * TaLogic_GetMinimumTask_MoveMission__001106f0 / TaLogic_GetMinimumTask_MoveMission__00125b70 (hide level
 * expanded in place: every path 0 for 0x11b680 with the tests 1500 life points and three monsters;
 * 2000 / 500 / 1000 for 0x1206f0), with two exits ahead of the search: when the high byte of the order's
 * halfword at +4 is 15 or 16 the goal is the unit's own column and the row GetCap(7, row + (GetMoveRev(unit,
 * column, row) & 15)) or GetCap(0, row - ...); TaLogic_GetRoot is asked with 2 instead of 7 and no goal cell
 * falls back to the unit's cell. GetUnit and GetMoveRev are used as lower_rules.h declares them (int unit).
 * 0x11b680 calls IsOpenUnit 0x11c140 and InitRogicControl 0x11c3e0 (reach_picks.h); 0x1206f0 calls 0x1211b0
 * and 0x1215e0 and hands GetUnitWarning 4 where 0x11b680 hands 3. */
extern int GetCap(int limit, int value);
extern int TaLogic_IsOpenUnit__001211b0(int x, int y, struct DotrRuleOrder *unused);
extern void func_001215e0_mode(struct DotrRuleOrder *order, unsigned char mode);
int TaLogic_GetMinimumTask_MoveMission__0011b680(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);
int TaLogic_GetMinimumTask_MoveMission__001206f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);

/* 0x27d020 TaLogic_GetMinimumTask_EncMission (driver.c). Parameter names and the running minimum
 * kept in WarnLvl follow the Japanese build's debug information, which lists seven parameters; the
 * code reads an eighth register (t3) for IsWarnHide and never reads t2, so an unused seventh is
 * assumed here. The structures are the project's views of the same layouts (DotrRuleOrder for the
 * unit order, DotrRuleResult for the task). func_00276cc0_enc is TaLogic_GetUnitWarning with a byte
 * fourth and a halfword fifth argument, as this caller's `andi`/`lhu` and the callee's own `sh t0`
 * at entry show; lower_rules.h declares both as int. */
extern int func_00276cc0_enc(int x, int z, int side, unsigned char d, unsigned short e, unsigned char f);
int TaLogic_GetMinimumTask_EncMission(struct DotrRuleOrder *pUnit, int nSide, struct DotrRuleResult *pTask, unsigned char Is1st, unsigned char IsView, int WarnLvl, int unused, unsigned char IsWarnHide);

#endif
