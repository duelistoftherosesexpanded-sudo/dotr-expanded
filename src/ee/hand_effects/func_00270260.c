#include "dotr/ee/hand_effects.h"

int Ta_Eff_Hand_Enc(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID)
{
    struct jp_8b329 eCall;
    struct jp_8b329 *pCall = &eCall;
    int temp;
    int Unit2;
    int Unit;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if ((Ta_GetEncEffect(eInfo->dwPar) & 0xffff) == 537) {
        return Ta_Eff_Hand_Force(eInfo, Temp, ID);
    }
    if (ID >= 0) {
        InitEffectTemp(Temp);
        switch (Ta_GetEncEffect(eInfo->dwPar) & 0xffff) {
        case 536:
            TaEffect_LoadModule(Temp, 536);
            break;
        case 540:
            TaEffect_LoadModule(Temp, 540);
            break;
        case 541:
            TaEffect_LoadModule(Temp, 541);
            break;
        case 542:
            TaEffect_LoadModule(Temp, 542);
            break;
        default:
            TaEffect_LoadModule(Temp, 536);
            break;
        }
        if ((SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0 ? 0 : 3) == 0) {
            Unit = eInfo->dwFrom >> 16;
        } else {
            Unit = eInfo->dwFrom & 0xffff;
        }
        if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 43)) < 0 ||
            (TaEnc_IsActReflexEnc(eInfo->dwPar & 0xffff) == 0 &&
             Ta_IsLDAbl(11, SzDuel_GetTurnOwner()) == 1 &&
             (SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) & 0x1f) == (SzDuel_GetUnitKind(SzDuel_GetTurnOwner(), 0) & 0x1f))) {
            Temp->wLvl = 2;
            Temp->wCount_1 = 0;
        } else {
            SzDuel_SetWindowDraw(0);
            TaEffect_LoadModule(Temp, 602);
            if ((temp = Ta_IsActiveTrapB(eInfo->dwTo, 46)) >= 0) {
                Temp->wLvl = 0;
                Temp->wCount_1 = 0;
                Temp->wCount_2 = temp;
            } else {
                Temp->wLvl = 0;
                Temp->wCount_1 = 1;
                Temp->wCount_2 = Unit2;
            }
            TaUtil_DoOpen(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xff, 1);
        }
        pCall->eNO = 602;
        pCall->dwFrom = eInfo->dwFrom >> 16;
        pCall->dwTo = eInfo->dwFrom & 0xffff;
        pCall->dwPar = 0;
        TaLib_c615(&Temp->nTasakiEfc, pCall, 0);
        return 1;
    }
    if (TaEffect_IsLoadModule(Temp) != 1) {
        if (Temp->wCount_2 > 0) {
            SzDuel_SetWindowDraw(0);
        }
        pCall->eNO = 602;
        pCall->dwFrom = eInfo->dwFrom >> 16;
        pCall->dwTo = eInfo->dwFrom & 0xffff;
        pCall->dwPar = 0;
        TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
        /* MATCHING DEVICE: jump to the `return 1` shared with the switch, as in
         * Ta_Eff_Hand_Force (delay-slot cleanup mode of the compiler). */
        goto one;
    } else {
        switch (Temp->wLvl) {
        case 0:
            SzDuel_SetWindowDraw(0);
            pCall->eNO = 602;
            pCall->dwFrom = eInfo->dwFrom >> 16;
            pCall->dwTo = eInfo->dwFrom & 0xffff;
            pCall->dwPar = 0;
            if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xff) == 255) {
                if (Temp->wCount_1 == 0) {
                    SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
                } else {
                    SzDuel_OpenMsgPopup(0, -120, 80, 259);
                }
                SzDuel_SetWindowBusy(1);
                TaView_ReqFocusCamera(0, Temp->wCount_2, 180, 180);
                pCall->eNO = 602;
                pCall->dwFrom = 0;
                pCall->dwTo = Temp->wCount_2;
                pCall->dwPar = 0;
                Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                Temp->oNum++;
                Temp->wLvl++;
            } else {
                TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
                return 1;
            }
            break;
        case 1:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
                if (Temp->wCount_2 > 0) {
                    SzDuel_SetWindowDraw(0);
                }
                return 1;
            }
            SzDuel_SetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xff, 0, 1);
            SzDuel_CloseMsgPopup();
            pCall->eNO = 5;
            pCall->dwFrom = eInfo->dwFrom >> 16;
            pCall->dwTo = eInfo->dwFrom & 0xffff;
            pCall->dwPar = 0;
            TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
            SzDuel_SetWindowBusy(0);
            TaView_RestoreOriginalView(Temp, 0);
            Temp->wLvl++;
            break;
        case 2:
            pCall->eNO = 5;
            pCall->dwFrom = eInfo->dwFrom >> 16;
            pCall->dwTo = eInfo->dwFrom & 0xffff;
            pCall->dwPar = 0;
            TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
            switch (Ta_GetEncEffect(eInfo->dwPar) & 0xffff) {
            case 536:
                if (Temp->wCount_1 != 0) {
                    SzDuel_OpenMsgPopup(0, 0, -160, SzDuel_GetMsgName(SzDuel_GetUnitCardNo(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xff)));
                } else {
                    SzDuel_OpenMsgPopup(0, 0, -160, 262);
                }
                pCall->eNO = 536;
                pCall->dwFrom = eInfo->dwFrom >> 16;
                pCall->dwTo = eInfo->dwFrom & 0xffff;
                pCall->dwPar = 0;
                break;
            case 540:
                pCall->eNO = 540;
                pCall->dwFrom = eInfo->dwFrom >> 16;
                pCall->dwTo = eInfo->dwFrom & 0xffff;
                pCall->dwPar = 0;
                break;
            case 541:
                pCall->eNO = 541;
                pCall->dwFrom = eInfo->dwFrom >> 16;
                pCall->dwTo = eInfo->dwFrom & 0xffff;
                pCall->dwPar = 0;
                break;
            case 542:
                pCall->eNO = 542;
                pCall->dwFrom = eInfo->dwFrom >> 16;
                pCall->dwTo = eInfo->dwFrom & 0xffff;
                pCall->dwPar = 0;
                break;
            default:
                pCall->eNO = 536;
                pCall->dwFrom = eInfo->dwFrom >> 16;
                pCall->dwTo = eInfo->dwFrom & 0xffff;
                pCall->dwPar = 0;
                break;
            }
            pCall->dwPar = SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0 ? 0 : 3;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->oNum++;
            Temp->wLvl++;
            break;
        case 3:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
                switch (Temp->fObj2++) {
                case 1:
                    SzSound_PlaySE(1, 76);
                    break;
                case 15:
                    switch (Ta_GetEncEffect(eInfo->dwPar) & 0xffff) {
                    case 536:
                        SzSound_PlaySE(1, 59);
                        break;
                    case 540:
                        SzSound_PlaySE(1, 75);
                        break;
                    case 541:
                        SzSound_PlaySE(1, 78);
                        break;
                    case 542:
                        SzSound_PlaySE(1, 79);
                        break;
                    }
                    break;
                case 16:
                    switch (Ta_GetEncEffect(eInfo->dwPar) & 0xffff) {
                    case 541:
                    case 542:
                        SzSound_PlaySE(1, 59);
                        break;
                    }
                    break;
                }
                if (SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2) {
                    if ((unsigned char)Temp->ePos[Temp->oNum - 1] == 0) {
                        if (SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0) {
                            Enchant(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff, Temp->wCount_1 << 16 | eInfo->dwPar, 0);
                        } else {
                            Enchant((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff, Temp->wCount_1 << 16 | eInfo->dwPar, 0);
                        }
                        Temp->ePos[Temp->oNum - 1] = 0xffff;
                    }
                }
                return 1;
            }
            SzDuel_CloseMsgPopup();
            if (SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0) {
                temp = SzDuel_GetUnitCtlRateC((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff);
                if (temp != SzDuel_GetUnitCtlGoalC((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff)) {
                    return 1;
                }
                Ta_DelUnit2((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff);
            } else {
                temp = SzDuel_GetUnitCtlRateC(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff);
                if (temp != SzDuel_GetUnitCtlGoalC(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff)) {
                    return 1;
                }
                Ta_DelUnit2(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff);
            }
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        default:
            return 1;
        }
    one:
        return 1;
    }
}
