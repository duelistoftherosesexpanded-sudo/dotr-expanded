#include "dotr/ee/effect_seqs.h"

int Ta_Eff_045(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
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
        TaUtil_DoOpen(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1);
        TaView_ReqFocusCamera(0, eInfo->dwFrom, 100, 180);
        TaEffect_LoadModule(Temp, 545);
        TaEffect_LoadModule(Temp, 601);
        TaEffect_LoadModule(Temp, 602);
        TaEffect_LoadModule(Temp, 603);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (SzDuel_GetUnitOpenRate(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) < 255
            || SzDuel_GetUnitOpenRate(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) < 255
            || SzDuel_GetUnitCtlRateC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) < 128) {
            return 1;
        }
        if ((SzDuel_GetUnitKind(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) >> 5) == 0) {
            if (SzEffect_IsModelStandby2(eInfo->dwTo) == 0) {
                return 1;
            }
        }
        if (TaEffect_IsLoadModule(Temp) == 1) {
            Temp->wLvl++;
        }
        return 1;
    case 1:
        SzDuel_OpenMsgPopup(0, -120, 80, 259);
        pCall->eNO = 602;
        pCall->dwFrom = 0;
        pCall->dwTo = eInfo->dwFrom;
        pCall->dwPar = 0;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->wLvl++;
        return 1;
    case 2:
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 0, 1);
            pCall->eNO = 601;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwFrom;
            pCall->dwPar = 1;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
        }
        return 1;
    case 3:
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            TaView_ReqFocusCamera(16, eInfo->dwTo, 100, 180);
            Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
            pCall->eNO = 603;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            pCall->eNO = 545;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[1] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
        }
        return 1;
    case 4:
        if (SzEffect_IsUse(Temp->eID[0]) != 0 || SzEffect_IsUse(Temp->eID[1]) != 0) {
            return 1;
        }
        Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wLvl++;
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
