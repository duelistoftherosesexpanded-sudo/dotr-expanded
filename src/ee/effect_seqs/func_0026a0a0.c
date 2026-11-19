#include "dotr/ee/effect_seqs.h"

int Ta_Eff_037(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int temp;
    int Efc;
    int Unit2;
    int Unit;
    int stored;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        if (SzDuel_GetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 2) != 0) {
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 4, 1);
        }
        if (SzDuel_GetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 2) != 0) {
            SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 4, 1);
        }
        if (SzDuel_GetUnitKind(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) >> 5 == 0) {
            Temp->fObj = 1;
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 2, 1);
        } else {
            Temp->fObj = 2;
            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 2, 1);
        }
        Efc = Ta_GetForceEffection(eInfo->dwFrom >> 8, Ta_GetEnchantOfs());
        Unit = (SzDuel_GetUnitKind(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) >> 5 == 0 ? 1 : 2) == 1
            ? eInfo->dwFrom : eInfo->dwTo;
        if (Ta_IsLDRangeAvilityWithLocation(Unit >> 8, Unit & 0xFF,
                SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF) & 0xFF,
                SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF) >> 8, 11) == 0) {
            if ((temp = Ta_IsActiveTrapB(eInfo->dwPar >> 16, 41)) >= 0) {
                TaEffect_LoadModule(Temp, 602);
                if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 46)) >= 0) {
                    TaEffect_LoadModule(Temp, 537);
                    TaView_ReqFocusCamera(0, Unit2, 180, 180);
                    SzEffect_SetBusyFlag2(Unit2, 0);
                    TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                    Temp->fObj = Unit2;
                    Temp->wLvl = 16;
                    Temp->wCount_2 = 0;
                    Temp->wCount_1 = Efc;
                    return 1;
                }
                TaEffect_LoadModule(Temp, 547);
                SzDuel_SetWindowDraw(0);
                TaView_ReqFocusCamera(0, temp, 180, 180);
                Temp->wLvl = 32;
                SzEffect_SetBusyFlag2(temp, 0);
                TaUtil_DoOpen(temp >> 8, temp & 0xFF, 1);
                Temp->fObj = temp;
                Temp->wCount_2 = 0;
                Temp->wCount_1 = Efc;
                return 1;
            }
            if ((temp = Ta_IsActiveTrapB(eInfo->dwTo >> 8, 43)) >= 0) {
                TaEffect_LoadModule(Temp, 537);
                TaEffect_LoadModule(Temp, 602);
                if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 46)) >= 0) {
                    TaView_ReqFocusCamera(0, Unit2, 180, 180);
                    SzEffect_SetBusyFlag2(Unit2, 0);
                    TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                    Temp->fObj = Unit2;
                    Temp->wLvl = 16;
                    Temp->wCount_2 = 0;
                    Temp->wCount_1 = Efc;
                    return 1;
                }
                TaView_ReqFocusCamera(0, temp, 180, 180);
                SzEffect_SetBusyFlag2(temp, 0);
                TaUtil_DoOpen(temp >> 8, temp & 0xFF, 1);
                Temp->fObj = temp;
                Temp->wLvl = 16;
                Temp->wCount_2 = 1;
                Temp->wCount_1 = Efc;
                return 1;
            }
        }
        TaEffect_LoadModule(Temp, 537);
        if (Efc < 0) {
            Efc = 0;
        }
        Temp->wLvl = 2;
        Temp->wCount_1 = Efc;
        Temp->wCount_2 = 0;
        return 1;
    }
    if (TaEffect_IsLoadModule(Temp) != 1) {
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        SzDuel_SetWindowDraw(0);
        SzDuel_OpenMsgPopup(0, -120, 80, 267);
        pCall->eNO = 547;
        pCall->dwFrom = (eInfo->dwTo >> 8 == 0 ? 1 : 0) << 8;
        pCall->dwTo = (eInfo->dwTo >> 8) << 8;
        pCall->dwPar = Temp->wCount_1;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->oNum++;
        Temp->wLvl++;
        return 1;
    case 1:
        SzDuel_SetWindowDraw(0);
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            if (Temp->wCamTime++ == 6) {
                TaView_ReqFireView(30, (eInfo->dwTo == 0 ? 1 : 0) << 8, 100, 180);
            }
        } else {
            SzDuel_CloseMsgPopup();
            if (SzDuel_GetUnitKind(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) >> 5 == 0) {
                Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
            } else {
                Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
            }
            SzDuel_CalcLP(eInfo->dwFrom >> 8, 1, Temp->wCount_1);
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            Temp->wLvl = 64;
        }
        return 1;
    case 2:
        if (Temp->wCount_2 == 1) {
            SzDuel_OpenMsgPopup(0, 0, -160, SzDuel_GetMsgName(816));
        } else {
            SzDuel_OpenMsgPopup(0, 0, -160, 268);
        }
        pCall->eNO = 537;
        pCall->dwFrom = eInfo->dwFrom;
        pCall->dwTo = eInfo->dwTo;
        pCall->dwPar = (Temp->wCount_1 << 16) | ((Temp->wCount_2 << 8)
            | (SzDuel_GetUnitKind(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) >> 5 == 0 ? 1 : 2));
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->wLvl++;
        break;
    case 3:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
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
                SzSound_PlaySE(1, 77);
                break;
            case 37:
                SzSound_PlaySE(1, 59);
                break;
            case 64:
                SzSound_PlaySE(1, 60);
                break;
            }
            if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                /* The original compares the low byte of ePos[0] with 0xFFFF
                 * (`lbu; li 0xffff; beq`), always unequal; `stored` only keeps the host build quiet. */
                if ((stored = (unsigned char)Temp->ePos[0]) != 0xFFFF) {
                    if (SzDuel_GetUnitKind(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) >> 5 == 0) {
                        Enchant(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF,
                            (Temp->wCount_2 << 16) | (eInfo->dwPar & 0x3FFF), (eInfo->dwPar >> 16) & 0xFF);
                        Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
                        if (SzDuel_GetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 4) == 0) {
                            SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 2, 0);
                        }
                    } else {
                        Enchant(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF,
                            (Temp->wCount_2 << 16) | (eInfo->dwPar & 0x3FFF), (eInfo->dwPar >> 16) & 0xFF);
                        Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
                        if (SzDuel_GetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 4) == 0) {
                            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 2, 0);
                        }
                    }
                    Temp->ePos[0] = 0xFFFF;
                }
            }
            return 1;
        }
        SzDuel_CloseMsgPopup();
        SzSound_PlaySE(0, 2);
        SzDuel_CloseMsgPopup();
        return 0;
    case 16:
        if (SzDuel_GetUnitOpenRate(Temp->fObj >> 8, Temp->fObj & 0xFF) == 255) {
            if (Temp->wCount_2 == 1) {
                SzDuel_OpenMsgPopup(0, -120, 80, 259);
            } else {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
            }
            pCall->eNO = 602;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->fObj;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[0] = 0;
            Temp->oNum++;
            Temp->wLvl = 17;
        }
        break;
    case 17:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        SzDuel_SetUnitStatus(Temp->fObj >> 8, Temp->fObj & 0xFF, 0, 1);
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 0);
        Temp->wLvl = 2;
        break;
    case 32:
        SzDuel_SetWindowDraw(0);
        if (SzDuel_GetUnitOpenRate(Temp->fObj >> 8, Temp->fObj & 0xFF) == 255) {
            SzDuel_OpenMsgPopup(0, -120, 80, 814);
            pCall->eNO = 602;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->fObj;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[0] = 0;
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    case 33:
        SzDuel_SetWindowDraw(0);
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            return 1;
        }
        SzDuel_SetUnitStatus(Temp->fObj >> 8, Temp->fObj & 0xFF, 0, 1);
        TaView_ReqBackFocusCamera(0, (Temp->fObj >> 8) << 8, 100, 180);
        Temp->wLvl = 0;
        break;
    default:
        if (Temp->wCamTime++ > 16) {
            SzSound_PlaySE(0, 2);
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    }
    return 1;
}
