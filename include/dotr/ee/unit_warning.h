#ifndef DOTR_EE_UNIT_WARNING_H
#define DOTR_EE_UNIT_WARNING_H

/* TaLogic_GetUnitWarning (0x276cc0, driver.c in the Japanese build): sums a danger value over the
 * opponent's controlled units.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The unit record (tag jp_9a533 in the Japanese build's debug information) is only
 * passed on here. */
struct jp_9a533;

/* Callees with the prototypes the Japanese debug information gives (driver_ctl.h,
 * driver_task.h carry the same); SzDuel_GetUnit and SzDuel_GetUnitLoc take the two
 * bytes of a packed unit word. */
extern struct jp_9a533 *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_IsUnitControl2(struct jp_9a533 *vInfo);
extern int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z,
                              unsigned char fView);
extern void TaLogic_GetUnitCtlLst(int nSide, unsigned short *wUnitLst, int sKind,
                                  unsigned char fView);
extern int TaLogic_GetUnitCtlLstLen(unsigned short *wUnitLst);

/* Prototype, parameter and local names from the Japanese build's debug information
 * (TaLogic_GetUnitWarning at 0x274de0 there, same size). */
int TaLogic_GetUnitWarning(int nX, int nZ, int nOwnerID, unsigned char bWarnRange,
                           unsigned short bWarnLvl, unsigned char fView);

#endif
