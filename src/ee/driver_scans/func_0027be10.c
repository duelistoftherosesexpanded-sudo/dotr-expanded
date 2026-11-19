#include "dotr/ee/driver_scans.h"

int TaLogic_IsAttackedNow2(int nSide, int nID, int x, int y, unsigned char fView)
{
    unsigned short wEnUnit[22];
    unsigned short nOwnerID;
    unsigned int dwRev;
    int Loc2;
    int Loc;
    int i;

    if (SzDuel_GetUnit(nSide, nID) == 0) {
        return 0;
    }
    nOwnerID = nSide << 8 | nID;
    for (i = 0; i < 22; i++) {
        wEnUnit[i] = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(Ta_IsUnitControl(nSide, nID) == 0 ? 1 : 0, wEnUnit, 31, fView);
    Loc = y << 8 | x;
    for (i = 0; i < TaLogic_GetUnitCtlLstLen(wEnUnit); i++) {
        Loc2 = SzDuel_GetUnitLoc(wEnUnit[i] >> 8, wEnUnit[i] & 0xFF);
        dwRev = GetMoveRev(SzDuel_GetUnit(wEnUnit[i] >> 8, wEnUnit[i] & 0xFF), Loc2 & 0xFF, Loc2 >> 8);
        if ((unsigned int)((((Loc2 & 0xFF) < (Loc & 0xFF) ? (Loc & 0xFF) : (Loc2 & 0xFF))
                            - ((Loc & 0xFF) < (Loc2 & 0xFF) ? (Loc & 0xFF) : (Loc2 & 0xFF)))
                           + (((Loc2 >> 8) < (Loc >> 8) ? (Loc >> 8) : (Loc2 >> 8))
                              - ((Loc >> 8) < (Loc2 >> 8) ? (Loc >> 8) : (Loc2 >> 8))))
            <= (dwRev & 0xF)) {
            if (TaLogic_IsPassRoot(wEnUnit[i] >> 8, wEnUnit[i] & 0xFF, Loc2 & 0xFF, Loc2 >> 8, Loc & 0xFF, Loc >> 8, &nOwnerID) == 1
                && TaLogic_IsUnitWin(nOwnerID, wEnUnit[i], Loc & 0xFF, Loc >> 8) <= 0
                && SzDuel_GetUnitCurseRest(wEnUnit[i] >> 8, wEnUnit[i] & 0xFF) == 0
                && SzDuel_GetUnitSpellRest(wEnUnit[i] >> 8, wEnUnit[i] & 0xFF) == 0
                && TaLogic_IsSafeTrap(nSide << 8 | nID, x, y) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

