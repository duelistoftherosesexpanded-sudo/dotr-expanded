#ifndef DOTR_EE_TUTO_SCRIPTS_H
#define DOTR_EE_TUTO_SCRIPTS_H

/* Tutorial scripts L034 and L035 (0x175fe0, 0x178590): each steps a 16-bit step word at +0x1a2
 * through duel set-up calls and tutorial messages, like L033.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/ai-entries.md. */

/* The tutorial record as these scripts use it; the same layout as L033's view
 * (struct DotrTutoL033Record in tuto_l033.h): a frame counter at +0x162, the step at +0x1a2,
 * a wait counter at +0x1a4 and the view backup area from +0x1b0 passed to
 * TaTuto_BkupOriginalView / TaTuto_RestoreOriginalView. Assumed: field meanings from their
 * use here; size unknown. */
struct DotrTutoScriptRecord {
    unsigned char pad0[0x162];
    unsigned short timer;
    unsigned char pad164[0x3e];
    unsigned short step;
    unsigned short wait;
    unsigned char pad1A6[0xa];
    unsigned char text[1];
};

/* Callees (names from the Japanese build's symbols); consistent with tuto_l033.h where both
 * declare a function. SzPad_SetAnalogInfo, TaTuto_DbgInit034, TaTuto_FocusUnit__00175ba0,
 * TaTuto_SetDeck035 and the TaTuto_* helpers are int-parameter prototypes (0x80 passes as a
 * full int). TaTuto_DbgInit034 takes one argument as called
 * here (state_records.h has another view). SzDuel_SetIgnoreJudge takes a 64-bit value;
 * SzDuel_SetField, SzDuel_SetUnitCurseRest and SzDuel_SetDestinyFlag take a signed char last.
 * Assumed: TaTuto_ControlFade (kind, direction, flag) and TaTuto_OpenTutoMsg (first, last,
 * record) return 1 when done; TaTuto_DrawTrapArea (record, a, x, y) draws a marker. */
extern int SzDuel_GetWindowDisp(void);
extern void SzDuel_SetWindowDisp(int on);
extern void SzDuel_SetWindowDraw(int on);
extern void SzPad_SetAnalogInfo(int port, int axis, int value);
extern int SzPad_Check(int port, int kind, int mask);
extern void SzDuel_SetIgnoreJudge(long long on);
extern int SzEffect_IsModelStandby2(int model);
extern int SzDuel_SetPlayerCur(int side, int x, int y);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern int SzDuel_CalcSummonPower(int side, int operation, int amount);
extern void SzDuel_SetHandPhase(int side, int value);
extern void SzDuel_SetDestinyFlag(int side, signed char value);
extern void SzDuel_AddUnit(int side, int col, int row, int card, int flag);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern void *SzDuel_MoveUnit(int side, int id, int x, int y);
extern int SzDuel_SetUnitCurseRest(int side, int pos, signed char value);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int SzDuel_SetField(int x, int y, signed char field);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern void SzDuel_CloseHandCursor(void);
extern int SzDuel_GetMsgName(int cardNo);
extern void SzDuel_SetTurnCount(int count);
extern int SzData_SetRankDirect(int side, int cardNo, int rank);
extern void Ta_SetUnitCurseRest(int side, int id, int value);
extern void TaTuto_DbgInit034(int a);
extern void TaTuto_FocusUnit__00175ba0(int card);
extern void TaTuto_SetDeck035(int step);
extern void TaTuto_DrawFinger(struct DotrTutoScriptRecord *rec, int a, int b, int x, int y);
extern void TaTuto_DrawTrapArea(struct DotrTutoScriptRecord *rec, int a, int x, int y);
extern int TaTuto_ControlFade(int kind, int direction, int flag);
extern void TaTuto_RestoreOriginalView(void *area, int a);
extern void TaTuto_BkupOriginalView(void *area);
extern int TaTuto_OpenTutoMsg(int first, int last, struct DotrTutoScriptRecord *rec);

/* 0x175fe0 L034 and 0x178590 L035 (names from the Japanese build's symbols). Each returns -2
 * on pad button 0x800 (port 0), -1 for a step outside its script, a button mask the tutorial
 * wants pressed on some steps, else 0. L034 does not use its first parameter; L035 reads it on
 * its cursor steps (assumed: low byte the cursor column, next byte the row) and returns the
 * direction button that moves the cursor towards the wanted cell. */
int L034(int arg0, void *tutorial);
int L035(int arg0, void *tutorial);

#endif
