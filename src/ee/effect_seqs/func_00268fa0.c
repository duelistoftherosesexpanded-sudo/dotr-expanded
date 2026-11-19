#include "dotr/ee/effect_seqs.h"

int Ta_Eff_059(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
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
        TaView_ReqStdCompleteView(16);
        TaEffect_LoadModule(Temp, 508);
        TaEffect_LoadModule(Temp, 529);
        TaView_ReqStdCompleteView(16);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 508;
            pCall->dwFrom = 0;
            pCall->dwTo = 1;
            pCall->dwPar = Ta_GetLPRevByNaturalEffect(44, eInfo->dwPar & 0xFFFF);
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[0] = 0xFFFF;
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        SzDuel_CalcLP(eInfo->dwFrom, 1, Ta_GetLPRevByNaturalEffect(44, eInfo->dwPar & 0xFFFF));
        Temp->wLvl++;
        break;
    case 2:
        SzDuel_OpenMsgPopup(0, -120, 80, 262);
        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 529);
        Temp->wLvl++;
        break;
    case 3:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount < Temp->oNum) {
            return 1;
        }
        SzDuel_CloseMsgPopup();
        for (tCount = 0; tCount < 22; tCount++) {
            if ((eInfo->dwTo >> (21 - tCount)) & 1) {
                if (tCount < 11) {
                    TaUtil_CalcUnitDP(0, tCount, 0, eInfo->dwPar >> 16);
                    TaUtil_CalcUnitAP(0, tCount, 0, eInfo->dwPar >> 16);
                } else {
                    TaUtil_CalcUnitDP(1, tCount - 11, 0, eInfo->dwPar >> 16);
                    TaUtil_CalcUnitAP(1, tCount - 11, 0, eInfo->dwPar >> 16);
                }
            }
        }
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
