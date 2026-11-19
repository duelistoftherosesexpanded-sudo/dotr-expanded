#include "dotr/ee/eff_009.h"

/* 0x2594d0. SzDuel_GetUnitOpenRate is declared without a prototype inside the function body. */
#ifndef __MWERKS__
/* Host build only: the code calls SzDuel_GetUnitOpenRate without a prototype. */
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

int Ta_Eff_009(void *effectAnimation, void *effectAnimationParams, int ID) {
    extern int SzDuel_GetUnitOpenRate();
    DotrEff009Info *eInfo = effectAnimation;
    DotrEff009Temp *Temp = effectAnimationParams;
    int Unit;
    int Unit2;
    int Par;
    DotrEff009Info eCall;
    DotrEff009Info *pCall;

    pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    Par = Ta_GetLPRevByNaturalEffect(0x2C, (unsigned short)eInfo->dwPar);
    if (Par == 0) {
        return 1;
    }
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 0x80;
        SzEffect_SetCardColBusyFlag2(0, 1);
        SzEffect_SetModelBusyFlag2(0, 1);
        SzEffect_SetCardColBusyFlag2(0x100, 1);
        SzEffect_SetModelBusyFlag2(0x100, 1);
        TaEffect_LoadModule(Temp, 0x1FD);
        if ((Unit = Ta_IsActiveTrapB(eInfo->dwTo == 0 ? 1 : 0, 0x29)) >= 0) {
            TaEffect_LoadModule(Temp, 0x25A);
            if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 0x2E)) >= 0) {
                TaView_ReqFocusCamera(0, Unit2, 0xB4, 0xB4);
                SzEffect_SetCardColBusyFlag2(Unit2, 1);
                SzEffect_SetModelBusyFlag2(Unit2, 1);
            } else {
                TaView_ReqFocusCamera2(0, eInfo->dwFrom, 0xFA);
                SzEffect_SetCardColBusyFlag2(Unit, 1);
                SzEffect_SetModelBusyFlag2(Unit, 1);
                TaEffect_LoadModule(Temp, 0x223);
            }
        } else {
            TaView_ReqFocusCamera2(0, eInfo->dwFrom, 0xFA);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 0x30) {
            Temp->wCount_1 -= 0xF;
        } else {
            Temp->wCount_1 = 0x30;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                if ((Unit = Ta_IsActiveTrapB(eInfo->dwTo == 0 ? 1 : 0, 0x29)) < 0) {
                    SzDuel_OpenMsgPopup(0, -0x78, 0x50, 0x10B);
                    TaView_ReqFireView(0x16, eInfo->dwTo << 8, 0x64, 0xB4);
                    pCall->eNO = 0x1FD;
                    pCall->dwFrom = eInfo->dwFrom;
                    pCall->dwTo = eInfo->dwTo << 8;
                    pCall->dwPar = Par;
                    Temp->wLvl = 1;
                } else {
                    if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 0x2E)) >= 0) {
                        TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                        Temp->wLvl = 0x10;
                        Temp->wCount_2 = Unit2;
                        return 1;
                    }
                    TaView_ReqFireView(0x16, eInfo->dwTo << 8, 0x64, 0xB4);
                    pCall->eNO = 0x1FD;
                    pCall->dwFrom = eInfo->dwFrom;
                    pCall->dwTo = eInfo->dwTo << 8;
                    pCall->dwPar = Par | 0x10000;
                    Temp->wLvl = 2;
                    Temp->wCount_2 = Unit;
                    Temp->bCamLvl = 1;
                }
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                Temp->oNum++;
            }
        }
        if (Temp->wCount_1 < 0x30) {
            Temp->wCount_1 = 0x30;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
            return 1;
        }
        SzDuel_CloseMsgPopup();
        Ta_CalcLPinEffectSeq(eInfo->dwTo, eInfo->dwPar >> 16, eInfo->dwPar & 0xFFFF);
        TaView_RestoreOriginalView(Temp, 0x10);
        Temp->wCamTime = 0;
        Temp->wLvl = 7;
        break;
    case 2:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 0) {
            Temp->wLvl++;
            TaView_ReqFocusCamera(0, Temp->wCount_2, 0xB4, 0xB4);
        }
        return 1;
    case 3:
        if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF) != 0xFF) {
            if (SzDuel_GetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 4) == 0) {
                TaUtil_DoOpen(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 1);
            }
        } else {
            SzDuel_OpenMsgPopup(0, -0x78, 0x50, SzDuel_GetMsgName(0x32E));
            pCall->eNO = 0x25A;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->wCount_2;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    case 4:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 0) {
            SzDuel_SetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 0, 1);
            Temp->wLvl++;
            TaView_ReqBackFocusCamera(0, eInfo->dwTo << 8, 0xB4, 0xB4);
        }
        break;
    case 5:
        SzDuel_OpenMsgPopup(0, -0x78, 0x50, 0x10B);
        pCall->eNO = 0x223;
        pCall->dwFrom = eInfo->dwTo << 8;
        pCall->dwTo = (eInfo->dwTo == 0 ? 1 : 0) << 8;
        pCall->dwPar = (unsigned short)eInfo->dwPar;
        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        Temp->oNum++;
        Temp->wLvl++;
        Temp->wCamTime = 0;
        return 1;
    case 6:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
            if (Temp->wCamTime++ == 6) {
                TaView_ReqFireView(0x16, (eInfo->dwTo == 0 ? 1 : 0) << 8, 0x64, 0xB4);
            }
        } else {
            SzDuel_CloseMsgPopup();
            Ta_CalcLPinEffectSeq(eInfo->dwTo == 0 ? 1 : 0, eInfo->dwPar >> 16, eInfo->dwPar & 0xFFFF);
            TaView_RestoreOriginalView(Temp, 0x10);
            Temp->wCamTime = 0;
            Temp->wLvl++;
        }
        break;
    case 7:
        Temp->wCount_1 += 0xF;
        if (Temp->wCount_1 > 0x80) {
            Temp->wCount_1 = 0x80;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 0x80 && Temp->wCamTime++ > 0x10) {
            SzEffect_SetCardColBusyFlag2(0, 0);
            SzEffect_SetModelBusyFlag2(0, 0);
            SzEffect_SetCardColBusyFlag2(0x100, 0);
            SzEffect_SetModelBusyFlag2(0x100, 0);
            SzEffect_SetCardColBusyFlag2(Temp->wCount_2, 0);
            SzEffect_SetModelBusyFlag2(Temp->wCount_2, 0);
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    case 16:
        if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF) >= 0xFF) {
            SzDuel_OpenMsgPopup(0, -0x78, 0x50, SzDuel_GetMsgName(0x339));
            pCall->eNO = 0x25A;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->wCount_2;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->oNum++;
            TaView_ReqFocusCamera2(0, eInfo->dwFrom, 0xFA);
            Temp->wLvl++;
        }
        break;
    case 17:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 0) {
            SzDuel_SetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 0, 1);
            Temp->wLvl++;
        }
        return 1;
    case 18:
        TaView_ReqFireView(0x16, eInfo->dwTo << 8, 0x64, 0xB4);
        pCall->eNO = 0x1FD;
        pCall->dwFrom = eInfo->dwFrom;
        pCall->dwTo = eInfo->dwTo << 8;
        pCall->dwPar = Par;
        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        Temp->oNum++;
        Temp->wLvl = 1;
        SzDuel_OpenMsgPopup(0, -0x78, 0x50, 0x10B);
        return 1;
    default:
        return 1;
    }
    return 1;
}
