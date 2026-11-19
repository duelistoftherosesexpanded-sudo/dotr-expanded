#include "dotr/ee/effect_seqs.h"

int Ta_Eff_004(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    int i;
    int Unit;
    int Unit2;
    int tCount;
    int eID;
    unsigned char FinishCond;
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
        Temp->wCount_1 = 0xFF;
        Temp->wCount_2 = 0xFF;
        Temp->wLvl = 2;
        if ((Unit = Ta_IsActiveTrapB(eInfo->dwFrom, 45)) >= 0) {
            if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwFrom, 46)) >= 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
                TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                Temp->wCount_2 = Unit2;
                Temp->wLvl = 0;
                TaEffect_LoadModule(Temp, 602);
            } else {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(824));
                for (i = 0; i < 11; i++) {
                    eInfo->dwTo = ConvUnitIDtoFlagList_OFF(eInfo->dwTo, Ta_GetCurrentSide(Unit >> 8, Unit & 0xFF), i);
                }
                TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                Temp->wCount_2 = Unit | 0x10000;
                Temp->wLvl = 0;
                TaEffect_LoadModule(Temp, 602);
            }
        }
        if ((Unit = Ta_IsActiveTrapB(eInfo->dwFrom == 0 ? 1 : 0, 45)) >= 0) {
            if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwFrom, 46)) >= 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
                TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                Temp->wCount_1 = Unit2;
                Temp->wLvl = 0;
                TaEffect_LoadModule(Temp, 602);
            } else {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(824));
                for (i = 0; i < 11; i++) {
                    eInfo->dwTo = ConvUnitIDtoFlagList_OFF(eInfo->dwTo, Ta_GetCurrentSide(Unit >> 8, Unit & 0xFF), i);
                }
                TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                Temp->wCount_1 = Unit | 0x10000;
                Temp->wLvl = 0;
                TaEffect_LoadModule(Temp, 602);
            }
        } else {
            Temp->wLvl = 2;
        }
        TaEffect_LoadModule(Temp, 21);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_2 != 0xFF) {
            if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF) == 0xFF) {
                if (TaEffect_IsLoadModule(Temp) == 1) {
                    pCall->eNO = 602;
                    pCall->dwFrom = 0;
                    pCall->dwTo = Temp->wCount_2;
                    pCall->dwPar = 0;
                    eID = Ta_Do_Effect(pCall, Temp);
                    if (eID >= 0) {
                        Temp->eID[Temp->oNum] = eID;
                        Temp->oNum++;
                    }
                    Temp->wLvl = 1;
                }
            }
        }
        if (Temp->wCount_1 != 0xFF) {
            if (SzDuel_GetUnitOpenRate(Temp->wCount_1 >> 8, Temp->wCount_1 & 0xFF) == 0xFF) {
                if (TaEffect_IsLoadModule(Temp) == 1) {
                    pCall->eNO = 602;
                    pCall->dwFrom = 0;
                    pCall->dwTo = Temp->wCount_1;
                    pCall->dwPar = 0;
                    Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                    Temp->oNum++;
                    Temp->wLvl = 1;
                }
            }
        }
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        SzDuel_SetUnitStatus(Temp->wCount_1 >> 8, Temp->wCount_1 & 0xFF, 0, 1);
        if (tCount == Temp->oNum) {
            SzDuel_CloseMsgPopup();
            Temp->wLvl = 2;
        }
        break;
    case 2:
        Temp->bCamLvl = 2;
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 21;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            eID = Ta_Do_Effect(pCall, Temp);
            if (eID >= 0) {
                Temp->eID[0] = eID;
            }
            Temp->wLvl++;
        }
        break;
    case 3:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            if (Temp->fObj++ % 28 == 0) {
                SzSound_PlaySE(1, 73);
            }
            if (Temp->fObj >= 28) {
                Temp->fObj = 0;
            }
        } else {
            for (; tCount < 22; tCount++) {
                if (((eInfo->dwTo >> (21 - tCount)) & 1) == 1) {
                    if (tCount < 11) {
                        Ta_DelUnit(0, tCount);
                    } else {
                        Ta_DelUnit(1, tCount - 11);
                    }
                }
            }
            for (tCount = 0; tCount < 11; tCount++) {
                SzEffect_SetBusyFlag2(tCount, 0);
                SzEffect_SetBusyFlag2(tCount | 0x100, 0);
            }
            FinishCond = 1;
        }
        break;
    default:
        FinishCond = 1;
        break;
    }
    switch (Temp->bCamLvl) {
    case 0:
        TaView_ReqFocusCamera(0, (Temp->wCount_1 != 0xFF) ? Temp->wCount_1 : Temp->wCount_2, 180, 180);
        Temp->bCamLvl++;
        break;
    case 1:
        break;
    case 2:
        TaView_ReqStdCompleteView(20);
        Temp->bCamLvl++;
        break;
    case 3:
        if (Temp->wCamTime++ >= 20) {
            Temp->bCamLvl++;
            Temp->wCamTime = 0;
        }
        break;
    case 4:
        if (!(FinishCond & 1) && eInfo->dwTo != 0) {
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
