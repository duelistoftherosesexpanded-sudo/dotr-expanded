#ifndef DOTR_EE_DRIVER_NEAR_H
#define DOTR_EE_DRIVER_NEAR_H

/* driver.c list scans that pick the nearest unit of a jp_9b50e list by cell distance
 * (TaLogic_GetNearUnitL3, TaLogic_GetTrapNearCard).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* TaLogic_GetNearUnitL3 (0x277160) and TaLogic_GetTrapNearCard (0x277cd0): prototypes, struct
 * jp_9b50e and the local names v1, v2, Dist, Loc and i from the Japanese build's debug information
 * (driver.c). struct jp_9a533 is only passed on and stays opaque here. The callee prototypes are
 * assumed: argument counts and widths as these callers pass them. Do not include together
 * with driver_scans.h, which declares struct jp_9b50e as well. */
struct jp_9a533;
struct jp_9b50e {
    unsigned short nUnitID;
    unsigned char bDist;
};
extern void *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int TaLogic_IsStatusMatch(int actualStatus, int targetStatus);
extern int TaLogic_IsUnitCategory(int nSide, int nID, int sKind);
extern int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView);
extern int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
extern int TaTrapA_IsTrapAct(int x, int y, int a, int b, int c, int d);
extern int TaLogic_IsUnitWin2(void *sourceUnit, int position, int col, int row);
int TaLogic_GetNearUnitL3(struct jp_9a533 *pVarInfo, int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase,
                          int sKey, unsigned char fOpen, unsigned char fDef, unsigned char fView, int bWarnLvl,
                          unsigned short wUnitNum);
int TaLogic_GetTrapNearCard(int tSide, int tID, int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase,
                            int sKey, unsigned char fOpen, unsigned char fDef, unsigned char fView, int bWarnLvl,
                            unsigned short wUnitNum, unsigned char fTrap);

#endif
