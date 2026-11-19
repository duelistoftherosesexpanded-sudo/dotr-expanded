#include "dotr/ee/effect_seqs.h"

int Ta_Eff_005(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    int tCount;
    int nMiss;
    int Unit;
    int Unit2;
    int Loc;
    int Self;
    int LOC;
    int EndCond;
    unsigned char FinishCond;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    LOC = 0;
    EndCond = 0;
    FinishCond = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->fObj2 = Ta_RemoveObjFromFlagLst(eInfo->dwTo, (unsigned short)eInfo->dwPar);
        if ((Unit = Ta_IsActiveTrapB((eInfo->dwPar >> 16) == 0 ? 1 : 0, 80)) < 0) {
            TaView_ReqFocusCamera2(0, eInfo->dwFrom, 200);
        } else {
            if ((Unit2 = Ta_IsActiveTrapB(eInfo->dwPar >> 16, 46)) < 0) {
                TaView_ReqFocusCamera(0, Unit, 180, 180);
                TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
                Temp->ePos[0] = Unit;
                Temp->fObj = 130;
                Temp->wLvl = 128;
                Temp->bCamLvl = 32;
            } else {
                TaView_ReqFocusCamera(0, Unit2, 180, 180);
                TaUtil_DoOpen(Unit2 >> 8, Unit2 & 0xFF, 1);
                Temp->ePos[0] = Unit2;
                Temp->fObj = 160;
                Temp->wLvl = 128;
                Temp->bCamLvl = 32;
            }
        }
        TaEffect_LoadModule(Temp, 505);
        TaEffect_LoadModule(Temp, 602);
        return 1;
    }
    if (TaEffect_IsLoadModule(Temp) == 1) {
        if (Temp->wLvl < 128 || Temp->wLvl == 255) {
            for (tCount = 0; tCount < Temp->oNum; tCount++) {
                if (SzEffect_IsUse(Temp->eID[tCount]) != 0) {
                    if (SzEffect_GetStatus(Temp->eID[tCount]) == 2) {
                        if (Temp->ePos[tCount] >= 0) {
                            if (SzDuel_GetMapUnit(Temp->ePos[tCount] & 0xFF, Temp->ePos[tCount] >> 8) != 0) {
                                Unit = SzDuel_GetMapUnitID(Temp->ePos[tCount] & 0xFF, Temp->ePos[tCount] >> 8);
                                if (Ta_GetAgreeOwnerIDon22bitFlagLst(Unit, eInfo->dwTo) != 0) {
                                    Temp->ePos[tCount] = -1;
                                    if (Ta_GetAgreeOwnerIDon22bitFlagLst(Unit, Temp->fObj2) != 0) {
                                        if ((Unit & 0xFF) > 0 && (Unit & 0xFF) < 6) {
                                            Ta_UnitFade(Unit >> 8, Unit & 0xFF, -1, 1, 3);
                                            Temp->ePos[Temp->wCount_2 + 13] = Unit;
                                            Temp->wCount_2++;
                                        }
                                    } else {
                                        Self = Ta_GetCurrentSide(Unit >> 8, Unit & 0xFF);
                                        Loc = SzDuel_GetUnitLoc(Self, 0);
                                        if (Ta_IsLDRangeAvility(Unit >> 8, Unit & 0xFF, 11) == 1) {
                                            Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                                            SzDuel_OpenMsgPopup(0, -120, 80, 270);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            for (tCount = 0; tCount < Temp->wCount_2; tCount++) {
                if (SzDuel_GetUnitCtlRateC(Temp->ePos[tCount + 13] >> 8, Temp->ePos[tCount + 13] & 0xFF) <= 0) {
                    if (SzDuel_GetUnitCtlRateM(Temp->ePos[tCount + 13] >> 8, Temp->ePos[tCount + 13] & 0xFF) <= 0) {
                        LOC++;
                    }
                }
            }
            if (LOC >= Temp->wCount_2) {
                EndCond++;
            }
            if (Temp->wCount_1 % 12 == 0) {
                if (Temp->wLvl != 255) {
                    if (Temp->wLvl == 0) {
                        Loc = eInfo->dwFrom;
                        pCall->eNO = 505;
                        pCall->dwFrom = 0;
                        pCall->dwTo = Loc;
                        pCall->dwPar = 0;
                        /* The original tests the stored byte, zero-extended, for >= 0
                         * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                        if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                            Temp->ePos[Temp->oNum] = Loc;
                            Temp->oNum++;
                        }
                        nMiss = 0;
                    } else {
                        for (tCount = 0, nMiss = 0; tCount < 4; tCount++) {
                            Loc = Ta_GetLocDistanceFromCenterOnRangeXY(eInfo->dwFrom, Temp->wLvl, tCount);
                            if (Loc >= 0) {
                                pCall->eNO = 505;
                                pCall->dwFrom = 0;
                                pCall->dwTo = Loc;
                                pCall->dwPar = 0;
                                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                                    Temp->ePos[Temp->oNum] = Loc;
                                    Temp->oNum++;
                                }
                            } else {
                                nMiss++;
                            }
                        }
                    }
                }
                if (nMiss >= 4) {
                    Temp->wLvl = 255;
                    for (tCount = 0, nMiss = 0; tCount < Temp->oNum; tCount++) {
                        if (SzEffect_IsUse(Temp->eID[tCount]) == 0) {
                            nMiss++;
                        }
                    }
                    if (nMiss >= Temp->oNum) {
                        EndCond++;
                    }
                } else {
                    Temp->wLvl++;
                }
            }
            Temp->wCount_1++;
            if (EndCond == 2) {
                for (tCount = 0; tCount < Ta_GetUnitNumFrom22bitFlagList(Temp->fObj2); tCount++) {
                    Unit = Ta_GetUnitOwnerIDFromLstCount(Temp->fObj2, tCount);
                    Ta_DelUnit(Unit >> 8, Unit & 0xFF);
                }
                FinishCond = 1;
            }
        } else {
            switch (Temp->wLvl - 128) {
            case 0:
                if (SzDuel_GetUnitOpenRate(Temp->ePos[0] >> 8, Temp->ePos[0] & 0xFF) == 255) {
                    if (Temp->fObj == 130) {
                        SzDuel_OpenMsgPopup(0, -120, 80, 259);
                    } else {
                        SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(825));
                    }
                    pCall->eNO = 602;
                    pCall->dwFrom = 0;
                    pCall->dwTo = Temp->ePos[0];
                    pCall->dwPar = 0;
                    Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                    Temp->wLvl++;
                }
                break;
            case 1:
                if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 0) {
                    SzDuel_SetUnitStatus(Temp->ePos[0] >> 8, Temp->ePos[0] & 0xFF, 0, 1);
                    SzDuel_CloseMsgPopup();
                    Temp->wLvl = Temp->fObj;
                    if (Temp->fObj - 128 == 2) {
                        Temp->bCamLvl++;
                    } else {
                        Temp->bCamLvl = 0;
                        TaView_ReqFocusCamera2(0, eInfo->dwFrom, 200);
                    }
                }
                break;
            case 2:
                SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(821));
                if (Temp->wCount_2 < 10) {
                    if (Temp->wCount_1 % 6 == 0) {
                        pCall->eNO = 505;
                        pCall->dwFrom = 0;
                        pCall->dwTo = SzDuel_GetUnitLoc(Temp->ePos[0] >> 8, Temp->ePos[0] & 0xFF);
                        pCall->dwPar = 0;
                        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                        Temp->ePos[Temp->oNum] = Temp->ePos[0];
                        Temp->wCount_2++;
                    }
                    Temp->wCount_1++;
                } else {
                    SzDuel_CloseMsgPopup();
                    if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 0) {
                        FinishCond = 1;
                    }
                }
                break;
            case 32:
                pCall->eNO = 505;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwFrom;
                pCall->dwPar = 0;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                Temp->ePos[0] = ((eInfo->dwFrom >> 8) << 8) | (eInfo->dwFrom & 0xFF);
                Temp->oNum++;
                Temp->wCount_1++;
                Temp->wLvl = 0;
                TaView_ReqFocusCamera2(0, eInfo->dwFrom, 200);
                break;
            }
        }
        switch (Temp->bCamLvl) {
        case 0:
            if (Temp->wCamTime++ > 16) {
                Temp->wCamTime = 0;
                Temp->bCamLvl++;
                TaView_ReqStdCompleteView(30);
            }
            break;
        case 1:
            if (Temp->wCamTime++ > 30) {
                Temp->bCamLvl++;
            }
            break;
        case 2:
            if (FinishCond & 1) {
                SzDuel_CloseMsgPopup();
                TaView_RestoreOriginalView(Temp, 16);
                Temp->bCamLvl = 255;
                Temp->wCamTime = 0;
            }
            break;
        case 32:
            break;
        case 33:
            TaView_ReqFocusCamera(30, Temp->ePos[0], 500, 500);
            Temp->bCamLvl = 1;
            Temp->wCamTime = 0;
            break;
        default:
            if (Temp->wCamTime++ > 16) {
                FinishCond |= 2;
            }
            break;
        }
    }
    if (FinishCond == 3) {
        if (Ta_IsFinishFade() == 1) {
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        }
    }
    return 1;
}
