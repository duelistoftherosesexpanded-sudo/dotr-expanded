#include "dotr/ee/slot_seeks.h"

int Ta_IsTrapBInDecideOperation(int nSide, int nID, int stDefense, struct jp_749a8 *eInfo) {
    int Unit;
    int Unit2;
    int Loc;
    int Self;
    int Loc2;
    int DoLD;

    DoLD = 0;
    Loc = SzDuel_GetUnitLoc(nSide, nID);
    Self = Ta_GetCurrentSide(nSide, nID);
    if (nID == 0) {
        return 0;
    }
    if (Ta_IsLDRangeAvilityWithLocation(nSide, nID, Loc & 0xff, Loc >> 8, 11)) {
        DoLD = 1;
    }
    if (Ta_IsResistAll(nSide, nID)) {
        return 0;
    }
    switch (stDefense) {
    case 0:
        if ((Unit = Ta_IsActiveTrapB(Self, 44)) >= 0) { /* 44: stop attack move */
            if (DoLD == 1) {
                Loc2 = SzDuel_GetUnitLoc(Self, 0);
                Ta_SetRangeVisual(Loc2 & 0xff, Loc2 >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                eInfo->eNO = 254;
                eInfo->dwFrom = 0;
                eInfo->dwTo = Self << 8;
                eInfo->dwPar = 16;
                return 1;
            }
            Unit2 = Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 46); /* 46: deny traps */
            if (Unit2 >= 0) {
                eInfo->eNO = 102;
                eInfo->dwFrom = 1;
                eInfo->dwTo = Unit2;
                eInfo->dwPar = 0;
                return 1;
            }
            eInfo->eNO = 70;
            eInfo->dwFrom = Unit;
            eInfo->dwTo = nSide << 8 | nID;
            eInfo->dwPar = 0;
            return 0x10001;
        }
        return 0;
    default:
        if ((Unit = Ta_IsActiveTrapB(Self, 74)) >= 0) { /* 74: Gorgon's eye */
            if (DoLD == 1) {
                Loc2 = SzDuel_GetUnitLoc(Self, 0);
                Ta_SetRangeVisual(Loc2 & 0xff, Loc2 >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                eInfo->eNO = 254;
                eInfo->dwFrom = 0;
                eInfo->dwTo = Self << 8;
                eInfo->dwPar = 16;
                return 1;
            }
            Unit2 = Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 46);
            if (Unit2 >= 0) {
                eInfo->eNO = 102;
                eInfo->dwFrom = 1;
                eInfo->dwTo = Unit2;
                eInfo->dwPar = 0;
                return 1;
            }
            eInfo->eNO = 71;
            eInfo->dwFrom = Unit;
            eInfo->dwTo = nSide << 8 | nID;
            eInfo->dwPar = 1;
            return 0x10001;
        }
        if ((Unit = Ta_IsActiveTrapB(Self, 35)) >= 0) { /* 35: shift to attack */
            if (DoLD == 1) {
                Loc2 = SzDuel_GetUnitLoc(Self, 0);
                Ta_SetRangeVisual(Loc2 & 0xff, Loc2 >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                eInfo->eNO = 254;
                eInfo->dwFrom = 0;
                eInfo->dwTo = Self << 8;
                eInfo->dwPar = 16;
                return 1;
            }
            Unit2 = Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 46);
            if (Unit2 >= 0) {
                eInfo->eNO = 102;
                eInfo->dwFrom = 1;
                eInfo->dwTo = Unit2;
                eInfo->dwPar = 0;
                return 1;
            }
            eInfo->eNO = 71;
            eInfo->dwFrom = Unit;
            eInfo->dwTo = nSide << 8 | nID;
            eInfo->dwPar = 0;
            return 1;
        }
        break;
    }
    return 0;
}
