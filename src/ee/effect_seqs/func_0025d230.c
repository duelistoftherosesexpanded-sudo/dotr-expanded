#include "dotr/ee/effect_seqs.h"

int Ta_Eff_029_CNG(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int dDP;
    int dAP;
    int i;
    int tCount = 0;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 128;
        if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
            TaView_ReqFocusCamera(16, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(16);
        }
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        TaEffect_LoadModule(Temp, 529);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(736));
                Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 529);
                Temp->wLvl++;
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
        SzDuel_CloseMsgPopup();
        for (i = 0; i < 22; i++) {
            if (eInfo->dwTo >> (21 - i) & 1) {
                if (i < 11) {
                    dAP = SzDuel_GetUnitDP(0, i, 0) - SzDuel_GetUnitAP(0, i, 0);
                    dDP = SzDuel_GetUnitAP(0, i, 0) - SzDuel_GetUnitDP(0, i, 0);
                    TaUtil_CalcUnitAP(0, i, 0, dAP);
                    TaUtil_CalcUnitDP(0, i, 0, dDP);
                } else {
                    dAP = SzDuel_GetUnitDP(1, i - 11, 0) - SzDuel_GetUnitAP(1, i - 11, 0);
                    dDP = SzDuel_GetUnitAP(1, i - 11, 0) - SzDuel_GetUnitDP(1, i - 11, 0);
                    TaUtil_CalcUnitAP(1, i - 11, 0, dAP);
                    TaUtil_CalcUnitDP(1, i - 11, 0, dDP);
                }
            }
        }
        Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wLvl++;
        }
        break;
    default:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            if (Temp->wCamTime++ > 16) {
                if (Ta_IsFinishFade() == 1) {
                    if (SzDuel_GetWindowDisp() != 0) {
                        SzDuel_SetWindowDraw(1);
                    }
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
            }
        }
        break;
    }
    return 1;
}
