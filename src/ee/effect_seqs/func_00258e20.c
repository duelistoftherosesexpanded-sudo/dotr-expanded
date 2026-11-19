#include "dotr/ee/effect_seqs.h"

int Ta_Eff_008(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int Par;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int Unit;
    int Unit2;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (eInfo->dwPar >> 16 == 4) {
        Par = (eInfo->dwPar & 0xFFFF) - SzDuel_GetLP(eInfo->dwTo);
    } else {
        Par = Ta_GetLPRevByNaturalEffect(43, eInfo->dwPar & 0xFFFF);
    }
    if (Par < 0) {
        Par = 0;
    }
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 508);
        if ((Unit = Ta_IsActiveTrapB(eInfo->dwTo, 42)) < 0) {
            Temp->wLvl = 32;
        } else if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 46)) < 0) {
            Temp->wCount_2 = Unit;
            TaEffect_LoadModule(Temp, 602);
            TaEffect_LoadModule(Temp, 548);
            Temp->wLvl = 64;
        } else {
            TaEffect_LoadModule(Temp, 602);
            TaView_ReqFocusCamera(0, Unit2, 180, 180);
            Temp->wCount_2 = Unit2;
            TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
            Temp->wLvl = 16;
            Temp->wCount_1 = 1;
        }
        return 1;
    }
    if (TaEffect_IsLoadModule(Temp) == 1) {
        switch (Temp->wLvl) {
        case 0:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
                return 1;
            }
            SzDuel_CalcLP(eInfo->dwTo, 0, Par);
            SzDuel_CloseMsgPopup();
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        case 1:
            if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF) != 255) {
                if (SzDuel_GetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 4) == 0) {
                    TaUtil_DoOpen(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 1);
                }
                return 1;
            }
            TaView_ReqFocusCamera(0, Temp->wCount_2, 180, 180);
            SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(815));
            pCall->eNO = 602;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->wCount_2;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            Temp->wLvl++;
            return 1;
        case 2:
        case 4:
        case 17:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 0) {
                if (Temp->wLvl != 4) {
                    SzDuel_SetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 0, 1);
                }
                Temp->wLvl++;
            }
            break;
        case 3:
            SzDuel_OpenMsgPopup(0, -120, 80, 267);
            pCall->eNO = 548;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = Par;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            Temp->wLvl++;
            return 1;
        case 5:
            SzDuel_CloseMsgPopup();
            SzDuel_SetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 0, 1);
            SzDuel_CalcLP(eInfo->dwTo, 1, Par);
            Temp->wLvl++;
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
            break;
        case 6:
            if (Temp->wCamTime++ > 16) {
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        case 16:
            if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF) != 255) {
                if (SzDuel_GetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 4) == 0) {
                    TaUtil_DoOpen(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 1);
                }
                return 1;
            }
            SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
            pCall->eNO = 602;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->wCount_2;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            Temp->wLvl++;
            break;
        case 18:
            SzDuel_OpenMsgPopup(0, -120, 80, 266);
            pCall->eNO = 508;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = Par;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            Temp->wLvl = 0;
            return 1;
        case 32:
            pCall->eNO = 508;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = Par;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            SzDuel_OpenMsgPopup(0, -120, 80, 266);
            Temp->wLvl = 0;
            break;
        case 64:
            pCall->eNO = 508;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = Par | 0x10000;
            SzDuel_OpenMsgPopup(0, -120, 80, 266);
            Temp->wLvl++;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            break;
        case 65:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
                return 1;
            }
            Temp->wLvl = 1;
            break;
        default:
            return 1;
        }
    }
    return 1;
}
