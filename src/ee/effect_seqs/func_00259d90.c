#include "dotr/ee/effect_seqs.h"

int Ta_Eff_012(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int tCount;
    unsigned char FinishCond;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    FinishCond = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaView_ReqTopView();
        TaEffect_LoadModule(Temp, 512);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            SzDuel_OpenMsgPopup(0, -120, 80, 264);
            Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 512);
            Temp->wLvl++;
        }
        break;
    default:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if (Temp->ePos[i] >= 0) {
                        TaUtil_DoOpen(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, 1);
                        SzDuel_SetUnitStatus(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, 0, 1);
                        Ta_SetUnitCurseRest(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, eInfo->dwPar);
                        Temp->ePos[i] = -1;
                    }
                }
            } else {
                tCount++;
            }
        }
        SzDuel_CloseMsgPopup();
        if (tCount >= Temp->oNum) {
            FinishCond = 1;
        }
        break;
    }
    switch (Temp->bCamLvl) {
    case 0:
        if (Temp->wCamTime++ > 10) {
            TaView_ReqStdCompleteView(16);
            Temp->bCamLvl++;
            Temp->wCamTime = 0;
        }
        break;
    case 1:
        if (FinishCond & 1) {
            if (Temp->wCamTime++ > 16) {
                Temp->bCamLvl++;
                Temp->wCamTime = 0;
                TaView_RestoreOriginalView(Temp, 16);
            }
        }
        break;
    default:
        if (Temp->wCamTime++ > 16) {
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            FinishCond |= 2;
        }
        break;
    }
    if (FinishCond == 3) {
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
