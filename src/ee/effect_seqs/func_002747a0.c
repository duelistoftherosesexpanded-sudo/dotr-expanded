#include "dotr/ee/effect_seqs.h"

int Ta_Eff_BTL_001(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int temp;
    int i;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (eInfo == 0 || Temp == 0 || pCall == 0) {
        SzDuel_CloseMsgPopup();
        return 0;
    }
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 128;
        if (((eInfo->dwPar >> 20) & 3) != 0 || ((eInfo->dwPar >> 22) & 3) != 0) {
            TaEffect_LoadModule(Temp, 513);
        }
        TaEffect_LoadModule(Temp, 604);
        TaEffect_LoadModule(Temp, 605);
        TaView_ReqFocusCamera(0, (unsigned short)eInfo->dwFrom, 300, 280);
        return 1;
    }
    switch (Temp->bCamLvl) {
    case 0:
        if (Temp->wCamTime++ == 0) {
            SzSound_PlaySE(0, 39);
            SzGame_SetAutoBlur(6);
            TaView_ReqFocusCamera(12, (unsigned short)eInfo->dwFrom, 100, 180);
            return 1;
        }
        if (Temp->wCamTime >= 12) {
            SzGame_SetAutoBlur(4);
        }
        if (Temp->wCamTime >= 24) {
            if (SzDuel_GetUnitOpenRate((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF) == 255) {
                if (SzDuel_GetUnitCtlRateC((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF) == 128) {
                    if (SzEffect_IsModelStandby2((unsigned short)eInfo->dwFrom) != 0) {
                        SzGame_SetAutoBlur(0);
                        TaView_ReqFocusCamera(0, (unsigned short)eInfo->dwTo, 300, 280);
                        Temp->wCamTime = 0;
                        Temp->bCamLvl++;
                    }
                }
            }
        }
        return 1;
    case 1:
        if (Temp->wCamTime++ == 0) {
            SzSound_PlaySE(0, 39);
            SzGame_SetAutoBlur(6);
            TaView_ReqFocusCamera(12, (unsigned short)eInfo->dwTo, 100, 180);
            return 1;
        }
        if (Temp->wCamTime >= 12) {
            SzGame_SetAutoBlur(4);
        }
        if (Temp->wCamTime >= 24) {
            if (SzDuel_GetUnitOpenRate((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF) == 255) {
                if (SzDuel_GetUnitCtlRateC((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF) == 128) {
                    if (SzEffect_IsModelStandby2((unsigned short)eInfo->dwTo) != 0) {
                        SzGame_SetAutoBlur(0);
                        TaView_RestoreOriginalView(Temp, 0);
                        Temp->wCamTime = 0;
                        Temp->bCamLvl++;
                    }
                }
            }
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            SzEffect_SetBusyFlag2((unsigned short)eInfo->dwFrom, 1);
            SzEffect_SetBusyFlag2((unsigned short)eInfo->dwTo, 1);
            if (((eInfo->dwPar >> 20) & 3) != 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, 260);
                pCall->eNO = 513;
                pCall->dwFrom = 0;
                pCall->dwTo = (unsigned short)eInfo->dwFrom;
                pCall->dwPar = 0;
                /* As in Ta_Eff_BTL_000, the stored byte is tested zero-extended
                 * for >= 0 (`andi; bltz`), always true; `stored` keeps the host build quiet. */
                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                    Temp->oNum++;
                }
            }
            if (((eInfo->dwPar >> 22) & 3) != 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, 260);
                pCall->eNO = 513;
                pCall->dwFrom = 0;
                pCall->dwTo = (unsigned short)eInfo->dwTo;
                pCall->dwPar = 0;
                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) > 0) {
                    Temp->oNum++;
                }
            }
            Temp->wLvl++;
        }
        return 1;
    case 1:
        if (Temp->oNum > 0) {
            for (i = 0; i < Temp->oNum; i++) {
                if (SzEffect_IsUse(Temp->eID[i]) != 0) {
                    return 1;
                }
            }
        }
        SzDuel_CloseMsgPopup();
        Temp->oNum = 0;
        Temp->wLvl++;
        return 1;
    case 2:
        Temp->wCamTime++;
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, -1)) > 0) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
        } else {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
            if (Temp->wCamTime >= 8) {
                if ((unsigned short)eInfo->dwPar == 0xFFFF) {
                    temp = 0;
                } else {
                    temp = Ta_GetLPRevByNaturalEffect(44, (unsigned short)eInfo->dwPar);
                }
                if (temp == 0) {
                    if (((eInfo->dwPar >> 28) & 1) == 1) {
                        pCall->eNO = 604;
                        pCall->dwFrom = (unsigned short)eInfo->dwFrom;
                        pCall->dwTo = (unsigned short)eInfo->dwTo;
                        pCall->dwPar = (((eInfo->dwPar >> 20) & 1) << 19) | (temp | 0x20000) | (((eInfo->dwPar >> 22) & 1) << 20);
                    } else {
                        pCall->eNO = 604;
                        pCall->dwFrom = (unsigned short)eInfo->dwFrom;
                        pCall->dwTo = (unsigned short)eInfo->dwTo;
                        pCall->dwPar = (((eInfo->dwPar >> 20) & 1) << 19) | (temp | 0x30000) | (((eInfo->dwPar >> 22) & 1) << 20);
                    }
                } else {
                    if (((eInfo->dwPar >> 28) & 1) == 1) {
                        pCall->eNO = 604;
                        pCall->dwFrom = (unsigned short)eInfo->dwFrom;
                        pCall->dwTo = (unsigned short)eInfo->dwTo;
                        pCall->dwPar = (((eInfo->dwPar >> 20) & 1) << 19) | (temp | 0x10000) | (((eInfo->dwPar >> 22) & 1) << 20);
                    } else {
                        pCall->eNO = 604;
                        pCall->dwFrom = (unsigned short)eInfo->dwFrom;
                        pCall->dwTo = (unsigned short)eInfo->dwTo;
                        pCall->dwPar = (((eInfo->dwPar >> 20) & 1) << 19) | temp | (((eInfo->dwPar >> 22) & 1) << 20);
                    }
                }
                if ((stored = (Temp->eID[0] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                    Temp->ePos[0] = 0;
                    Temp->oNum++;
                }
                pCall->eNO = 605;
                pCall->dwFrom = (unsigned short)eInfo->dwFrom;
                pCall->dwTo = (unsigned short)eInfo->dwTo;
                pCall->dwPar = (((eInfo->dwPar >> 19) & 1) << 1) | ((eInfo->dwPar >> 18) & 1);
                Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                Temp->ePos[Temp->oNum] = 0;
                SzEffect_Sleep(Temp->eID[Temp->oNum], 1);
                Temp->wLvl++;
            }
        }
        return 1;
    case 3:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                switch (Temp->ePos[0]) {
                case 0:
                    Temp->ePos[0]++;
                    if (SzDuel_GetOption3DBattle(SzDuel_GetTurnOwner()) != 0) {
                        if ((unsigned short)eInfo->dwPar != 0xFFFF) {
                            if (TaTuto_IsTuto(SzDuel_GetTurnOwner()) == 0) {
                                SzEffect_Sleep(Temp->eID[0], 1);
                                return 4;
                            }
                        }
                    }
                    break;
                case 1:
                case 2:
                    TaEffect_LoadModule(Temp, 500);
                    Temp->ePos[0]++;
                    SzSound_PlaySE(1, 65);
                    break;
                }
            }
            return 1;
        }
        Temp->wCount_1 = 0;
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
        if (((eInfo->dwPar >> 16) & 1) == 1 && (temp = (unsigned short)eInfo->dwPar) != 0xFFFF) {
            SzDuel_CalcLP(Ta_IsUnitControl((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF), 1, Ta_GetLPRevByNaturalEffect(44, temp));
        } else if (((eInfo->dwPar >> 17) & 1) == 1 && (temp = (unsigned short)eInfo->dwPar) != 0xFFFF) {
            SzDuel_CalcLP(Ta_IsUnitControl((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF), 1, Ta_GetLPRevByNaturalEffect(44, temp));
        }
        SzEffect_Sleep(Temp->eID[Temp->oNum++], 0);
        Temp->fObj2 = 0;
        Temp->wLvl++;
        return 1;
    case 4:
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, 1)) < 128) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
        }
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
            switch (Temp->fObj2++) {
            case 0:
                if (((eInfo->dwPar >> 19) & 1) != 0) {
                    Temp->ePos[Temp->oNum - 1]++;
                    SzSound_PlaySE(1, 66);
                    SzSound_PlaySE(1, 45);
                }
                if (((eInfo->dwPar >> 18) & 1) != 0) {
                    Temp->ePos[Temp->oNum - 1]++;
                    SzSound_PlaySE(1, 66);
                    SzSound_PlaySE(1, 45);
                }
                break;
            case 20:
                if (Temp->ePos[Temp->oNum - 1] == 0) {
                    if (((eInfo->dwPar >> 19) & 1) == 0) {
                        SzSound_PlaySE(0, 2);
                    }
                    if (((eInfo->dwPar >> 18) & 1) == 0) {
                        SzSound_PlaySE(0, 2);
                    }
                }
                break;
            case 40:
                if (Temp->ePos[Temp->oNum - 1] > 0) {
                    if (((eInfo->dwPar >> 19) & 1) == 0) {
                        SzSound_PlaySE(0, 2);
                    }
                    if (((eInfo->dwPar >> 18) & 1) == 0) {
                        SzSound_PlaySE(0, 2);
                    }
                }
                break;
            }
            return 1;
        }
        if ((eInfo->dwPar >> 25) != 0) {
            if (((eInfo->dwPar >> 26) & 1) == 1 && ((eInfo->dwPar >> 19) & 1) == 0) {
                Ta_SetUnitCurseRest((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 1);
                TaView_ReqFocusCamera(0, (unsigned short)eInfo->dwFrom, 180, 180);
                SzDuel_OpenMsgPopup(0, -120, 80, 264);
                pCall->eNO = 500;
                pCall->dwFrom = 0;
                pCall->dwTo = (unsigned short)eInfo->dwFrom;
                pCall->dwPar = 0;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            } else if (((eInfo->dwPar >> 25) & 1) == 1 && ((eInfo->dwPar >> 18) & 1) == 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, 264);
                Ta_SetUnitCurseRest((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF, 1);
                pCall->eNO = 500;
                pCall->dwFrom = 0;
                pCall->dwTo = (unsigned short)eInfo->dwTo;
                pCall->dwPar = 0;
                TaView_ReqFocusCamera(0, (unsigned short)eInfo->dwTo, 180, 180);
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            }
            Temp->wLvl = 5;
        }
        Temp->wLvl = 16;
        return 1;
    case 5:
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, 1)) < 128) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
        }
        /* MATCHING DEVICE: a statement without effect in the taken arm keeps the original's test
         * (`bnez` to the return, then a jump to the same return); both arms return 1. */
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            (void)Temp->wLvl;
        }
        break;
    default:
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, 1)) < 128) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
            return 1;
        }
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 0);
        if ((eInfo->dwFrom >> 16) != 0) {
            if ((eInfo->dwFrom >> 31) == 0) {
                TaUtil_CalcUnitAP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 1, (eInfo->dwFrom >> 16) & 0x7FFF);
                TaUtil_CalcUnitDP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 1, (eInfo->dwFrom >> 16) & 0x7FFF);
            } else {
                TaUtil_CalcUnitAP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 0, (eInfo->dwFrom >> 16) & 0x7FFF);
                TaUtil_CalcUnitDP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 0, (eInfo->dwFrom >> 16) & 0x7FFF);
            }
        }
        if ((eInfo->dwTo >> 16) != 0) {
            if ((eInfo->dwTo >> 31) == 0) {
                TaUtil_CalcUnitAP((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF, 1, (eInfo->dwTo >> 16) & 0x7FFF);
                TaUtil_CalcUnitDP((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF, 1, (eInfo->dwTo >> 16) & 0x7FFF);
            } else {
                TaUtil_CalcUnitAP((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF, 0, (eInfo->dwTo >> 16) & 0x7FFF);
                TaUtil_CalcUnitDP((eInfo->dwTo & 0xFFFF) >> 8, eInfo->dwTo & 0xFF, 0, (eInfo->dwTo >> 16) & 0x7FFF);
            }
        }
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
