#ifndef DOTR_EE_ATK_MISSIONS_H
#define DOTR_EE_ATK_MISSIONS_H

/* Copies of TaLogic_GetMinimumTask_AtkMission (one per TaLogicNNN unit): same body, each calling
 * its own unit's GetWarnHideLvl, IsOpenUnit, IsOpenMoveUnit and InitRogicControl.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */
#include "dotr/ee/reach_picks.h"

/* Callees as these copies call them. SzDuel_GetUnitAP (0x1e0290, state_grid) and
 * TaLogic006_IsOpenMoveUnit (0x12b730, state_queries) are declared as in their own groups.
 * reach_picks.h supplies the order/result views, the nibble cells and the `_mode` / `_rule`
 * views of 0x27b1b0, 0x12b830, 0x2464f0, 0x27a090 and 0x27afc0. The result's shouldFlipUp takes
 * IsOpenUnit's answer or the "an open move was found" flag; that reading is assumed. */
extern int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
extern int TaLogic006_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00129d00(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic008_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00133be0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic022_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00170f10(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic019_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00160da0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic014_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__0014e550(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic_GetWarnHideLvl__0015d120(int side);
extern int TaLogic018_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__0015b820(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic011_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00140830(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic012_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00145400(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic_GetWarnHideLvl__0014b330(int side);
extern int TaLogic013_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00149c60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic010_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__0013c430(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic020_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00165ec0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
extern int TaLogic_GetWarnHideLvl__0016b0e0(int side);
extern int TaLogic021_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__00169fe0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x12f350 takes a fifth argument and hands it to its IsOpenUnit (0x130940, cell_verdicts) with
 * the order. */
extern int TaLogic_IsOpenUnit__00130940(int x, int y, struct DotrRuleOrder *unused, int flag);
extern int TaLogic007_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__0012f350(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, int flag);
/* 0x1526d0 first asks TaLogic015_IsActiveMode (0x153ab0, position_rules) and keeps the answer
 * as a byte: the goal loop's warning kind is 5 when it is nonzero, else 2. */
extern int TaLogic015_IsActiveMode(void);
extern int TaLogic_GetWarnHideLvl__00153ed0(int side);
extern int TaLogic015_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__001526d0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x1379f0 asks for no warning level: it passes 0 where the other copies pass their
 * GetWarnHideLvl answer. */
extern int TaLogic009_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__001379f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x11aa70, 0x125500 and 0x11fae0 have their unit's GetWarnHideLvl body (0x11aa10, 0x1254a0,
 * 0x11fa80) expanded in place instead of called, and reset the order once more after a goal
 * cell was chosen. They reset with their unit's InitRogicControl where 0x129d00 does, and after
 * the chosen cell; 0x11aa70 does so in the last branch as well. 0x126940 and 0x1215e0 are
 * TaLogic005/004_InitRogicControl, declared in pick_copies.h with a DotrPickMask pointer; that
 * header cannot be included with lower_rules.h (conflicting views of other callees), so the
 * `_mode` names are those functions as called here (order, mode byte), like reach_picks.h's
 * func_0011c3e0_mode. */
extern void func_00126940_mode(struct DotrRuleOrder *order, unsigned char mode);
extern void func_001215e0_mode(struct DotrRuleOrder *order, unsigned char mode);
extern int TaLogic003_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
extern int TaLogic005_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic_GetMinimumTask_AtkMission__0011aa70(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_AtkMission__00125500(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x11fae0 also keeps the root search's answer and hands it to its IsOpenMoveUnit (0x1214e0,
 * grid_scans) as a seventh argument. Its IsOpenUnit (0x1211b0, cell_verdicts) reads two
 * arguments; the order is passed as a third. */
extern int TaLogic_IsOpenUnit__001211b0(int x, int y, struct DotrRuleOrder *unused);
extern int TaLogic004_IsOpenMoveUnit(int row, int column, int x, int y, int to_x, int to_y, int kind);
int TaLogic_GetMinimumTask_AtkMission__0011fae0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x115e20: IsOpenUnit (0x117870) and GetWarnHideLvl (0x117c30) come from reach_picks.h; no
 * IsDefStyleUnit and no IsOpenMoveUnit call. The constants it stores in shouldFlipUp / atkDefPosition
 * are not interpreted. */
int TaLogic_GetMinimumTask_AtkMission__00115e20(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x156fc0: as 0x115e20 without the IsOpenUnit call; GetWarnHideLvl is 0x158780. */
int TaLogic_GetMinimumTask_AtkMission__00156fc0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* 0x10fff0: no unit-specific callees. IsOpenUnit (0x10ee00) is declared as in cell_verdicts and
 * given the order; CheckLandAgreement2 (0x24df00) as in state_queries. The level body and the
 * open-move test are expanded in place; that both were inline functions in the original is
 * assumed. */
int TaLogic_IsOpenUnit__0010ee00(int x, int y, unsigned *record);
extern int CheckLandAgreement2(int field, int side, int pos);
int TaLogic_GetMinimumTask_AtkMission__0010fff0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
#endif
