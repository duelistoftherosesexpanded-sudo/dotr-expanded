#include "dotr/ee/effect_seqs.h"

int Ta_Eff_007(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    struct jp_6989c eCall;
    int Self;
    int Loc;
    int Unit2;
    int Unit;
    int i;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        /* MATCHING DEVICE: the flag list goes through a local first; passed directly, the two
         * argument loads come out in the other order. */
        Unit = eInfo->dwTo;
        Temp->fObj2 = Ta_RemoveObjFromFlagLst(Unit, (unsigned short)eInfo->dwPar);
        if ((Unit = Ta_IsActiveTrapB(eInfo->dwPar, 75)) >= 0) {
            if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwPar == 0 ? 1 : 0, 46)) >= 0) {
                TaView_ReqFocusCamera(0, Unit, 180, 180);
                TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                Temp->wLvl = 32;
                Temp->wCount_1 = Unit2;
                Temp->wCount_2 = 36;
                TaEffect_LoadModule(Temp, 602);
                TaEffect_LoadModule(Temp, 507);
            } else {
                TaView_ReqFocusCamera(0, Unit2, 180, 180);
                TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                Temp->wLvl = 32;
                Temp->wCount_1 = Unit;
                Temp->wCount_2 = 34;
                TaEffect_LoadModule(Temp, 602);
                TaEffect_LoadModule(Temp, 603);
            }
        } else {
            Ta_SetRangeVisual(eInfo->dwFrom & 0xFF, eInfo->dwFrom >> 8, 0, 0, 1, 18, 255);
            TaView_ReqStdCompleteView(30);
            TaEffect_LoadModule(Temp, 507);
            TaEffect_LoadModule(Temp, 603);
        }
        return 1;
    }
    if (TaEffect_IsLoadModule(Temp) != 1) {
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 507);
        Temp->wLvl++;
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1 && SzEffect_GetStatus(Temp->eID[i]) == 2
                && Temp->ePos[i] >= 0) {
                if (Ta_GetAgreeOwnerIDon22bitFlagLst(Temp->ePos[i], Temp->fObj2)) {
                    pCall->eNO = 603;
                    pCall->dwFrom = 0;
                    pCall->dwTo = Temp->ePos[i];
                    pCall->dwPar = 1;
                    /* The original tests the stored byte, zero-extended, for >= 0
                     * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                    if ((stored = (Temp->eID[Temp->oNum + i] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                        Temp->ePos[Temp->oNum + i] = Temp->ePos[i];
                        Temp->wCount_2++;
                        Temp->ePos[i] = -1;
                    }
                } else {
                    Self = Ta_GetCurrentSide(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF);
                    Loc = SzDuel_GetUnitLoc(Self, 0);
                    if (Ta_IsLDRangeAvility(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, 11) == 1) {
                        Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                        SzDuel_OpenMsgPopup(0, -120, 80, 270);
                    }
                    Temp->ePos[i] = -1;
                }
            }
        }
        for (i = 0; i < Temp->oNum + Temp->wCount_2; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1) {
                return 1;
            }
        }
        for (i = 0; i < 22; i++) {
            if (((Temp->fObj2 >> (21 - i)) & 1) == 1) {
                if (i < 11) {
                    Ta_DelUnit(0, i);
                } else {
                    Ta_DelUnit(1, i - 11);
                }
            }
        }
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wCamTime = 0;
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        Temp->wLvl++;
        break;
    case 2:
        if (Temp->wCamTime++ > 16) {
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    case 32:
        if (SzDuel_GetUnitOpenRate(Temp->wCount_1 >> 8, Temp->wCount_1 & 0xFF) != 255) {
            return 1;
        }
        if (Temp->wCount_2 == 36) {
            SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
        } else {
            SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(820));
        }
        pCall->eNO = 602;
        pCall->dwFrom = 0;
        pCall->dwTo = Temp->wCount_1;
        pCall->dwPar = 0;
        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        Temp->oNum++;
        Temp->wLvl++;
        break;
    case 33:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
            return 1;
        }
        SzDuel_SetUnitStatus(Temp->wCount_1 >> 8, Temp->wCount_1 & 0xFF, 0, 1);
        SzDuel_CloseMsgPopup();
        Temp->wLvl = Temp->wCount_2;
        break;
    case 34:
        pCall->eNO = 603;
        pCall->dwFrom = 0;
        pCall->dwTo = Temp->wCount_1;
        pCall->dwPar = 0;
        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        Temp->oNum++;
        Temp->wCount_2 = 35;
        Temp->wLvl = 33;
        break;
    case 35:
        Ta_DelUnit(Temp->wCount_1 >> 8, Temp->wCount_1 & 0xFF);
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wCamTime = 0;
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        Temp->wLvl = 2;
        break;
    case 36:
        Temp->oNum = 0;
        Temp->wCount_2 = 0;
        TaView_ReqStdCompleteView(30);
        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 507);
        Temp->wLvl = 1;
        return 1;
    default:
        return 1;
    }
    return 1;
}
