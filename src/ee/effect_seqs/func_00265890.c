#include "dotr/ee/effect_seqs.h"

int Ta_Eff_046(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int j;
    int tCount;
    int LOC;
    int Unit;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 128;
        TaUtil_DoOpen(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 1);
        TaView_ReqFocusCamera(0, eInfo->dwFrom, 100, 180);
        TaEffect_LoadModule(Temp, 602);
        TaEffect_LoadModule(Temp, 603);
        TaEffect_LoadModule(Temp, 546);
    } else switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (SzDuel_GetUnitOpenRate(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) == 255) {
                if (TaEffect_IsLoadModule(Temp) == 1) {
                    if (eInfo->dwPar == 1) {
                        SzDuel_OpenMsgPopup(0, -120, 80, 260);
                    } else {
                        SzDuel_OpenMsgPopup(0, -120, 80, 259);
                    }
                    pCall->eNO = 602;
                    pCall->dwFrom = 0;
                    pCall->dwTo = eInfo->dwFrom;
                    pCall->dwPar = 0;
                    Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                    Temp->wLvl++;
                }
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        return 1;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0])) {
            return 1;
        }
        SzDuel_SetUnitStatus(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF, 0, 1);
        TaView_ReqFocusCamera(16, eInfo->dwFrom, 500, 500);
        Temp->wCount_1 = 0;
        LOC = SzDuel_GetUnitLoc(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF);
        pCall->eNO = 546;
        pCall->dwFrom = 0;
        pCall->dwTo = LOC;
        pCall->dwPar = 1;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->ePos[0] = eInfo->dwFrom;
        Temp->wCount_1++;
        Temp->oNum++;
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if ((LOC & 0xFF) + i >= 0 && (LOC & 0xFF) + i < 7 && (LOC >> 8) + j >= 0 && (LOC >> 8) + j < 7) {
                    if ((i + j >= 0 ? i + j : -(i + j)) < 3) {
                        if (i != 0 || j != 0) {
                            pCall->eNO = 546;
                            pCall->dwFrom = 0;
                            pCall->dwTo = (((LOC >> 8) + j) << 8) | ((LOC & 0xFF) + i);
                            pCall->dwPar = 0;
                            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                            if (SzDuel_GetMapUnit((LOC & 0xFF) + i, (LOC >> 8) + j)) {
                                Unit = SzDuel_GetMapUnitID((LOC & 0xFF) + i, (LOC >> 8) + j);
                                if (eInfo->dwPar == 1) {
                                    Temp->ePos[Temp->oNum] = Unit;
                                    Temp->wCount_1++;
                                } else if (Ta_IsUnitControl(Unit >> 8, Unit & 0xFF) != Ta_IsUnitControl(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF)) {
                                    if (Ta_IsLDRangeAvility(Unit >> 8, Unit & 0xFF, 11) == 0) {
                                        if (Ta_IsResistAll(Unit >> 8, Unit & 0xFF) != 1) {
                                            Temp->ePos[Temp->oNum] = Unit;
                                            Temp->wCount_1++;
                                            if (eInfo->dwPar == 0) {
                                                if ((unsigned int)Unit == eInfo->dwTo) {
                                                    tCount++;
                                                }
                                            }
                                        }
                                    }
                                } else {
                                    Temp->ePos[Temp->oNum] = Unit;
                                    Temp->wCount_1++;
                                    if (eInfo->dwPar == 0) {
                                        if ((unsigned int)Unit == eInfo->dwTo) {
                                            tCount++;
                                        }
                                    }
                                }
                            } else {
                                Temp->ePos[Temp->oNum] = -1;
                            }
                            Temp->oNum++;
                        }
                    }
                }
            }
        }
        if (tCount == 0 && eInfo->dwPar == 0) {
            pCall->eNO = 603;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 1;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = -1;
            Temp->oNum++;
        }
        Temp->wLvl++;
        return 1;
    case 2:
        Temp->fObj2++;
        for (i = 0; i < Temp->oNum; i++) {
            switch (Temp->fObj2) {
            case 1:
                SzSound_PlaySE(1, 49);
                break;
            case 5:
                SzSound_PlaySE(1, 49);
                SzSound_PlaySE(1, 49);
                SzSound_PlaySE(1, 49);
                SzSound_PlaySE(1, 49);
                break;
            }
            if (SzEffect_IsUse(Temp->eID[i])) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if (Temp->ePos[i] >= 0) {
                        if ((Temp->ePos[i] & 0xFF) > 0) {
                            Ta_UnitFade(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, -1, 1, 3);
                            Temp->ePos[Temp->oNum + Temp->wCount_2] = Temp->ePos[i];
                            Temp->ePos[i] = -1;
                        }
                        Temp->wCount_2++;
                    }
                }
            }
        }
        if (Temp->wCount_1 <= Temp->wCount_2) {
            Temp->wLvl++;
        }
        break;
    case 3:
        if (Ta_IsFinishFade()) {
            for (i = Temp->oNum; i < Temp->oNum + Temp->wCount_2; i++) {
                if (Temp->ePos[i] >= 0) {
                    if ((Temp->ePos[i] & 0xFF) > 0) {
                        Ta_DelUnit(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF);
                    }
                }
            }
            if (eInfo->dwTo != 0) {
                Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
            }
            Temp->wCount_1 = 48;
            Temp->wLvl++;
        }
        break;
    case 4:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            Temp->wLvl++;
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
        }
        break;
    default:
        if (Temp->wCamTime++ > 16) {
            if (Ta_IsFinishEffect(Temp) == 1) {
                if (Ta_IsFinishFade() == 1) {
                    if (SzDuel_GetWindowDisp() != 0) {
                        SzDuel_SetWindowDraw(1);
                    }
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
            }
        }
        break;
    }
    return 1;
}
