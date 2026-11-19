#include "dotr/ee/driver_root.h"

int TaLogic_IsRunPassRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned short *gUnitID,
                           unsigned char fFoot)
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
        if (SzDuel_GetMapUnit(gX, gZ) != 0) {
            if (*gUnitID == SzDuel_GetMapUnitID(gX, gZ)) {
                return 1;
            }
            if (fFoot == 0) {
                return 0;
            }
            RootUnit = SzDuel_GetMapUnitID(gX, gZ);
            if (Ta_IsUnitControl(RootUnit >> 8, RootUnit & 0xFF) == Ta_IsUnitControl2(pVarInfo)) {
                return 1;
            }
            return 0;
        }
        return 1;
    }
    if (DRIVER_ROOT_DIST() == 2) {
        switch (DRIVER_ROOT_ABS(nX - gX)) {
        case 0: {
            int m = (nZ + gZ) >> 1;
            if (TaLogic_IsPassRoot2(pVarInfo, nX, nZ, gX, m, gUnitID) == 1) {
                return TaLogic_IsPassRoot2(pVarInfo, nX, m, gX, gZ, gUnitID);
            }
            break;
        }
        case 2: {
            int m = (nX + gX) >> 1;
            if (TaLogic_IsPassRoot2(pVarInfo, nX, nZ, m, gZ, gUnitID) == 1) {
                return TaLogic_IsPassRoot2(pVarInfo, m, nZ, gX, gZ, gUnitID);
            }
            break;
        }
        case 1:
            if (TaLogic_IsPassRoot2(pVarInfo, nX, nZ, gX, nZ, gUnitID) == 1 &&
                TaLogic_IsPassRoot2(pVarInfo, gX, nZ, gX, gZ, gUnitID) == 1) {
                return 1;
            }
            if (TaLogic_IsPassRoot2(pVarInfo, nX, nZ, nX, gZ, gUnitID) == 1 &&
                TaLogic_IsPassRoot2(pVarInfo, nX, gZ, gX, gZ, gUnitID) == 1) {
                return 1;
            }
            return 0;
        default:
            return 0;
        }
    }
    return 0;
}
