#ifndef DOTR_EE_TUTO_L033_H
#define DOTR_EE_TUTO_L033_H

/* L033 (0x1747d0): a tutorial script that steps a 16-bit step word at +0x1a2 through duel set-up
 * calls and tutorial messages.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/ai-entries.md. */

/* The tutorial record as L033 uses it (the record TaTuto_OpenTutoMsg steps; cue_steps.h has
 * its own view): a frame counter at +0x162, the step at +0x1a2, a wait counter at +0x1a4 and
 * the view backup area from +0x1b0 passed to TaTuto_BkupOriginalView /
 * TaTuto_RestoreOriginalView. Assumed: field meanings from their use here; size unknown. */
struct DotrTutoL033Record {
    unsigned char pad0[0x162];
    unsigned short timer;
    unsigned char pad164[0x3e];
    unsigned short step;
    unsigned short wait;
    unsigned char pad1A6[0xa];
    unsigned char text[1];
};

/* Callees (names from the Japanese build's symbols). SzPad_SetAnalogInfo and SzDuel_SetUnitCtlGoal
 * take 0x80 as a full int here (a signed char parameter would pass -128). SzDuel_SetIgnoreJudge
 * takes a 64-bit value. Assumed: TaTuto_ControlFade (kind, direction, flag) and TaTuto_OpenTutoMsg
 * (first, last, record) return 1 when done. */
extern int SzDuel_GetWindowDisp(void);
extern void SzDuel_SetWindowDisp(int on);
extern void SzDuel_SetWindowDraw(int on);
extern void SzPad_SetAnalogInfo(int port, int axis, int value);
extern int SzPad_Check(int port, int kind, int mask);
extern void SzDuel_SetIgnoreJudge(long long on);
extern int SzEffect_IsModelStandby2(int model);
extern int SzDuel_SetPlayerCur(int side, int x, int y);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern void SzDuel_AddUnit(int side, int col, int row, int card, int flag);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern void *SzDuel_MoveUnit(int side, int id, int x, int y);
extern int SzDuel_SetUnitCtlGoal(int side, int id, int goal);
extern int SzDuel_SetUnitCtlRate(int side, int id, int a, int b);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int SzDuel_SetField(int x, int y, signed char field);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern void SzDuel_CloseHandCursor(void);
extern int SzDuel_GetMsgName(int cardNo);
extern void SzDuel_SetTurnCount(int count);
extern void TaView_ReqStdCompleteView(int a);
extern void TaView_ReqHalfFarView(int a);
extern void TaTuto_FocusUnit__001745f0(int card);
extern void TaTuto_DrawFinger(struct DotrTutoL033Record *rec, int a, int b, int x, int y);
extern int TaTuto_ControlFade(int kind, int direction, int flag);
extern void TaTuto_RestoreOriginalView(void *area, int a);
extern void TaTuto_BkupOriginalView(void *area);
extern int TaTuto_OpenTutoMsg(int first, int last, struct DotrTutoL033Record *rec);

/* 0x1747d0 L033 (name from the Japanese build's symbols). The first parameter is unused.
 * Returns -2 on pad button 0x800 (port 0), -1 past the last step, a button mask the
 * tutorial wants pressed on some steps (0x10, 0x20, 0x40, 0x80, 0x800, 0x1000, 0x4000),
 * else 0. */
int L033(int arg0, void *tutorial);

/* 0x1728a0 L032 (name from the Japanese build's symbols), the preceding tutorial script, with
 * the record as a byte pointer (it reads the same +0x162 / +0x1a2 / +0x1a4 halfwords through
 * raw offsets and passes +0x1b0 as the view backup area). Its own callees:
 * TaTuto_FocusUnit__00172510 (kind_sends; result unused here) and views of TaTuto_OpenTutoMsg
 * and TaTuto_DrawFinger taking that byte pointer. Assumed: as for L033. */
extern void TaTuto_FocusUnit__00172510(int card);
extern int func_0027fc20_l032(int first, int last, unsigned char *rec);
extern void func_0027f630_l032(unsigned char *rec, int a, int b, int x, int y);
int L032(int arg0, unsigned char *arg1);

/* 0x17aa70 L036 (name from the Japanese build's symbols), a later tutorial script on the same
 * record and callees as L033. The first parameter is unused. Returns -2 on pad button 0x800
 * (port 0), -1 past the last step, a button mask on some steps, else 0. */
int L036(int arg0, void *tutorial);

#endif
