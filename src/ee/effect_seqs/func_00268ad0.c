#include "dotr/ee/effect_seqs.h"

int Ta_Eff_058(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6a917 *pVarInfo;
    struct jp_6b127 *pInfo;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int j;
    int cNum;
    int LOC;
    int MaxAP;
    int MaxNo;
    int MaxSide;
    int MaxEntry;
    int cNO;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    j = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaView_ReqStdCompleteView(16);
        TaEffect_LoadModule(Temp, 508);
        TaEffect_LoadModule(Temp, 501);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 508;
            pCall->dwFrom = 0;
            pCall->dwTo = 1;
            pCall->dwPar = eInfo->dwFrom;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[0] = 0xFFFF;
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0])) {
            return 1;
        }
        SzDuel_CalcLP(eInfo->dwTo, 1, eInfo->dwFrom);
        Temp->wLvl++;
        break;
    case 2:
        for (; (unsigned)j < eInfo->dwPar; j++) {
            LOC = Ta_GetSummonablePieceWithOrder(eInfo->dwTo, j, 1);
            pCall->eNO = 501;
            pCall->dwFrom = 0;
            pCall->dwTo = LOC;
            pCall->dwPar = 0;
            Temp->eID[j] = Ta_Do_Effect(pCall, Temp);
            if (LOC < 0) {
                Temp->ePos[j] = 0xFFFF;
            } else {
                Temp->ePos[j] = LOC;
            }
            Temp->oNum++;
        }
        Temp->wLvl++;
        break;
    case 3:
        /* MATCHING DEVICE: this loop counts in LOC; the debug information also lists a local i, which as
         * the index swaps the stack slots of the two spilled values. */
        for (LOC = 0; LOC < Temp->oNum; LOC++) {
            if (SzEffect_IsUse(Temp->eID[LOC])) {
                if (SzEffect_GetStatus(Temp->eID[LOC]) == 2) {
                    if ((unsigned short)Temp->ePos[LOC] != 0xFFFF) {
                        MaxNo = 999;
                        MaxEntry = -1;
                        MaxAP = 0;
                        cNum = SzDuel_GetGraveCount(eInfo->dwTo);
                        for (j = 0; j < cNum; j++) {
                            cNO = SzDuel_InqGrave(eInfo->dwTo, j);
                            if (cNO != 999) {
                                pInfo = SzDuel_GetCardConstInfo(cNO);
                                if (MaxAP <= pInfo->wAP && (pInfo->bKind >> 5) == 0) {
                                    MaxSide = eInfo->dwTo;
                                    MaxNo = cNO;
                                    MaxAP = pInfo->wAP;
                                    MaxEntry = j;
                                }
                            }
                        }
                        j = SzDuel_GetGraveCount(eInfo->dwTo == 0 ? 1 : 0);
                        for (cNum = 0; cNum < j; cNum++) {
                            cNO = SzDuel_InqGrave(eInfo->dwTo == 0 ? 1 : 0, cNum);
                            if (cNO != 999) {
                                pInfo = SzDuel_GetCardConstInfo(cNO);
                                if (pInfo->wAP > MaxAP && (pInfo->bKind >> 5) == 0) {
                                    MaxSide = eInfo->dwTo == 0 ? 1 : 0;
                                    MaxNo = cNO;
                                    MaxAP = pInfo->wAP;
                                    MaxEntry = cNum;
                                }
                            }
                        }
                        if (MaxEntry >= 0) {
                            pVarInfo = SzDuel_AddUnit(eInfo->dwTo, Temp->ePos[LOC] & 0xFF, Temp->ePos[LOC] >> 8, MaxNo, 0);
                            if (pVarInfo != 0) {
                                pVarInfo->fFinish = -1;
                            }
                            SzDuel_DelGrave(MaxSide, MaxEntry);
                            SzDuel_PackGrave(MaxSide);
                            Temp->ePos[LOC] = 0xFFFF;
                        }
                    }
                }
            } else {
                j++;
            }
        }
        if (j >= Temp->oNum) {
            if (Temp->wCamTime++ > 30) {
                Temp->wCamTime = 0;
                Temp->wLvl++;
                TaView_RestoreOriginalView(Temp, 16);
                break;
            }
        }
        return 1;
    default:
        if (Temp->wCamTime++ > 16) {
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    }
    return 1;
}

