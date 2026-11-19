#include "dotr/ee/effect_seqs.h"

int Ta_Eff_100(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c *pCall;
    struct jp_6989c eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    pCall = &eCall;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        if (SzDuel_GetUnitCtlRateM(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) > 0) {
            Ta_UnitFade(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, -1, 1, 1);
            Temp->wCount_1 = 1;
        }
        if (SzDuel_GetUnitCtlRateM(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) > 0) {
            Ta_UnitFade(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, -1, 1, 1);
            Temp->wCount_2 = 1;
        }
        if (eInfo->dwPar == 999) {
            TaEffect_LoadModule(Temp, 538);
        } else if (eInfo->eNO == 43) {
            TaEffect_LoadModule(Temp, 543);
        } else {
            TaEffect_LoadModule(Temp, 600);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) != 1) {
            return 1;
        }
        if (Temp->wCount_1 != 0) {
            if (SzDuel_GetUnitCtlRateM(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) !=
                SzDuel_GetUnitCtlGoalM(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF)) {
                return 1;
            }
        }
        if (Temp->wCount_2 != 0) {
            if (SzDuel_GetUnitCtlRateM(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) !=
                SzDuel_GetUnitCtlGoalM(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF)) {
                return 1;
            }
        }
        if (eInfo->dwPar == 999) {
            pCall->eNO = 538;
            pCall->dwFrom = eInfo->dwFrom;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 1;
        } else if (eInfo->eNO == 43) {
            pCall->eNO = 543;
            pCall->dwFrom = eInfo->dwFrom;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 1;
        } else {
            pCall->eNO = 600;
            pCall->dwFrom = eInfo->dwFrom;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 1;
        }
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->ePos[0] = 0;
        Temp->wLvl++;
        return 1;
    default:
        if (eInfo->dwPar == 999) {
            if (SzEffect_IsUse(Temp->eID[0])) {
                switch (Temp->fObj2++) {
                case 0:
                    SzSound_PlaySE(1, 56);
                    break;
                case 20:
                    SzSound_PlaySE(1, 57);
                    break;
                case 30:
                    SzSound_PlaySE(1, 76);
                    break;
                case 36:
                    SzSound_PlaySE(1, 74);
                    break;
                case 48:
                    SzSound_PlaySE(1, 60);
                    break;
                }
                return 1;
            } else {
                Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
                if (Temp->wCount_2 != 0) {
                    Ta_UnitFade(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1, 1, 1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
        } else {
            if (SzEffect_IsUse(Temp->eID[0])) {
                switch (Temp->fObj2++) {
                case 0:
                    SzSound_PlaySE(1, 56);
                    break;
                case 20:
                    SzSound_PlaySE(1, 57);
                    break;
                case 30:
                    SzSound_PlaySE(1, 76);
                    break;
                case 40:
                    SzSound_PlaySE(1, 58);
                    break;
                case 100:
                    SzSound_PlaySE(1, 59);
                    break;
                case 120:
                    SzSound_PlaySE(1, 60);
                    break;
                }
                if (SzEffect_GetStatus(Temp->eID[0]) != 2) {
                    return 1;
                }
                if (Temp->ePos[0] == 0) {
                    SzDuel_AddGrave(eInfo->dwTo >> 8, SzDuel_GetUnitCardNo(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF));
                    SzDuel_PutUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, eInfo->dwPar);
                    Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
                    SzDuel_SetUnitCtlModeC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1);
                    SzDuel_SetUnitCtlGoalC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0);
                    SzDuel_SetUnitCtlRateC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, -1, 0);
                    Temp->ePos[0] = 1;
                }
                return 1;
            } else {
                SzDuel_SetUnitCtlModeC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1);
                SzDuel_SetUnitCtlGoalC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 128);
                SzDuel_SetUnitCtlRateC(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1, 128);
                SzSound_PlaySE(0, 2);
                SzDuel_CloseMsgPopup();
                return 0;
            }
        }
        break;
    }
    return 1;
}
