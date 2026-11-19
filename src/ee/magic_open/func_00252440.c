#include "dotr/ee/magic_open.h"

/* Clears the effect record and the two pInfo bytes of pDat, then answers step PrNo of opening
 * (flipping face-up) the unit nSide/nID. Step 0: a kind-0 unit with a type-3 effect that is not
 * status 4 and is status 2 is opened and its effect run through Ta_Manage_ActEfc (result | 0x100);
 * kinds 1 and 5 need status 2 and return (PrNo + 1) << 16; anything else returns 0x100. Steps 1..6
 * look, in order, for the natural effects 14, 76 (effect type 20) and 17 (effect type 50), then
 * for the traps 14, 76 (type 20) and 17 (type 50) of the current side, each with or without trap
 * 46, and fill pDat->eInfo with the sequence to run; with none of them the unit opens with
 * sequence 101 and parameter 1 (0x70001). Any other step goes to Ta_Do_Magic.
 * Name, prototype, parameter, local and member names from the Japanese build's debug information
 * (Magic.c); the locals are declared in the reverse of that order. */
int Ta_Open(int nSide, int nID, struct jp_108cd *pDat, int PrNo)
{
    int Unit;
    int Unit2;
    int LOC;
    int bKind;
    struct jp_10845 *temp;
    struct jp_107e2 *pInfo;

    pInfo = &pDat->pInfo;
    bKind = SzDuel_GetUnitKind(nSide, nID) >> 5;
    pDat->eInfo.eNO = 0;
    pDat->eInfo.dwFrom = 0;
    pDat->eInfo.dwTo = 0;
    pDat->eInfo.dwPar = 0;
    pDat->pInfo.bType = 0;
    pDat->pInfo.bKind = 0;
    temp = Ta_GetUnitEffect(nSide, nID, 0);
    switch (PrNo) {
    case 0:
        if (bKind == 0) {
            if ((temp = Ta_GetUnitEffect(nSide, nID, 3)) != 0 && SzDuel_GetUnitStatus(nSide, nID, 4) == 0 &&
                SzDuel_GetUnitStatus(nSide, nID, 2) != 0) {
                LOC = SzDuel_GetUnitLoc(nSide, nID);
                TaUtil_DoOpen(nSide, nID, 1);
                return Ta_Manage_ActEfc(temp, nSide, nID, -1, -1, LOC & 0xFF, LOC >> 8, pDat, pInfo,
                    0) | 0x100;
            }
            return 0x100;
        }
        if (bKind != 1 && bKind != 5) {
            return 0x100;
        }
        if (bKind == 1 || bKind == 5) {
            if (SzDuel_GetUnitStatus(nSide, nID, 2) == 0) {
                return 0x100;
            }
        }
        return (PrNo + 1) << 16;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        Unit = Ta_SearchUncTypeNaturalEffectUser(14, 0);
        if (Unit >= 0) {
            switch (PrNo) {
            case 1:
                pDat->eInfo.eNO = 101;
                pDat->eInfo.dwFrom = 0;
                pDat->eInfo.dwTo = (nSide << 8) | nID;
                pDat->eInfo.dwPar = 0;
                return 0x20001;
            case 2:
                pDat->eInfo.eNO = 102;
                pDat->eInfo.dwFrom = 0;
                pDat->eInfo.dwTo = Unit;
                pDat->eInfo.dwPar = 0;
                return 0x30001;
            case 3:
                Ta_DelUnit(nSide, nID);
                return 0;
            }
            return 0;
        }
        Unit = Ta_SearchUncTypeNaturalEffectUser(76, 0);
        if (Unit >= 0 && (temp->dwID >> 8) == 20) {
            switch (PrNo) {
            case 1:
                pDat->eInfo.eNO = 101;
                pDat->eInfo.dwFrom = 0;
                pDat->eInfo.dwTo = (nSide << 8) | nID;
                pDat->eInfo.dwPar = 0;
                return 0x20001;
            case 2:
                pDat->eInfo.eNO = 102;
                pDat->eInfo.dwFrom = 0;
                pDat->eInfo.dwTo = Unit;
                pDat->eInfo.dwPar = 0;
                return 0x30001;
            case 3:
                Ta_DelUnit(nSide, nID);
                return 0;
            }
            return 0;
        }
        Unit = Ta_SearchUncTypeNaturalEffectUser(17, 0);
        if (Unit >= 0 && (temp->dwID >> 8) == 50) {
            switch (PrNo) {
            case 1:
                pDat->eInfo.eNO = 101;
                pDat->eInfo.dwFrom = 0;
                pDat->eInfo.dwTo = (nSide << 8) | nID;
                pDat->eInfo.dwPar = 0;
                return 0x20001;
            case 2:
                pDat->eInfo.eNO = 102;
                pDat->eInfo.dwFrom = 0;
                pDat->eInfo.dwTo = Unit;
                pDat->eInfo.dwPar = 0;
                return 0x30001;
            case 3:
                Ta_DelUnit(nSide, nID);
                return 0;
            }
            return 0;
        }
        if ((Unit = Ta_IsActiveTrapB(Ta_GetCurrentSide(nSide, nID), 14)) >= 0) {
            if ((Unit2 = Ta_IsActiveTrapB(0, 46)) < 0) {
                switch (PrNo) {
                case 1:
                    return (PrNo + 1) << 16;
                case 2:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 0x30001;
                case 3:
                    TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                    return 0x40000;
                case 4:
                    if (SzDuel_GetUnitOpenRate(Unit >> 8, Unit & 0xFF) == 255) {
                        pDat->eInfo.eNO = 51;
                        pDat->eInfo.dwFrom = Unit;
                        pDat->eInfo.dwTo = (nSide << 8) | nID;
                        pDat->eInfo.dwPar = 1;
                        return 1;
                    }
                    return 0x40000;
                }
                return 0;
            } else {
                switch (PrNo) {
                case 1:
                    return (PrNo + 1) << 16;
                case 2:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 0x30001;
                case 3:
                    TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                    return 0x40000;
                case 4:
                    if (SzDuel_GetUnitOpenRate(Unit2 >> 8, Unit2 & 0xFF) == 255) {
                        pDat->eInfo.eNO = 102;
                        pDat->eInfo.dwFrom = 1;
                        pDat->eInfo.dwTo = Unit2;
                        pDat->eInfo.dwPar = 0;
                        return 0x50001;
                    }
                    return 0x40000;
                case 5:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 2;
                    return 0x70001;
                }
                return 0;
            }
        }
        if ((Unit = Ta_IsActiveTrapB(Ta_GetCurrentSide(nSide, nID), 76)) >= 0 &&
            (temp->dwID >> 8) == 20) {
            if ((Unit2 = Ta_IsActiveTrapB(0, 46)) < 0) {
                switch (PrNo) {
                case 1:
                    return (PrNo + 1) << 16;
                case 2:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 0x30001;
                case 3:
                    TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                    return 0x40000;
                }
                if (SzDuel_GetUnitOpenRate(Unit >> 8, Unit & 0xFF) == 255) {
                    pDat->eInfo.eNO = 51;
                    pDat->eInfo.dwFrom = Unit;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 1;
                }
                return 0x40000;
            } else {
                switch (PrNo) {
                case 1:
                    return (PrNo + 1) << 16;
                case 2:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 0x30001;
                case 3:
                    TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                    return 0x40000;
                case 4:
                    if (SzDuel_GetUnitOpenRate(Unit2 >> 8, Unit2 & 0xFF) == 255) {
                        pDat->eInfo.eNO = 102;
                        pDat->eInfo.dwFrom = 1;
                        pDat->eInfo.dwTo = Unit2;
                        pDat->eInfo.dwPar = 0;
                        return 0x50001;
                    }
                    return 0x40000;
                case 5:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 2;
                    return 0x70001;
                }
                return 0;
            }
        }
        if ((Unit = Ta_IsActiveTrapB(Ta_GetCurrentSide(nSide, nID), 17)) >= 0 &&
            (temp->dwID >> 8) == 50) {
            if ((Unit2 = Ta_IsActiveTrapB(0, 46)) < 0) {
                switch (PrNo) {
                case 1:
                    return (PrNo + 1) << 16;
                case 2:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 0x30001;
                case 3:
                    TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                    return 0x40000;
                }
                if (SzDuel_GetUnitOpenRate(Unit >> 8, Unit & 0xFF) == 255) {
                    pDat->eInfo.eNO = 51;
                    pDat->eInfo.dwFrom = Unit;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 1;
                }
                return 0x40000;
            } else {
                switch (PrNo) {
                case 1:
                    return (PrNo + 1) << 16;
                case 2:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 0;
                    return 0x30001;
                case 3:
                    TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                    return 0x40000;
                case 4:
                    if (SzDuel_GetUnitOpenRate(Unit2 >> 8, Unit2 & 0xFF) == 255) {
                        pDat->eInfo.eNO = 102;
                        pDat->eInfo.dwFrom = 1;
                        pDat->eInfo.dwTo = Unit2;
                        pDat->eInfo.dwPar = 0;
                        return 0x50001;
                    }
                    return 0x40000;
                case 5:
                    pDat->eInfo.eNO = 101;
                    pDat->eInfo.dwFrom = 0;
                    pDat->eInfo.dwTo = (nSide << 8) | nID;
                    pDat->eInfo.dwPar = 2;
                    return 0x70001;
                }
                return 0;
            }
        }
        pDat->eInfo.eNO = 101;
        pDat->eInfo.dwFrom = 0;
        pDat->eInfo.dwTo = (nSide << 8) | nID;
        pDat->eInfo.dwPar = 1;
        return 0x70001;
    default:
        return Ta_Do_Magic(nSide, nID, &pDat->eInfo, pInfo, PrNo, temp);
    }
}
