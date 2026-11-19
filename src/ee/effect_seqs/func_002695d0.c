#include "dotr/ee/effect_seqs.h"

int Ta_Eff_036(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int temp2;
    int temp;
    int Unit2;
    int Unit;

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
        if (eInfo->dwPar == 999) {
            TaEffect_LoadModule(Temp, 538);
            Temp->fObj = 1;
        } else {
            Unit = Temp->fObj == 1 ? eInfo->dwFrom : eInfo->dwTo;
            Unit2 = Temp->fObj == 1 ? eInfo->dwTo : eInfo->dwFrom;
            if ((temp = Ta_IsActiveTrapB(eInfo->dwFrom >> 8, 43)) < 0
                || TaEnc_IsActReflexEnc((unsigned short)SzDuel_GetUnitCardNo(Unit2 >> 8, Unit2 & 0xFF)) == 0
                || Ta_IsLDRangeAvilityWithLocation(Unit >> 8, Unit & 0xFF,
                    SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF) & 0xFF,
                    SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF) >> 8, 11) == 1) {
                Temp->wCount_1 = 0;
                Ta_Eff_036_LoadEffect(eInfo, Temp);
            } else {
                TaEffect_LoadModule(Temp, 602);
                if ((temp2 = Ta_IsActiveTrapB(eInfo->dwTo >> 8, 46)) >= 0) {
                    TaView_ReqFocusCamera(0, temp2, 180, 180);
                    SzEffect_SetBusyFlag2(temp2, 0);
                    TaUtil_DoOpen(temp2 >> 8, temp2 & 0xFF, 1);
                    Temp->wLvl = 16;
                    Temp->wCount_1 = 0;
                    Temp->wCount_2 = temp2;
                    return 1;
                }
                TaView_ReqFocusCamera(0, temp, 180, 180);
                SzEffect_SetBusyFlag2(temp, 0);
                TaUtil_DoOpen(temp >> 8, temp & 0xFF, 1);
                Temp->wLvl = 16;
                Temp->wCount_1 = 1;
                Temp->wCount_2 = temp;
                return 1;
            }
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            if (eInfo->dwPar == 999) {
                pCall->eNO = 538;
                pCall->dwFrom = eInfo->dwFrom;
                pCall->dwTo = eInfo->dwTo;
                pCall->dwPar = 1;
            }
            if (Temp->wCount_1 != 0) {
                SzDuel_OpenMsgPopup(0, 0, -160, SzDuel_GetMsgName(816));
            } else {
                Unit = Temp->fObj == 1 ? eInfo->dwTo : eInfo->dwFrom;
                if (TaEnc_IsActReflexEnc((unsigned short)SzDuel_GetUnitCardNo(Unit >> 8, Unit & 0xFF)) != 0) {
                    SzDuel_OpenMsgPopup(0, 0, -160, 262);
                }
            }
            pCall->eNO = Temp->eMod.wModule[Temp->oNum];
            pCall->dwFrom = eInfo->dwFrom;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = Temp->fObj;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
            Ta_Eff_036_ManageSound(eInfo, Temp);
            if (SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2) {
                if ((Temp->ePos[Temp->oNum - 1] & 0xFF) == 0 && eInfo->dwPar != 999) {
                    if (SzDuel_GetUnitKind(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) >> 5 == 0) {
                        Enchant(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF,
                            (Temp->wCount_1 << 16) | (eInfo->dwPar & 0x3FFF), 0);
                        Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
                        if (SzDuel_GetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 4) == 0) {
                            SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 2, 0);
                        }
                    } else {
                        Enchant(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF,
                            (Temp->wCount_1 << 16) | (eInfo->dwPar & 0x3FFF), 0);
                        Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
                        if (SzDuel_GetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 4) == 0) {
                            SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 2, 0);
                        }
                    }
                    Temp->ePos[Temp->oNum - 1] = 0xFFFF;
                }
            }
            return 1;
        }
        SzDuel_CloseMsgPopup();
        if (eInfo->dwPar == 999) {
            Ta_DelUnit(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
            if (SzDuel_GetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 4) == 0) {
                SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 2, 0);
            }
        }
        SzSound_PlaySE(0, 2);
        SzDuel_CloseMsgPopup();
        return 0;
    case 16:
        if (SzDuel_GetUnitOpenRate(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF) == 255
            && TaEffect_IsLoadModule(Temp) == 1) {
            if (Temp->wCount_1 != 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, 259);
            } else {
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
            }
            pCall->eNO = 602;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->wCount_2;
            pCall->dwPar = 0;
            Temp->wLvl++;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->oNum++;
        }
        break;
    case 17:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
            return 1;
        }
        SzDuel_SetUnitStatus(Temp->wCount_2 >> 8, Temp->wCount_2 & 0xFF, 0, 1);
        Ta_Eff_036_LoadEffect(eInfo, Temp);
        TaView_RestoreOriginalView(Temp, 0);
        Temp->wLvl = 0;
        break;
    default:
        return 1;
    }
    return 1;
}
