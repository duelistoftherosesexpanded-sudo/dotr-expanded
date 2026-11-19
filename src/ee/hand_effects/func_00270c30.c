#include "dotr/ee/hand_effects.h"

int Ta_Eff_Hand_Force(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID)
{
    int stored;
    struct jp_8b329 eCall;
    struct jp_8b329 *pCall = &eCall;
    int Unit2;
    int Unit;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = Ta_GetForceEffection(eInfo->dwTo, Ta_GetEnchantOfs());
        if ((SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0 ? 0 : 3) == 0) {
            Unit = eInfo->dwFrom >> 16;
        } else {
            Unit = eInfo->dwFrom & 0xffff;
        }
        if (Ta_IsLDAbl(11, SzDuel_GetTurnOwner()) == 1 &&
            (SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) & 0x1f) == (SzDuel_GetUnitKind(SzDuel_GetTurnOwner(), 0) & 0x1f)) {
            TaEffect_LoadModule(Temp, 537);
            Temp->wLvl = 2;
            Temp->wCount_2 = 0;
        } else if ((Unit = Ta_IsActiveTrapB(eInfo->dwTo, 41)) < 0) {
            if ((Unit = Ta_IsActiveTrapB(eInfo->dwTo, 43)) < 0) {
                TaEffect_LoadModule(Temp, 537);
                Temp->wLvl = 2;
                Temp->wCount_2 = 0;
            } else if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 46)) >= 0) {
                TaEffect_LoadModule(Temp, 602);
                TaEffect_LoadModule(Temp, 537);
                SzEffect_SetBusyFlag2(Unit2, 0);
                TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xff, 1);
                Temp->fObj = Unit2;
                Temp->wLvl = 0;
                Temp->wCount_2 = 0;
            } else {
                TaEffect_LoadModule(Temp, 602);
                TaEffect_LoadModule(Temp, 537);
                SzEffect_SetBusyFlag2(Unit, 0);
                TaUtil_DoOpen(Unit >> 8, Unit & 0xff, 1);
                Temp->fObj = Unit;
                Temp->wLvl = 0;
                Temp->wCount_2 = 1;
            }
        } else if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwTo, 46)) >= 0) {
            TaEffect_LoadModule(Temp, 602);
            TaEffect_LoadModule(Temp, 537);
            SzEffect_SetBusyFlag2(Unit2, 0);
            TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xff, 1);
            Temp->fObj = Unit2;
            Temp->wLvl = 0;
            Temp->wCount_2 = 0;
        } else {
            SzDuel_SetWindowDraw(0);
            TaEffect_LoadModule(Temp, 602);
            TaEffect_LoadModule(Temp, 547);
            SzEffect_SetBusyFlag2(Unit, 0);
            TaUtil_DoOpen(Unit >> 8, Unit & 0xff, 1);
            Temp->fObj = Unit;
            Temp->fObj2 = 1;
            Temp->wLvl = 0;
            Temp->wCount_2 = 0;
        }
        pCall->eNO = 602;
        pCall->dwFrom = eInfo->dwFrom >> 16;
        pCall->dwTo = eInfo->dwFrom & 0xffff;
        pCall->dwPar = 0;
        TaLib_c615(&Temp->nTasakiEfc, pCall, 0);
        return 1;
    }
    if (Temp->eMod.fLoad == 0) {
        pCall->eNO = 602;
        pCall->dwFrom = eInfo->dwFrom >> 16;
        pCall->dwTo = eInfo->dwFrom & 0xffff;
        pCall->dwPar = 0;
        TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
        if (TaEffect_IsLoadModule(Temp) == 1) {
            Temp->eMod.fLoad = -1;
        }
        /* MATCHING DEVICE: the jump to a `return 1` shared with the switch, inside the else
         * block and with no statement after it, is what makes the compiler remove the
         * instructions it duplicated into branch delay slots, everywhere in the function.
         * The plain form (`if {} else switch {}` followed by `return 1;`) is 60 bytes longer. */
        goto one;
    } else {
        switch (Temp->wLvl) {
        case 0:
            SzDuel_SetWindowDraw(0);
            pCall->eNO = 602;
            pCall->dwFrom = eInfo->dwFrom >> 16;
            pCall->dwTo = eInfo->dwFrom & 0xffff;
            pCall->dwPar = 0;
            if (SzDuel_GetUnitOpenRate(Temp->fObj >> 8, Temp->fObj & 0xff) == 255) {
                if (Temp->wCount_2 == 0 && Temp->fObj2 == 0) {
                    SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
                } else {
                    SzDuel_OpenMsgPopup(0, -120, 80, 259);
                }
                SzDuel_SetWindowBusy(1);
                TaView_ReqFocusCamera(0, Temp->fObj, 180, 180);
                pCall->eNO = 602;
                pCall->dwFrom = 0;
                pCall->dwTo = Temp->fObj;
                pCall->dwPar = 0;
                /* MATCHING DEVICE: the original tests the stored byte, zero-extended, for >= 0
                 * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                    Temp->ePos[Temp->oNum] = 0;
                    Temp->oNum++;
                    Temp->wLvl++;
                } else {
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
            } else {
                TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
            }
            break;
        case 1:
            SzDuel_SetWindowDraw(0);
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
                return 1;
            }
            SzDuel_SetUnitStatus(Temp->fObj >> 8, Temp->fObj & 0xff, 0, 1);
            if (Temp->fObj2 != 0) {
                SzDuel_SetWindowBusy(1);
                TaView_ReqBackFocusCamera(0, (eInfo->dwTo == 0 ? 1 : 0) << 8, 180, 180);
                Temp->wLvl = 16;
            } else {
                SzDuel_SetWindowDraw(1);
                pCall->eNO = 5;
                pCall->dwFrom = eInfo->dwFrom >> 16;
                pCall->dwTo = eInfo->dwFrom & 0xffff;
                pCall->dwPar = 0;
                TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
                SzDuel_SetWindowBusy(0);
                TaView_RestoreOriginalView(Temp, 0);
                Temp->wLvl++;
            }
            break;
        case 2:
            pCall->eNO = 5;
            pCall->dwFrom = eInfo->dwFrom >> 16;
            pCall->dwTo = eInfo->dwFrom & 0xffff;
            pCall->dwPar = 0;
            TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
            if (Temp->wCount_2 == 1) {
                SzDuel_OpenMsgPopup(0, 0, -160, SzDuel_GetMsgName(SzDuel_GetUnitCardNo(Temp->fObj >> 8, Temp->fObj & 0xff)));
            } else {
                SzDuel_OpenMsgPopup(0, 0, -160, 268);
            }
            pCall->eNO = 537;
            pCall->dwFrom = eInfo->dwFrom >> 16;
            pCall->dwTo = eInfo->dwFrom & 0xffff;
            pCall->dwPar = Temp->wCount_1 << 16 | (Temp->wCount_2 << 8 | (SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 ? 3 : 0));
            if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                Temp->ePos[Temp->oNum] = 0;
                Temp->oNum++;
                Temp->wLvl++;
            } else {
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        case 3:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
                switch (Temp->fObj2++) {
                case 1:
                    SzSound_PlaySE(1, 76);
                    break;
                case 15:
                    SzSound_PlaySE(1, 77);
                    break;
                case 16:
                    SzSound_PlaySE(1, 59);
                    break;
                }
                if (SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2) {
                    if ((unsigned char)Temp->ePos[Temp->oNum - 1] == 0) {
                        if (SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0) {
                            Enchant(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff, Temp->wCount_2 << 16 | eInfo->dwPar, eInfo->dwTo & 0xff);
                            Ta_DelUnit2((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff);
                        } else {
                            Enchant((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff, Temp->wCount_2 << 16 | eInfo->dwPar, eInfo->dwTo & 0xff);
                            Ta_DelUnit2(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff);
                        }
                        Temp->ePos[Temp->oNum - 1] = 0xffff;
                    }
                }
                return 1;
            }
            SzDuel_CloseMsgPopup();
            return 0;
        case 16:
            SzDuel_SetWindowDraw(0);
            SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(814));
            pCall->eNO = 547;
            pCall->dwFrom = (eInfo->dwTo == 0 ? 1 : 0) << 8;
            pCall->dwTo = eInfo->dwTo << 8;
            pCall->dwPar = Temp->wCount_1;
            if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                Temp->oNum++;
                Temp->wLvl++;
            } else {
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        case 17:
            SzDuel_SetWindowDraw(0);
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
                if (Temp->wCamTime++ == 6) {
                    TaView_ReqFireView(30, eInfo->dwTo << 8, 100, 180);
                }
                break;
            }
            if (SzDuel_GetUnitKind(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff) >> 5 == 0) {
                Ta_DelUnit2((eInfo->dwFrom & 0xffff) >> 8, eInfo->dwFrom & 0xff);
            } else {
                Ta_DelUnit2(eInfo->dwFrom >> 24, (eInfo->dwFrom >> 16) & 0xff);
            }
            SzDuel_CalcLP(eInfo->dwTo, 1, Temp->wCount_1);
            SzDuel_CloseMsgPopup();
            SzDuel_SetWindowBusy(0);
            TaView_RestoreOriginalView(Temp, 0);
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            return 0;
        }
    one:
        return 1;
    }
}
