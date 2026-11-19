#include "dotr/ee/effect_seqs.h"

int Ta_Eff_014(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int eObj;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 514);
        if (eInfo->dwPar != 0) {
            TaEffect_LoadModule(Temp, 611);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 514;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        if (eInfo->dwPar == 0) {
            if (SzDuel_GetHandCount(eInfo->dwTo) == 0) {
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
            if (Temp->wCount_1 < 150) {
                TaEff_PopHand(eInfo, Temp, 1, eInfo->dwTo, 1);
                Temp->wCount_1++;
                return 1;
            }
            if (TaEff_PopHand(eInfo, Temp, 1, eInfo->dwTo, 0) == 0) {
                SzDuel_CloseHandPopup();
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                return 0;
            }
        } else {
            if (SzDuel_GetHandCount(eInfo->dwTo == 0 ? 1 : 0) == 0) {
                Temp->wLvl++;
                return 1;
            }
            if (TaEff_PopHand(eInfo, Temp, 0, eInfo->dwTo == 0 ? 1 : 0, 1) == 0) {
                return 1;
            }
            pCall->eNO = 611;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
            Temp->ePos[0] = eInfo->dwTo;
        }
        break;
    case 2:
        if (SzEffect_IsUse(Temp->eID[0])) {
            if (Temp->wCount_1++ == 6) {
                SzDuel_CloseHandPopup();
                Temp->fObj = 0;
                Temp->wCount_2 = 0;
            }
            return 1;
        }
        Temp->wCount_1 = 0;
        if (SzDuel_GetHandCount(eInfo->dwTo) == 0) {
            Temp->wLvl = 16;
            return 1;
        }
        if (TaEff_PopHand(eInfo, Temp, 1, eInfo->dwTo, 1) == 0) {
            return 1;
        }
        pCall->eNO = 611;
        pCall->dwFrom = 0;
        pCall->dwTo = 0;
        pCall->dwPar = 0;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->ePos[0] = eInfo->dwTo;
        Temp->wLvl++;
        break;
    case 3:
        if (SzEffect_IsUse(Temp->eID[0])) {
            if (Temp->wCount_1++ == 6) {
                SzDuel_CloseHandPopup();
            }
            return 1;
        }
        Temp->wLvl++;
        break;
    default:
        for (i = 0; i < 5; i++) {
            eObj = SzDuel_InqHand(0, i) & 0xFFF;
            if (eObj >= 0 && eObj != 999) {
                SzDuel_DelHand(0, i);
                SzDuel_AddGrave(0, eObj);
            }
        }
        for (i = 0; i < 5; i++) {
            eObj = SzDuel_InqHand(1, i) & 0xFFF;
            if (eObj >= 0 && eObj != 999) {
                SzDuel_DelHand(1, i);
                SzDuel_AddGrave(1, eObj);
            }
        }
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
