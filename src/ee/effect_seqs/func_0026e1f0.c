#include "dotr/ee/effect_seqs.h"

int Ta_Eff_051(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int tCount;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaView_ReqFocusCamera(0, eInfo->dwFrom, 180, 180);
        TaEffect_LoadModule(Temp, 602);
        TaEffect_LoadModule(Temp, 51);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            SzDuel_OpenMsgPopup(0, -120, 80, 259);
            pCall->eNO = 602;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwFrom;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 0, 1);
            SzDuel_OpenMsgPopup(0, -120, 80,
                SzDuel_GetMsgName(SzDuel_GetUnitCardNo(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF)));
            pCall->eNO = 51;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwFrom;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    case 2:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0
            && SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2
            && Temp->ePos[Temp->oNum - 1] == 0) {
            TaView_ReqFocusCamera(0, eInfo->dwTo, 100, 180);
            pCall->eNO = 51;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->ePos[Temp->oNum - 1] = 1;
            Temp->wLvl++;
            Temp->oNum++;
        }
        return 1;
    case 3:
        TaView_ReqFocusCamera(16, eInfo->dwTo, 300, 300);
        Temp->wLvl++;
        break;
    case 4:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            if (eInfo->dwPar == 1) {
                Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
            }
            Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wLvl++;
            break;
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
