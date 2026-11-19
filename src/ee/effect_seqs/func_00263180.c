#include "dotr/ee/effect_seqs.h"

int Ta_Eff_064(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int Loc2;
    int Loc;
    int AP;
    int DP;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaView_ReqFocusCamera(30, eInfo->dwTo, 100, 180);
        TaEffect_LoadModule(Temp, 529);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            if (Temp->wCamTime++ >= 30) {
                SzDuel_OpenMsgPopup(0, -120, 80, 271);
                pCall->eNO = 529;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwTo;
                pCall->dwPar = 0;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                Temp->ePos[0] = eInfo->dwTo;
                Temp->wLvl++;
            }
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        Loc = SzDuel_GetUnitLoc(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
        for (i = 0; i < 22; i++) {
            if (eInfo->dwFrom >> (21 - i) & 1) {
                if (i < 11) {
                    Loc2 = SzDuel_GetUnitLoc(0, i);
                    AP = TaUtil_GetUnitAP(0, i, Loc2 & 0xFF, Loc2 >> 8);
                    DP = TaUtil_GetUnitDP(0, i, Loc2 & 0xFF, Loc2 >> 8);
                    TaUtil_CalcUnitAP(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0, AP - TaUtil_GetUnitAP3(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, Loc & 0xFF, Loc >> 8));
                    TaUtil_CalcUnitDP(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0, DP - TaUtil_GetUnitDP3(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, Loc & 0xFF, Loc >> 8));
                } else {
                    Loc2 = SzDuel_GetUnitLoc(1, i - 11);
                    AP = TaUtil_GetUnitAP(1, i - 11, Loc2 & 0xFF, Loc2 >> 8);
                    DP = TaUtil_GetUnitDP(1, i - 11, Loc2 & 0xFF, Loc2 >> 8);
                    TaUtil_CalcUnitAP(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0, AP - TaUtil_GetUnitAP3(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, Loc & 0xFF, Loc >> 8));
                    TaUtil_CalcUnitDP(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0, DP - TaUtil_GetUnitDP3(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, Loc & 0xFF, Loc >> 8));
                }
            }
        }
        SzDuel_CloseMsgPopup();
        Temp->wLvl++;
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wCamTime = 0;
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
