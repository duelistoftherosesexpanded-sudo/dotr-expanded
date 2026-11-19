#ifndef DOTR_EE_END_PHASE_H
#define DOTR_EE_END_PHASE_H

/* Adjacent functions 0x2724f0-0x273458 that the Japanese build's debug information places in
 * EndPhase.c, and Ta_Do_StandbyPhase (0x267c50, TurnStanby.c); they share the SzDuel_/SzData_
 * callees.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Callee signatures as declared in state_grid.h, state_records.h and side_table.h. */
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzData_CalcExp(int side, int cardNo, int operation, int expChange);

/* Further callee signatures as declared in the state_grid, state_records,
 * state_queries, small_misc and low_leaves headers; SzDuel_GetUnit is only tested
 * against null here. Ta_GetSommonPiecePreference (0x24e410, summon_cells) is declared as
 * called here. */
extern int SzDuel_GetTurnOwner(void);
extern int SzDuel_GetTurnFirst(void);
extern int SzDuel_GetTurnCount(void);
extern void *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetField(int x, int y);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
extern int SzDuel_SetUnitSpellRest(int side, int pos, int value);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_SetUnitCurseRest(int side, int pos, int value);
extern int SzDuel_GetUnitCurseFlag(int a, int b);
extern int SzDuel_SetUnitCurseFlag(int a, int b, int flag);
extern int Ta_IsLDAbl(int ability, int side);
extern int Ta_IsVirusAll(int *out);
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_GetLDRange(int side);
extern int Ta_IsSquareRange(int nX, int nZ, int mX, int mZ, int R);
extern int Ta_IsLD_DestObj(int side, int objKind);
extern int Ta_IsLD_CurseObj(int side, int objKind);
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);
extern int Ta_IsUnitControl(int a, int b);
extern int GetCap(int limit, int value);
extern int TaPerm_IsPermCurseActive(int a, int b);
extern int Ta_GetSommonPiecePreference(int a, int direction);

/* Effect request filled by the phase steps; layout and member names from
 * the Japanese build's debug information (tag unnamed in the original). */
struct jp_908da {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};

/* Prototypes from the Japanese build's debug information. */
int Ta_Do_EndPhase(int PrNo, struct jp_908da *eInfo);
int Ta_Do_DuelEnd(int nSide, unsigned char bCond);

/* Callee signatures for Ta_Do_StandbyPhase as declared in the state_records,
 * state_grid, state_queries, pick_copies, slot_seeks, panel_draws and
 * resource_support headers. Ta_ManageNaturalEffectInStandbyPhase is declared in
 * perm_efc.h with its own request type, hence the cast at the call;
 * Ta_ManageNaturalEffectInStandbyPhase2 (0x266910, perm_efc) is declared
 * with a signature assumed from the call. */
struct jp_65da1;
extern int SzDuel_GetSummonCharge(int side);
extern void SzDuel_SetTurnCount(int value);
extern unsigned *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int SzDuel_GetUnitDelayCount(int side, int pos);
extern int SzDuel_SetUnitDelayCount(int side, int pos, int value);
extern int Ta_ManageNaturalEffectInStandbyPhase(int nSide, int nID, struct jp_65da1 *eInfo);
extern int SzDuel_Kind2Type(int kind);
extern int Ta_IsExzodia(int side);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int Ta_GetExzodiaPartsUnit(int side, int limbId);
extern int TaTuto_IsTuto(int side);
extern int Ta_GetSummonCalcRevision(int who);
extern int SzDuel_CalcSummonPower(int side, int operation, int amount);
extern int SzDuel_InqDestinyFlag(int side);
extern int Ta_IsDestinyDraw(int side);
extern void SzDuel_SetDestinyFlag(int side, int value);
extern void SzDuel_SetHandPhase(int side, int value);

/* Effect request and unit effect word pair used by Ta_Do_StandbyPhase
 * (0x267c50, TurnStanby.c in the Japanese build); layouts and member names
 * from that build's debug information (tags unnamed in the original). */
struct jp_6826f {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
struct jp_6835c {
    unsigned int dwID;
    unsigned int dwParam;
};
extern int Ta_ManageNaturalEffectInStandbyPhase2(int nSide, int nID, struct jp_6826f *eInfo);
int Ta_Do_StandbyPhase(int PrNo, struct jp_6826f *eInfo);

#endif
