#include "dotr/ee/effect_seqs.h"

int Ta_Eff_031(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int eID;
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
        TaEffect_LoadModule(Temp, 531);
        TaEffect_LoadModule(Temp, 532);
        if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
            TaView_ReqFocusCamera(0, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(0);
        }
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (!Temp->eMod.fLoad) {
                if (TaEffect_IsLoadModule(Temp) == 1) {
                    Temp->eMod.fLoad = -1;
                }
            } else {
                pCall->eNO = 531;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwFrom & 0xFFFF;
                pCall->dwPar = 0;
                eID = Ta_Do_Effect(pCall, Temp);
                if (eID >= 0) {
                    Temp->eID[Temp->oNum] = eID;
                    Temp->oNum++;
                    Temp->wLvl++;
                } else if (eID == -1 || eID == -2) {
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
            if (SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2) {
                if (eInfo->dwFrom >> 16 == 1) {
                    SzEffect_SetBusyFlag2(eInfo->dwFrom & 0xFFFF, 0);
                    SzEffect_SetAnimIntr2(eInfo->dwFrom & 0xFFFF, 0);
                    if (SzDuel_GetUnitCtlRateM((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF) == 0) {
                        SzDuel_SetUnitCtlGoalM((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 128);
                        SzDuel_SetUnitCtlRateM((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 1, 128);
                    }
                }
                SzDuel_MoveUnit((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, eInfo->dwPar & 0xFF, eInfo->dwPar >> 8);
                Temp->wCamTime = 0;
                Temp->wLvl++;
            }
        }
        break;
    case 2:
        if (Temp->wCamTime == 0) {
            if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
                TaView_ReqFocusCamera(30, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
            } else {
                TaView_ReqStdCompleteView(16);
            }
        }
        if (Temp->wCamTime++ >= 24) {
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 1) {
                pCall->eNO = 532;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwFrom & 0xFFFF;
                pCall->dwPar = 0;
                eID = Ta_Do_Effect(pCall, Temp);
                if (eID >= 0) {
                    Temp->eID[Temp->oNum] = eID;
                    Temp->oNum++;
                    Temp->wLvl++;
                } else if (eID == -1 || eID == -2) {
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
            }
        }
        break;
    case 3:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 1) {
            Temp->wLvl++;
            TaView_RestoreOriginalView(Temp, 16);
            Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
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
