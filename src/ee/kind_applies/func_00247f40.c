#include "dotr/ee/kind_applies.h"

int SelfHeap(int nSide_1, int ID_1, int nSide_2, int ID_2, struct jp_1e41 *eInfo) {
    int eNO;
    int FusRes;
    int KIND_2;
    int KIND_1;

    if (ID_2 == 0) {
        eInfo->eNO = 95;
        eInfo->dwFrom = (nSide_1 << 8) | ID_1;
        eInfo->dwTo = (nSide_2 << 8) | ID_2;
        eInfo->dwPar = 0;
        return 1;
    }
    KIND_1 = SzDuel_GetUnitKind(nSide_1, ID_1) >> 5;
    KIND_2 = SzDuel_GetUnitKind(nSide_2, ID_2) >> 5;
    if (KIND_1 == 0 && KIND_2 == 0) {
        if (ID_1 == 0) {
            eInfo->eNO = 95;
            eInfo->dwFrom = (nSide_2 << 8) | ID_2;
            eInfo->dwTo = (nSide_1 << 8) | ID_1;
            eInfo->dwPar = 1;
            return 1;
        }
        FusRes = Fusion(SzDuel_GetUnitCardNo(nSide_1, ID_1), SzDuel_GetUnitCardNo(nSide_2, ID_2));
        eInfo->eNO = 100;
        eInfo->dwFrom = (nSide_1 << 8) | ID_1;
        eInfo->dwTo = (nSide_2 << 8) | ID_2;
        eInfo->dwPar = FusRes;
        return 1;
    } else if (KIND_1 == 0 && KIND_2 == 2) {
        if (ID_1 == 0) {
            eInfo->eNO = 95;
            eInfo->dwFrom = (nSide_2 << 8) | ID_2;
            eInfo->dwTo = (nSide_1 << 8) | ID_1;
            eInfo->dwPar = 1;
            return 1;
        }
        eNO = Chk_Enc(nSide_1, ID_1, SzDuel_GetUnitCardNo(nSide_2, ID_2));
        if (eNO == 999) {
            eInfo->eNO = 36;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = 999;
            return 1;
        }
        if (((eNO >> 30) & 1) == 1) {
            eInfo->eNO = 43;
            eInfo->dwFrom = (nSide_2 << 8) | ID_2;
            eInfo->dwTo = (nSide_1 << 8) | ID_1;
            eInfo->dwPar = (eNO >> 14) & 0x3fff;
            return 0x10001;
        }
        if ((eNO >> 14) == 537) {
            eInfo->eNO = 37;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = (eNO & 0x3fff) | (Ta_GetCurrentSide(nSide_1, ID_1) << 16);
        } else {
            eInfo->eNO = 36;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = eNO;
        }
        return 0x10001;
    } else if (KIND_1 == 2 && KIND_2 == 0) {
        if (ID_2 == 0) {
            eInfo->eNO = 95;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = 0;
            return 1;
        }
        eNO = Chk_Enc(nSide_2, ID_2, SzDuel_GetUnitCardNo(nSide_1, ID_1));
        if (eNO == 999) {
            eInfo->eNO = 36;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = 999;
            return 1;
        }
        if (((eNO >> 30) & 1) == 1) {
            eInfo->eNO = 43;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = (eNO >> 14) & 0x3fff;
            return 1;
        }
        if ((eNO >> 14) == 537) {
            eInfo->eNO = 37;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = (eNO & 0x3fff) | (Ta_GetCurrentSide(nSide_2, ID_2) << 16);
        } else {
            eInfo->eNO = 36;
            eInfo->dwFrom = (nSide_1 << 8) | ID_1;
            eInfo->dwTo = (nSide_2 << 8) | ID_2;
            eInfo->dwPar = eNO;
        }
        return 1;
    }
    eInfo->eNO = 95;
    eInfo->dwFrom = (nSide_1 << 8) | ID_1;
    eInfo->dwTo = (nSide_2 << 8) | ID_2;
    eInfo->dwPar = 0;
    return 1;
}
