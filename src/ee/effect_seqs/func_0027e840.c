#include "dotr/ee/effect_seqs.h"

int Ta_Eff_114(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 128;
        TaEffect_LoadModule(Temp, 625);
        SzDuel_CloseHandPopup();
        TaView_SetDestinyCamera(0);
        SzEffect_SetBusyFlag2(eInfo->dwFrom << 8, 1);
        return 1;
    }
    SzDuel_SetWindowDraw(0);
    switch (Temp->wLvl) {
    case 0:
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, -1)) > 0) {
            SzEffect_SetBlackOutForBattle(*(unsigned char *)&Temp->wCount_1);
            SzEffect_SetBlackOutForDuel(*(unsigned char *)&Temp->wCount_1);
        } else {
            SzEffect_SetBlackOutForBattle(*(unsigned char *)&Temp->wCount_1);
            SzEffect_SetBlackOutForDuel(*(unsigned char *)&Temp->wCount_1);
            if (TaEffect_IsLoadModule(Temp) == 1) {
                Temp->wLvl++;
            }
        }
        break;
    case 1:
        SzDuel_OpenMsgPopup(0, -120, 80, 277);
        pCall->eNO = 625;
        pCall->dwFrom = 0;
        pCall->dwTo = SzDuel_GetUnitLoc(eInfo->dwFrom, 0);
        pCall->dwPar = 0;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->wLvl++;
        break;
    case 2:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            if (Temp->wCamTime == 0) {
                SzSound_PlaySE(0, 34);
            }
            if (Temp->wCamTime++ == 19) {
                TaView_ReqBackFocusCamera(0, eInfo->dwFrom << 8, 300, 300);
                SzSound_PlaySE(0, 37);
                SzSound_PlaySE(0, 36);
            }
            return 1;
        }
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wCamTime = 0;
        Temp->wCount_1 = 0;
        Temp->wLvl++;
        break;
    default:
        Temp->wCamTime++;
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, 1)) < 128) {
            SzEffect_SetBlackOutForBattle(*(unsigned char *)&Temp->wCount_1);
            SzEffect_SetBlackOutForDuel(*(unsigned char *)&Temp->wCount_1);
        } else if (Temp->wCamTime > 16) {
            SzEffect_SetBusyFlag2(eInfo->dwFrom << 8, 0);
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
