#include "dotr/ee/driver_task.h"

/* Parameter and local names, their order and the structure layouts come from the
   Japanese build's debug information; its unused locals N2, C3, C2 and R3 are left out. */
int TaLogic_CommandEncUnit(struct jp_9add8 *pTask, unsigned char bChaseRange, unsigned char bIsView, unsigned char WarLvl)
{
    struct PieceTreetag *pTree;
    struct jp_9a533 *pVarInfo;
    unsigned short wIgnoreLst[49];
    struct jp_9b50e nUnitLst[22];
    unsigned short wUnitLst[22];
    unsigned short wSize;
    unsigned short wNodeCount;
    unsigned short wUnitNum;
    int N4;
    int i;
    int j;
    int Loc;
    int nSide;
    int Unit;
    int R2;
    int F;
    int nCardNo;
    int tempAP;
    int bKind;

    if (SzDuel_GetUnit(pTask->wUnit >> 8, pTask->wUnit & 0xFF) == 0) {
        return 0;
    }
    nCardNo = SzDuel_GetUnitCardNo(pTask->wUnit >> 8, pTask->wUnit & 0xFF);
    nSide = Ta_IsUnitControl(pTask->wUnit >> 8, pTask->wUnit & 0xFF);
    for (i = 0; i < 22; i++) {
        nUnitLst[i].nUnitID = 0xFFFF;
        nUnitLst[i].bDist = 0xFF;
        wUnitLst[i] = 0xFFFF;
    }
    TaLogic_InitIgnoreLst(wIgnoreLst);
    switch (TaEnc_GetEncID(nCardNo & 0xFFFF)) {
    case 3:
        if (TaLogic_CommandGunnerUnit(pTask, bIsView) == 1) {
            bKind = SzDuel_GetUnitKind(((pTask->dwGoal >> 16) & 0xFFFF) >> 8, (pTask->dwGoal >> 16) & 0xFF);
            if (bKind >> 5 == 0 && (bKind & 0x1F) != 14) {
                return 1;
            }
        }
        pTask->wPurpose = 0x5FF;
        return 1;
    case 2:
        Unit = -1;
        TaUtil_GetUnitLst(nSide, 1, wUnitLst);
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(wUnitLst); i++) {
            if (SzDuel_GetUnitCurseRest(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF) == 0xFF) {
                if (TaUtil_GetUnitAP(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF, SzDuel_GetUnitLoc(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF) & 0xFF, SzDuel_GetUnitLoc(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF) >> 8) > -1) {
                    Unit = wUnitLst[i];
                }
            }
        }
        if (Unit > 0) {
            /* MATCHING DEVICE: the shifted unit goes through j; written inline, the first
               reset loop below swaps its counter and pointer registers. */
            j = Unit << 16;
            pTask->wPurpose = 0x9FF;
            pTask->dwGoal = j | (pTask->dwGoal & 0xFFFF);
            return 1;
        }
        R2 = -1;
        for (j = 0; j < 22; j++) {
            wUnitLst[j] = 0xFFFF;
        }
        TaLogic_GetUnitCtlLst2(nSide == 0 ? 1 : 0, wUnitLst, 29, bIsView, bIsView);
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(wUnitLst); i++) {
            tempAP = SzDuel_GetUnitAP(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF, 0);
            if (tempAP - SzDuel_GetUnitAP(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF, 1) > -1) {
                R2 = wUnitLst[i];
            }
        }
        if (R2 > 0) {
            pTask->wPurpose = 0x9FF;
            pTask->dwGoal = (R2 << 16) | (pTask->dwGoal & 0xFFFF);
            return 1;
        }
        R2 = -1;
        for (j = 0; j < 22; j++) {
            wUnitLst[j] = 0xFFFF;
        }
        TaLogic_GetUnitCtlLst2(nSide, wUnitLst, 52, 1, 1);
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(wUnitLst); i++) {
            if (SzDuel_GetUnitCurseRest(wUnitLst[i] >> 8, wUnitLst[i] & 0xFF) > -1) {
                R2 = wUnitLst[i];
            }
        }
        if (R2 > 0) {
            pTask->wPurpose = 0x9FF;
            pTask->dwGoal = (R2 << 16) | (pTask->dwGoal & 0xFFFF);
            return 1;
        }
        pTask->wPurpose = 0x5FF;
        return 1;
    default:
        nSide = Ta_GetCurrentSide(pTask->wUnit >> 8, pTask->wUnit & 0xFF);
        Loc = SzDuel_GetUnitLoc(pTask->wUnit >> 8, pTask->wUnit & 0xFF);
        pVarInfo = SzDuel_GetUnit(pTask->wUnit >> 8, pTask->wUnit & 0xFF);
        wUnitNum = 0;
        wNodeCount = 0;
        wSize = 0;
        pTree = 0;
        for (i = 0; i < bChaseRange; i++) {
            for (N4 = 0; N4 < 22; N4++) {
                wUnitLst[N4] = 0xFFFF;
            }
            F = 0;
            pTree = TaLogic_GetNearUnitL2(pTree, pVarInfo, Loc & 0xFF, Loc >> 8, nSide, i, bIsView, WarLvl, nUnitLst, &wUnitNum, &wSize, &wNodeCount, wIgnoreLst);
            for (j = 0; j < 22; j++) {
                if (Do_ChkEncMatch(nUnitLst[j].nUnitID >> 8, nUnitLst[j].nUnitID & 0xFF, nCardNo) == 1) {
                    wUnitLst[F] = nUnitLst[j].nUnitID;
                    F++;
                }
            }
            Unit = TaLogic_GetMaxApUnit2(wUnitLst, nSide);
            if (Unit >= 0) {
                pTask->wPurpose = 0x9FF;
                pTask->dwGoal = (Unit << 16) | (pTask->dwGoal & 0xFFFF);
                return 1;
            }
        }
        pTask->wPurpose = 0x5FF;
        return 1;
    }
}
