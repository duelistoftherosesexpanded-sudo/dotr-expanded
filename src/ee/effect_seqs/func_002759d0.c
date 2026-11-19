#include "dotr/ee/effect_seqs.h"

int Ta_Eff_071(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaUtil_DoOpen(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 1);
        TaView_ReqFocusCamera(0, eInfo->dwFrom, 100, 180);
        TaEffect_LoadModule(Temp, 602);
        if (eInfo->dwPar != 0) {
            TaEffect_LoadModule(Temp, 500);
        } else {
            TaEffect_LoadModule(Temp, 513);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (SzDuel_GetUnitOpenRate(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) == 255) {
            if (TaEffect_IsLoadModule(Temp) == 1) {
                SzDuel_OpenMsgPopup(0, -120, 80, 259);
                pCall->eNO = 602;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwFrom;
                pCall->dwPar = 0;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                Temp->wLvl++;
            }
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 0, 1);
        TaView_ReqFocusCamera(0, eInfo->dwTo, 100, 180);
        if (eInfo->dwPar != 0) {
            pCall->eNO = 500;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
        } else {
            pCall->eNO = 513;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
        }
        SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(SzDuel_GetUnitCardNo(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF)));
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->wLvl++;
        return 1;
    case 2:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        if (eInfo->dwPar != 0) {
            Ta_SetUnitCurseRest(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 255);
        } else {
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 3, 0);
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 5, 0);
        }
        Temp->wLvl++;
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
        break;
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
