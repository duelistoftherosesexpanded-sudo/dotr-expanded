#include "dotr/ee/move_efc.h"

/* Larger and smaller of two ints, spelled as the comparisons the code shows. */
#define MAXI(a, b) ((a) < (b) ? (b) : (a))
#define MINI(a, b) ((b) < (a) ? (b) : (a))
int Ta_Do_MoveEfc(int nSide, int nID, int nX, int nZ, struct jp_9026 *eInfo)
{
    int i;
    int j;
    int eObj;
    int eObj2;
    int eObj3;
    int Unit;
    struct jp_8cf7 *pDat;
    int bKind;

    if (nID == 0) {
        if (Ta_IsLDAbl(7, nSide)) {
            SzDuel_SetField(nX, nZ, Ta_GetLDChgLand(SzDuel_GetUnitKind(nSide, nID) & 0x1F));
        }
        i = 0;
        eObj = 0;
        eObj2 = 0;
        eObj3 = 0;
        for (; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                Unit = SzDuel_GetMapUnitID(i, j);
                if (Unit < 0) {
                    continue;
                }
                if (nSide == Ta_GetCurrentSide(Unit >> 8, Unit & 0xFF)) {
                    continue;
                }
                if ((Unit & 0xFF) == 0) {
                    continue;
                }
                if (!Ta_IsSquareRange(nX, nZ, i, j, Ta_GetLDRange(nSide))) {
                    continue;
                }
                bKind = SzDuel_GetUnitKind(Unit >> 8, Unit & 0xFF);
                if ((bKind >> 5) == 0) {
                    if (Ta_IsLDAbl(16, nSide)) {
                        if (Ta_IsLD_DestObj(nSide, bKind & 0x1F) && (Unit & 0xFF) != 0) {
                            eObj = ConvUnitIDtoFlagList(eObj, Unit >> 8, Unit & 0xFF);
                        }
                    }
                    if (Ta_IsLDAbl(15, nSide)
                        && Ta_IsLD_CurseObj(nSide, SzDuel_GetUnitKind(Unit >> 8, Unit & 0xFF) & 0x1F)
                        && SzDuel_GetUnitSpellRest(Unit >> 8, Unit & 0xFF) == 0 && (Unit & 0xFF) != 0) {
                        eObj2 = ConvUnitIDtoFlagList(eObj2, Unit >> 8, Unit & 0xFF);
                    }
                }
                if (Ta_IsLDAbl(13, nSide) && SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xFF, 4) == 0
                    && (Unit & 0xFF) != 0) {
                    eObj3 = ConvUnitIDtoFlagList(eObj3, Unit >> 8, Unit & 0xFF);
                }
            }
        }
        if (eObj > 0 || eObj2 > 0 || eObj3 > 0) {
            eInfo->eNO = 0x71;
            eInfo->dwFrom = eObj | 0x80000000;
            eInfo->dwTo = eObj2;
            eInfo->dwPar = eObj3;
            return 1;
        }
    } else if ((pDat = Ta_GetUnitEffect(nSide, nID, 1)) != 0) {
        switch (pDat->dwID >> 8) {
        case 0x42:
            switch ((pDat->dwID >> 6) & 3) {
            case 1:
                SzDuel_CalcLP(Ta_GetCurrentSide(nSide, nID), 1,
                              Ta_GetLPRevByNaturalEffect(0x2C, pDat->dwParam & 0xFFFF));
                break;
            case 2:
                SzDuel_CalcLP(Ta_GetCurrentSide(nSide, nID) == 0 ? 1 : 0, 1,
                              Ta_GetLPRevByNaturalEffect(0x2C, pDat->dwParam & 0xFFFF));
                break;
            }
            eInfo->eNO = 0xFF;
            eInfo->dwFrom = 0;
            eInfo->dwTo = (nSide << 8) | nID;
            eInfo->dwPar = 0;
            return 1;
        case 2:
            for (i = 0; i < 7; i++) {
                for (j = 0; j < 7; j++) {
                    if ((unsigned)((MAXI(i, nX) - MINI(i, nX)) + (MAXI(j, nZ) - MINI(j, nZ))) <= (pDat->dwParam >> 16)) {
                        SzDuel_SetField(i, j, pDat->dwParam & 0xFFFF);
                    }
                }
            }
            eInfo->eNO = 0xFF;
            eInfo->dwFrom = 0;
            eInfo->dwTo = (nSide << 8) | nID;
            eInfo->dwPar = 0;
            return 1;
        case 10:
            if (SzDuel_GetField(nX, nZ) == 8) {
                SzDuel_SetField(nX, nZ, 7);
                eInfo->eNO = 0xFF;
                eInfo->dwFrom = 0;
                eInfo->dwTo = (nSide << 8) | nID;
                eInfo->dwPar = 0;
                return 1;
            }
            return 0;
        case 9:
            if (SzDuel_GetField(nX, nZ) == 8) {
                eObj = ConvUnitIDtoFlagList(0, nSide, nID);
                eInfo->eNO = 0x3C;
                eInfo->dwFrom = 0;
                eInfo->dwTo = eObj;
                eInfo->dwPar = WallFusion(nSide, nID);
                return 0x10001;
            }
            break;
        }
    }
    return 0;
}
