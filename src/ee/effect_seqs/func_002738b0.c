#include "dotr/ee/effect_seqs.h"

int Ta_Eff_113(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int Loc;
    int Num;
    int Unit;
    int i;
    int tCount;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = (eInfo->dwPar >> 31) ? (SzDuel_GetTurnOwner() == 0 ? 1 : 0) : SzDuel_GetTurnOwner();
        Loc = SzDuel_GetUnitLoc(Temp->wCount_1, 0);
        TaView_ReqFocusCamera(0, Temp->wCount_1 << 8, 300, 300);
        SzDuel_OpenMsgPopup(0, -120, 80, 270);
        if (eInfo->dwFrom & 0x3FFFFF) {
            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Temp->wCount_1), 1, 1, 16);
        } else if (eInfo->dwTo != 0) {
            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Temp->wCount_1), 1, 1, 15);
        } else if (eInfo->dwPar & 0x3FFFFF) {
            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Temp->wCount_1), 1, 1, 13);
        }
        TaEffect_LoadModule(Temp, 613);
        if (eInfo->dwFrom & 0x3FFFFF) {
            TaEffect_LoadModule(Temp, 603);
        }
        if (eInfo->dwTo != 0) {
            TaEffect_LoadModule(Temp, 500);
        }
        if (eInfo->dwPar & 0x3FFFFF) {
            TaEffect_LoadModule(Temp, 513);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            Loc = SzDuel_GetUnitLoc(Temp->wCount_1, 0);
            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Temp->wCount_1), 0, -1, 16);
            pCall->eNO = 613;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->wCount_1 << 8;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[0] = 0;
            Temp->wLvl++;
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        TaView_ReqStdCompleteView(16);
        Temp->wLvl++;
        return 1;
    case 2:
        if (Temp->wCamTime++ >= 16) {
            if (eInfo->dwFrom & 0x3FFFFF) {
                Ta_SetEffectDependOnFlagLst3(eInfo, Temp, pCall, 603);
            }
            if (eInfo->dwTo != 0) {
                Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 500);
            }
            if (eInfo->dwPar & 0x3FFFFF) {
                Ta_SetEffectDependOnFlagLst2(eInfo, Temp, pCall, 513);
            }
            Temp->wLvl++;
        }
        return 1;
    case 3:
        for (i = 0, tCount = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (Temp->oNum == tCount) {
        Temp->wLvl++;
        }
        return 1;
    case 4:
        Num = Ta_GetUnitNumFrom22bitFlagList(eInfo->dwFrom & 0x3FFFFF);
        for (i = 0; i < Num; i++) {
            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwFrom & 0x3FFFFF, i);
            Ta_DelUnit(Unit >> 8, Unit & 0xFF);
        }
        Num = Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo);
        for (i = 0; i < Num; i++) {
            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
            SzDuel_SetUnitSpellRest(Unit >> 8, Unit & 0xFF, 255);
            SzDuel_SetUnitStatus(Unit >> 8, Unit & 0xFF, 0, 1);
        }
        Num = Ta_GetUnitNumFrom22bitFlagList(eInfo->dwPar & 0x3FFFFF);
        for (i = 0; i < Num; i++) {
            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwPar & 0x3FFFFF, i);
            TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
        }
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wCamTime = 0;
        Temp->wLvl++;
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
