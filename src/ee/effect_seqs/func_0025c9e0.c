#include "dotr/ee/effect_seqs.h"

int Ta_Eff_026(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int Self;
    int Loc;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        if (eInfo->dwPar == 0xFFFFFFFF) {
            SzDuel_CloseMsgPopup();
            return 0;
        }
        SzDuel_SetWindowDraw(0);
        TaView_ReqStdCompleteView(16);
        TaEffect_LoadModule(Temp, 513);
        TaEffect_LoadModule(Temp, 26);
        TaEffect_LoadModule(Temp, 27);
        return 1;
    }
    if (eInfo->dwPar == 0xFFFFFFFF) {
        SzDuel_CloseMsgPopup();
        return 0;
    }
    SzDuel_SetWindowDraw(0);
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 26;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
        }
        break;
    case 1:
    case 3:
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            if (Temp->wLvl == 3) {
                TaView_RestoreOriginalView(Temp, 16);
                Temp->wCamTime = 0;
            }
            Temp->wLvl++;
        }
        break;
    case 2:
        if (eInfo->dwPar == 0xFFFE) {
            Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 27);
            if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
                TaView_ReqFocusCamera(0, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
            }
            Temp->wLvl++;
        } else {
            pCall->eNO = 27;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwPar;
            pCall->dwPar = 0;
            TaView_ReqFocusCamera(0, eInfo->dwPar, 100, 180);
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl++;
        }
        break;
    case 4:
        Temp->wLvl++;
        if (eInfo->dwPar == 0xFFFE) {
            for (i = 0; i < 22; i++) {
                if ((eInfo->dwTo >> (21 - i)) & 1) {
                    if (i < 11) {
                        Self = Ta_GetCurrentSide(0, i);
                        if (Ta_IsLDRangeAvility(0, i, 11)) {
                            Loc = SzDuel_GetUnitLoc(Self, 0);
                            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                            SzDuel_OpenMsgPopup(0, -120, 80, 270);
                        } else if (eInfo->dwPar != (unsigned int)Ta_GetCurrentSide(0, i)) {
                            if (SzDuel_GetUnitStatus(0, i, 1)) {
                                SzDuel_SetUnitStatus(0, i, 1, 0);
                            } else {
                                SzDuel_SetUnitStatus(0, i, 1, 1);
                            }
                            if (SzDuel_GetTurnOwner() == Ta_IsUnitControl(0, i)) {
                                if (SzDuel_GetUnitCurseRest(0, i) == 0) {
                                    if (SzDuel_GetUnitSpellRest(0, i) == 0) {
                                        SzDuel_SetUnitStatus(0, i, 0, 0);
                                    }
                                }
                            } else {
                                SzDuel_SetUnitStatus(0, i, 0, 1);
                            }
                        }
                    } else {
                        Self = Ta_GetCurrentSide(1, i - 11);
                        if (Ta_IsLDRangeAvility(1, i - 11, 11)) {
                            Loc = SzDuel_GetUnitLoc(Self, 0);
                            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                            SzDuel_OpenMsgPopup(0, -120, 80, 270);
                        } else if (eInfo->dwPar != (unsigned int)Ta_GetCurrentSide(1, i - 11)) {
                            if (SzDuel_GetUnitStatus(1, i - 11, 1)) {
                                SzDuel_SetUnitStatus(1, i - 11, 1, 0);
                            } else {
                                SzDuel_SetUnitStatus(1, i - 11, 1, 1);
                            }
                            if (SzDuel_GetTurnOwner() == Ta_IsUnitControl(1, i - 11)) {
                                if (SzDuel_GetUnitCurseRest(1, i - 11) == 0) {
                                    if (SzDuel_GetUnitSpellRest(1, i - 11) == 0) {
                                        SzDuel_SetUnitStatus(1, i - 11, 0, 0);
                                    }
                                }
                            } else {
                                SzDuel_SetUnitStatus(1, i - 11, 0, 1);
                            }
                        }
                    }
                }
            }
            if (Temp->wLvl > 4) {
                return 1;
            }
            if (Temp->wCamTime++ > 16) {
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
        } else {
            Self = Ta_GetCurrentSide(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF);
            if (Ta_IsLDRangeAvility(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF, 11)) {
                Loc = SzDuel_GetUnitLoc(Self, 0);
                Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                SzDuel_OpenMsgPopup(0, -120, 80, 270);
            } else if (Ta_IsResistAll(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF) == 1) {
                Temp->wCount_2 = Ta_IsResistAllMaster(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF);
                TaView_ReqFocusCamera(0, Temp->wCount_2, 100, 180);
                pCall->eNO = 513;
                pCall->dwFrom = 0;
                pCall->dwTo = Temp->wCount_2;
                pCall->dwPar = 0;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                Temp->wLvl = 16;
            } else if (eInfo->dwFrom != (unsigned int)Ta_GetCurrentSide(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF)) {
                if (SzDuel_GetUnitStatus(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF, 1)) {
                    SzDuel_SetUnitStatus(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF, 1, 0);
                } else {
                    SzDuel_SetUnitStatus(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF, 1, 1);
                }
                if (SzDuel_GetUnitCurseRest(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF) == 0) {
                    if (SzDuel_GetUnitSpellRest(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF) == 0) {
                        SzDuel_SetUnitStatus(eInfo->dwPar >> 8, eInfo->dwPar & 0xFF, 0, 0);
                    }
                }
            }
        }
        break;
    case 16:
        SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(83));
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            if (Temp->wCount_1++ <= 30) {
                SzDuel_CloseMsgPopup();
                TaView_RestoreOriginalView(Temp, 16);
                Temp->wCamTime = 0;
                Temp->wLvl++;
            }
        }
        break;
    default:
        if (Temp->wCamTime++ > 16) {
            SzDuel_CloseMsgPopup();
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            return 0;
        }
        break;
    }
    return 1;
}
