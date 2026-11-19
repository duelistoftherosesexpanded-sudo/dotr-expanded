#include "dotr/ee/effect_seqs.h"

int Ta_Eff_067(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 513);
        if (eInfo->dwPar == 0) {
            Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
            if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
                TaView_ReqFocusCamera(30, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
            } else {
                TaView_ReqStdCompleteView(30);
            }
        } else {
            TaEffect_LoadModule(Temp, 610);
            TaEffect_LoadModule(Temp, 572);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            if (eInfo->dwPar == 0) {
                if (Temp->wCamTime++ >= 30) {
                    SzDuel_OpenMsgPopup(0, -120, 80, 275);
                    Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 513);
                    Temp->wLvl++;
                }
            } else {
                pCall->eNO = 513;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwFrom;
                pCall->dwPar = 0;
                Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                Temp->oNum++;
                Temp->wLvl++;
            }
        }
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i])) {
                return 1;
            }
        }
        if (eInfo->dwPar == 1) {
            if (SzDuel_GetUnitCtlRateM(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) !=
                SzDuel_GetUnitCtlGoalM(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF)) {
                return 1;
            }
            SzDuel_OpenMsgPopup(0, -120, 80, 274);
            pCall->eNO = 610;
            pCall->dwFrom = eInfo->dwFrom;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            Temp->wLvl++;
            return 1;
        } else {
            Temp->wLvl = 4;
            Temp->wCamTime = 0;
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
        }
        break;
    case 2:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
            return 1;
        }
        SzEffect_SetBusyFlag2(eInfo->dwFrom, 0);
        Ta_UnitFade(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, -1, 1, 1);
        SzDuel_SetUnitCtlGoalC(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 0);
        SzDuel_SetUnitCtlRateC(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, -1, 0);
        Ta_UnitFade(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, -1, 1, 1);
        Temp->wLvl++;
        break;
    case 3:
        if (Ta_IsFinishFade() != 1) {
            return 1;
        }
        SzDuel_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
        if (SzDuel_GetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1)) {
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1, 0);
        } else {
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1, 1);
        }
        if (SzDuel_GetUnitCurseRest(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) == 0) {
            if (SzDuel_GetUnitSpellRest(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) == 0) {
                SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0, 0);
            }
        }
        Temp->wCamTime = 0;
        TaView_RestoreOriginalView(Temp, 16);
        SzDuel_CloseMsgPopup();
        Temp->wLvl++;
        break;
    default:
        if (Temp->wCamTime++ > 16) {
            if (Ta_IsFinishFade() == 1) {
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
        }
        break;
    }
    return 1;
}
