#ifndef DOTR_EE_DRIVER_TASK_H
#define DOTR_EE_DRIVER_TASK_H

/* driver.c functions that fill a jp_9add8 task from the near-unit list helpers
 * (TaLogic_GetNearUnit).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* TaLogic_GetNearUnit (0x27de20): prototype, the locals Unit and Loc3 and the structures
 * jp_9add8 and jp_9b50e from the Japanese build's debug information (driver.c). struct jp_9a533
 * and struct PieceTreetag are only passed on and stay opaque here. The callee prototypes are
 * assumed: argument counts and widths as this caller passes them. Do not include together
 * with driver_scans.h, driver_near.h or lower_rules.h, which declare the same structures. */
struct jp_9a533;
struct PieceTreetag;
struct jp_9b50e {
    unsigned short nUnitID;
    unsigned char bDist;
};
struct jp_9add8 {
    unsigned short wUnit;
    unsigned short wOrder;
    unsigned short wPurpose;
    unsigned int dwGoal;
    unsigned char bCond : 2;
};
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
extern struct PieceTreetag *TaLogic_GetNearUnitL2(struct PieceTreetag *pTree, struct jp_9a533 *pVarInfo, int nX, int nZ,
                                                  int nSide, unsigned char bChase, int fView, int bWarnLvl,
                                                  struct jp_9b50e *nUnitLst, unsigned short *wUnitNum,
                                                  unsigned short *wSize, unsigned short *wNodeCount,
                                                  unsigned short *wIgnoreLst);
extern int TaLogic_GetNearUnitL3(struct jp_9a533 *pVarInfo, int nX, int nZ, struct jp_9b50e *nUnitLst,
                                 unsigned char bChase, int sKey, unsigned char fOpen, unsigned char fDef,
                                 unsigned char fView, int bWarnLvl, unsigned short wUnitNum);
extern int TaUtil_GetUnitAP2(struct jp_9a533 *pVarInfo, int nX, int nZ);
extern int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView);
extern int TaLogic_IsUnitWin5(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView);
extern void TaLogic_SetTaskAtkOrMove(struct jp_9add8 *pTask, int nEnemyID, unsigned char fView);
/* TaLogic_CommandEncUnit (0x27ca50): prototype and local names from the Japanese build's debug
 * information (driver.c). Callee prototypes are assumed, as this caller passes them:
 * SzDuel_GetUnit's result is only tested and passed on; TaLogic_CommandGunnerUnit takes the
 * view flag unmasked (unsigned char assumed); TaLogic_GetUnitCtlLst2 as in driver_ctl.h. */
extern struct jp_9a533 *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
extern int Ta_IsUnitControl(int a, int b);
extern int Ta_GetCurrentSide(int side, int pos);
extern int TaEnc_GetEncID(int cardNumber);
extern int Do_ChkEncMatch(unsigned side, int pos, int cardNumber);
extern void TaUtil_GetUnitLst(int sideToCheck, int compMode, unsigned short *units);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern void TaLogic_InitIgnoreLst(unsigned short *wIgnoreLst);
extern int TaLogic_CommandGunnerUnit(struct jp_9add8 *pTask, unsigned char fView);
extern int TaLogic_GetUnitCtlLstLen(unsigned short *units);
extern void TaLogic_GetUnitCtlLst2(int nSide, unsigned short *wUnitLst, int sKind, int sCondView, unsigned char fView);
extern int TaLogic_GetMaxApUnit2(unsigned short *ownerIdArray, int team);
int TaLogic_CommandEncUnit(struct jp_9add8 *pTask, unsigned char bChaseRange, unsigned char bIsView, unsigned char WarLvl);
int TaLogic_GetNearUnit(struct jp_9add8 *pTask, struct PieceTreetag *pTree, struct jp_9a533 *pVarInfo, int nX, int nZ, int eSide, unsigned char bDist, int WarnLvl, struct jp_9b50e *nUnitLst, unsigned short wUnitNum, unsigned short *wSize, unsigned short *wNodeCount, unsigned char bChase, unsigned char fView, int Powerful, unsigned short *wIgnoreLst);

#endif
