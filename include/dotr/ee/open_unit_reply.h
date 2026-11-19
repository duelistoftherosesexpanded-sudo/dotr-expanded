#ifndef DOTR_EE_OPEN_UNIT_REPLY_H
#define DOTR_EE_OPEN_UNIT_REPLY_H

/* TaLogic_IsOpenUnit copy at 0x153ae0 (AI reply for a face-down unit: card-number and
 * effect-kind cases over unit lists and attack values).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callee prototypes as the other groups declare them (assumed; the
 * definitions of TaLogic_GetActionReaction, TaLogic_GetUnitCtlLst
 * and TaLogic_GetUnitCtlLst2 take an unsigned char where an int is shown). */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_IsUnitControl(int a, int b);
extern unsigned *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int TaLogic_GetActionReaction(int, int, int, int, int, int);
extern void TaLogic_GetUnitCtlLst2(int, unsigned short *words, int, int, int);
extern void TaLogic_GetUnitCtlLst(int, unsigned short *words, int, int);
extern void TaUtil_GetUnitLst(int sideToCheck, int compMode, unsigned short *units);
extern int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);

/* 0x153ae0: reply 0, 1 or 2 for the unit at (x, y). Parameter meaning and int
 * types assumed; the name is the Japanese build's TaLogic_IsOpenUnit, which
 * several functions of this executable share (hence the address suffix). */
int TaLogic_IsOpenUnit__00153ae0(int x, int y);

#endif
