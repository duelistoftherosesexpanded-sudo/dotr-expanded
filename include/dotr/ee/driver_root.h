#ifndef DOTR_EE_DRIVER_ROOT_H
#define DOTR_EE_DRIVER_ROOT_H

/* driver.c root tests at 0x2798c0 and 0x279cc0: both decide whether a unit can pass from one cell
 * to a goal cell at most two cells away, with GetMoveRev, SzDuel_GetField and the map unit
 * queries.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */
/* Names, prototypes and local names of the two functions come from the Japanese build's debug
 * information (driver.c). struct jp_9a533 is only passed on and stays opaque here. The callee
 * prototypes are assumed: argument counts and widths as these callers pass them. */
struct jp_9a533;
extern unsigned int GetMoveRev(struct jp_9a533 *vInfo, int nX, int nZ);
extern int SzDuel_GetField(int x, int y);
extern int Ta_IsVirus_Chk3(struct jp_9a533 *vInfo, int x, int y);
extern void *SzDuel_GetMapUnit(int x, int y);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int Ta_IsUnitControl(int a, int b);
extern int Ta_IsUnitControl2(struct jp_9a533 *vInfo);
/* Assumed prototypes as TaLogic_IsPassInvadeRoot2 passes the arguments. */
extern int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
extern int TaLogic_IsUnitWin2(struct jp_9a533 *sourceUnit, int position, int col, int row);
extern int TaLogic_IsUnitWin3(struct jp_9a533 *unit, int base, int x, int y);

/* Assumed spelling of the cell distance: the original's macros are not in the debug information. */
#define DRIVER_ROOT_MIN(a, b) ((b) < (a) ? (b) : (a))
#define DRIVER_ROOT_MAX(a, b) ((a) < (b) ? (b) : (a))
#define DRIVER_ROOT_ABS(a) ((a) >= 0 ? (a) : -(a))
#define DRIVER_ROOT_DIST() \
    ((DRIVER_ROOT_MAX(gX, nX) - DRIVER_ROOT_MIN(gX, nX)) + (DRIVER_ROOT_MAX(gZ, nZ) - DRIVER_ROOT_MIN(gZ, nZ)))

int TaLogic_IsPassRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned short *gUnitID);
int TaLogic_IsRunPassRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned short *gUnitID,
                           unsigned char fFoot);
int TaLogic_IsPassInvadeRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, int *gUnitID, int fView,
                              unsigned char Node0, unsigned short bWarnLvl);

#endif
