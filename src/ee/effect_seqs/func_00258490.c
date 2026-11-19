#include "dotr/ee/effect_seqs.h"

int Ta_Eff_006(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    unsigned char FinishCond = 0;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 6);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 6;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) != 1) {
            for (i = 0; i < 22; i++) {
                if ((eInfo->dwTo >> (21 - i) & 1) == 1) {
                    if (i < 11) {
                        Ta_DelUnit(0, i);
                    } else {
                        Ta_DelUnit(1, i - 11);
                    }
                }
            }
            Temp->wLvl++;
        }
        break;
    default:
        FinishCond = 1;
        break;
    }
    switch (Temp->bCamLvl) {
    case 0:
        TaView_ReqStdCompleteView(30);
        Temp->bCamLvl++;
        break;
    case 1:
        if (Temp->wCamTime++ >= 30) {
            Temp->bCamLvl++;
            Temp->wCamTime = 0;
        }
        break;
    case 2:
        if (!(FinishCond & 1)) {
            if (Temp->wCamTime++ == 0) {
                TaView_ReqRotCamera(60, 16383);
            } else if (Temp->wCamTime >= 60) {
                Temp->wCamTime = 0;
            }
        } else {
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
            Temp->bCamLvl++;
        }
        break;
    default:
        if (Temp->wCamTime++ >= 16) {
            FinishCond |= 2;
        }
        break;
    }
    if (FinishCond == 3) {
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
