#include "dotr/ee/driver_root.h"

int TaLogic_IsPassInvadeRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, int *gUnitID, int fView,
                              unsigned char Node0, unsigned short bWarnLvl)
{
    unsigned int dwMoveRev;
    int RootUnit;

    dwMoveRev = GetMoveRev(pVarInfo, nX, nZ);
    if (pVarInfo == 0) {
        return 0;
    }
    if (DRIVER_ROOT_DIST() == 0) {
        return 1;
    }
    if (DRIVER_ROOT_DIST() == 1) {
        /* Field 8 is labyrinth (passable only with move flag 0x10), 9 is crush. */
        if ((SzDuel_GetField(gX, gZ) == 8 && !(dwMoveRev & 0x10)) ||
            (SzDuel_GetField(gX, gZ) == 9 && Ta_IsVirus_Chk3(pVarInfo, gX, gZ) == 1)) {
            return 0;
        }
        if (SzDuel_GetMapUnit(gX, gZ) == 0) {
            return 1;
        }
        RootUnit = SzDuel_GetMapUnitID(gX, gZ);
        if (gUnitID != 0 && RootUnit == *gUnitID) {
            return 1;
        }
        if (Ta_IsUnitControl(RootUnit >> 8, RootUnit & 0xFF) != Ta_IsUnitControl2(pVarInfo)) {
            if (TaLogic_IsJudgeUnitDanger(RootUnit >> 8, RootUnit & 0xFF, fView)
                    ? TaLogic_IsUnitWin2(pVarInfo, RootUnit, gX, gZ) > 0
                    : TaLogic_IsUnitWin3(pVarInfo, bWarnLvl, gX, gZ) > 0) {
                return 1;
            }
            return 2;
        }
        if ((RootUnit & 0xFF) != 0) {
            if (Node0 == 1) {
                return 0;
            }
            return 1;
        }
        return 0;
    }
    if (DRIVER_ROOT_DIST() == 2) {
        switch (DRIVER_ROOT_ABS(nX - gX)) {
        case 0: {
            int m = (nZ + gZ) >> 1;
            if (TaLogic_IsPassInvadeRoot2(pVarInfo, nX, nZ, gX, m, gUnitID, fView, Node0, bWarnLvl) == 1) {
                return TaLogic_IsPassInvadeRoot2(pVarInfo, nX, m, gX, gZ, gUnitID, fView, Node0, bWarnLvl);
            }
            break;
        }
        case 2: {
            int m = (nX + gX) >> 1;
            if (TaLogic_IsPassInvadeRoot2(pVarInfo, nX, nZ, m, gZ, gUnitID, fView, Node0, bWarnLvl) == 1) {
                return TaLogic_IsPassInvadeRoot2(pVarInfo, m, nZ, gX, gZ, gUnitID, fView, Node0, bWarnLvl);
            }
            break;
        }
        case 1:
            if (TaLogic_IsPassInvadeRoot2(pVarInfo, nX, nZ, gX, nZ, gUnitID, fView, Node0, bWarnLvl) == 1 &&
                TaLogic_IsPassInvadeRoot2(pVarInfo, gX, nZ, gX, gZ, gUnitID, fView, Node0, bWarnLvl) == 1) {
                return 1;
            }
            if (TaLogic_IsPassInvadeRoot2(pVarInfo, nX, nZ, nX, gZ, gUnitID, fView, Node0, bWarnLvl) == 1 &&
                TaLogic_IsPassInvadeRoot2(pVarInfo, nX, gZ, gX, gZ, gUnitID, fView, Node0, bWarnLvl) == 1) {
                return 1;
            }
            return 0;
        default:
            return 0;
        }
    }
    return 0;
}
