#ifndef DOTR_EE_FINISH_MOVE_H
#define DOTR_EE_FINISH_MOVE_H

/* Ta_FinishMove, the step dispatcher that finishes a unit's move onto a square (traps, effects,
 * battle, removal).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* The record Ta_FinishMove fills, as this function reads and writes it. The first four
 * members (16 bytes) have the layout and the member names of `struct jp_7b52` in
 * action_reaction.h (Chk_TrapA's record, from the Japanese build's debug information); this
 * view goes on past them: a short at 16 that this function never reads and a one-bit signed
 * field in bit 0 of byte 18, set by step 0 and tested by steps 2 and 17. The address of
 * that trailing part is also handed to Ta_Manage_ActEfc as an argument of its own.
 * Assumed: that the trailing part belongs to the same object (the caller, SzDuel_Act, passes
 * its record at 0x3180d0 as an integer), the member names `sub`, `h`, `open` and the
 * meaning "the unit was face down with status bit 2 when the move began". A separate tag
 * and not an extension of jp_7b52 because no evidence says Chk_TrapA's record has the
 * trailing part. */
struct DotrFinishMoveSub {
    short h;
    signed char open : 1;
};
struct DotrFinishMoveInfo {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
    struct DotrFinishMoveSub sub;
};

/* Callees as this function calls them, with the prototypes of the headers that declare them
 * (state_grid.h, state_records.h, state_queries.h, small_misc.h, slot_seeks.h, side_table.h,
 * cell_tints.h, sound_gate.h, item_release.h, packed_item.h). Assumed: every parameter name
 * and the result types; Ta_GetCurrentSide is called with a unit's (side, id) pair. */
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int Ta_IsUnitControl(int a, int b);
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
extern int Ta_IsLDAbl(int ability, int side);
extern void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);
extern int Ta_IsResistAll(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern void TaUtil_DoOpen(int side1, int pos2, int mode);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
extern void Ta_UnitFade(int side, int pos, int dir, int value, int flags);
extern int SzDuel_SetUnitCtlGoalC(int side, int pos, int value);
extern int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value);
extern int SzDuel_GetUnitCtlRateC(int side, int pos);
extern int SzDuel_GetUnitCtlGoalC(int a, int b);
extern int SzDuel_GetUnitCtlRateM(int side, int pos);
extern int SzDuel_GetUnitCtlGoalM(int a, int b);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_AddGrave(int side, int cardNo);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern int SzData_CalcExp(int side, int cardNo, int operation, int expChange);
extern int SzDuel_GetUnit(int side, int pos);
extern int SzEffect_SetBusyFlag2(int packed, int flag);
extern int Ta_GetAgreeOwnerIDon22bitFlagLst(int ownerId, unsigned flagList);

/* Callees that take this function's record, under symbols of their own because the groups
 * that define them declare other record types. They link by address. Assumed for all: the
 * parameter counts beyond what this function's call sites pass, the parameter names and the
 * result types.
 * - 0x250740 Ta_GetUnitEffect: the result is read as a pointer to unsigned words; only word
 *   0 is read (`>> 8 == 81`), and the pointer is handed on to Ta_Manage_ActEfc.
 * - 0x263930 Ta_Manage_ActEfc: TEN arguments here (effect words, two unit pairs, the cell,
 *   the record, the record's trailing part, and a mode 1 or 2).
 * - 0x246810 Battle: seven arguments (defender pair, mover pair, cell, record).
 * - 0x247f40 SelfHeap: five. 0x2517d0 Ta_Manage_DestEfc: eight (the last a 0/1 flag).
 * - 0x24feb0 Ta_IsVirusAll: the record. 0x24f830 Ta_Do_MoveEfc: unit pair, a cell, record.
 * - 0x252440 Ta_Open: unit pair, record, a step number. */
extern unsigned *func_00250740_finishmove(int a, int b, int n);
extern int func_00263930_finishmove(unsigned *e, int a, int b, int c, int d, int x, int z,
                                    struct DotrFinishMoveInfo *info, struct DotrFinishMoveSub *sub, int mode);
extern int func_00246810_finishmove(int a, int b, int c, int d, int x, int z, struct DotrFinishMoveInfo *info);
extern int func_00247f40_finishmove(int a, int b, int c, int d, struct DotrFinishMoveInfo *info);
extern int func_002517d0_finishmove(int a, int b, int c, int d, int x, int z, struct DotrFinishMoveInfo *info,
                                    int flag);
extern int func_0024feb0_finishmove(struct DotrFinishMoveInfo *info);
extern int func_0024f830_finishmove(int a, int b, int x, int y, struct DotrFinishMoveInfo *info);
extern int func_00252440_finishmove(int a, int b, struct DotrFinishMoveInfo *info, int n);

/* 0x249680 Ta_FinishMove (name from the Japanese build; the parameter names follow
 * Chk_TrapA's, whose first five parameters have the same roles). Runs step `PrNo & 0xff` of
 * finishing the move of unit (nSide, nID) onto cell (nX, nZ) and returns a packed word: the
 * next step in the low byte, flags in bits 8 to 12 and a value from bit 16. Assumed: the
 * prototype (six parameters and an int result, as the caller SzDuel_Act passes and
 * stores them) and the meaning given to the steps and bits. */
int Ta_FinishMove(int nSide, int nID, int nX, int nZ, struct DotrFinishMoveInfo *eInfo, int PrNo);

#endif
