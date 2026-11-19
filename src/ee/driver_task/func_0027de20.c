#include "dotr/ee/driver_task.h"

/* Unit and Loc3 are the Japanese build's local names. */
int TaLogic_GetNearUnit(struct jp_9add8 *pTask, struct PieceTreetag *pTree, struct jp_9a533 *pVarInfo, int nX, int nZ, int eSide, unsigned char bDist, int WarnLvl, struct jp_9b50e *nUnitLst, unsigned short wUnitNum, unsigned short *wSize, unsigned short *wNodeCount, unsigned char bChase, unsigned char fView, int Powerful, unsigned short *wIgnoreLst)
{
    int Unit;
    int Loc3;

    TaLogic_GetNearUnitL2(pTree, pVarInfo, nX, nZ, eSide, bDist, fView, WarnLvl, nUnitLst, &wUnitNum, wSize, wNodeCount, wIgnoreLst);
    if ((Unit = TaLogic_GetNearUnitL3(pVarInfo, nX, nZ, nUnitLst, bChase, 30, 1, 2, fView, WarnLvl, wUnitNum)) >= 0 && TaUtil_GetUnitAP2(pVarInfo, nX, nZ) <= Powerful) {
        pTask->wPurpose = 0x2FF;
        pTask->dwGoal = (Unit << 16) | (pTask->dwGoal & 0xFFFF);
        return 1;
    }
    if ((Unit = TaLogic_GetNearUnitL3(pVarInfo, nX, nZ, nUnitLst, bChase, 29, 1, 2, fView, WarnLvl, wUnitNum)) >= 0) {
        Loc3 = SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF);
        pTask->dwGoal = (Unit << 16) | (pTask->dwGoal & 0xFFFF);
        if (TaLogic_IsUnitWin4(pVarInfo, Unit, WarnLvl, Loc3 & 0xFF, Loc3 >> 8, fView) > 0) {
            TaLogic_SetTaskAtkOrMove(pTask, Unit, fView);
            return 1;
        }
if (SzDuel_GetUnitCurseRest(Unit >> 8, Unit & 0xFF) < 2 && SzDuel_GetUnitSpellRest(Unit >> 8, Unit & 0xFF) == 0) { return TaLogic_IsUnitWin5(pVarInfo, Unit, WarnLvl, Loc3 & 0xFF, Loc3 >> 8, fView); }
    } else {
        if ((Unit = TaLogic_GetNearUnitL3(pVarInfo, nX, nZ, nUnitLst, bChase, 52, 0, 2, fView, WarnLvl, wUnitNum)) >= 0) {
            Loc3 = SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF);
            pTask->dwGoal = (Unit << 16) | (pTask->dwGoal & 0xFFFF);
            if (TaLogic_IsUnitWin4(pVarInfo, Unit, WarnLvl, Loc3 & 0xFF, Loc3 >> 8, fView) > 0) {
                TaLogic_SetTaskAtkOrMove(pTask, Unit, fView);
                return 1;
            }
            if (SzDuel_GetUnitCurseRest(Unit >> 8, Unit & 0xFF) < 2 && SzDuel_GetUnitSpellRest(Unit >> 8, Unit & 0xFF) == 0) {
                return TaLogic_IsUnitWin5(pVarInfo, Unit, WarnLvl, Loc3 & 0xFF, Loc3 >> 8, fView);
            }
        }
    }
    return -1;
}
