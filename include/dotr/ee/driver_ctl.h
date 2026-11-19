#ifndef DOTR_EE_DRIVER_CTL_H
#define DOTR_EE_DRIVER_CTL_H

/* driver.c controlled-unit list builders at 0x27c070 and 0x27c2c0: both walk SzDuel_GetUnit over
 * both sides and filter with Ta_IsUnitControl2, TaLogic_IsJudgeUnitDanger and
 * TaLogic_IsUnitCategory.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Names, prototypes and local names of the two functions come from the Japanese build's debug
 * information (driver.c). struct jp_9a533 is only passed on and stays opaque here. The callee
 * prototypes are assumed: argument counts and widths as these callers pass them. */
struct jp_9a533;
extern struct jp_9a533 *SzDuel_GetUnit(int side, int pos);
extern int Ta_IsUnitControl2(struct jp_9a533 *vInfo);
extern int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
extern int TaLogic_IsUnitCategory(int nSide, int nID, int sKind);
void TaLogic_GetUnitCtlLst(int nSide, unsigned short *wUnitLst, int sKind, unsigned char fView);
void TaLogic_GetUnitCtlLst2(int nSide, unsigned short *wUnitLst, int sKind, int sCondView, unsigned char fView);

#endif
