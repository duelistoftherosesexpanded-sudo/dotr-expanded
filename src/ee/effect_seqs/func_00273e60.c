#include "dotr/ee/effect_seqs.h"

int Ta_Eff_BTL_000(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
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
        if (Ta_IsLDAbl(6, eInfo->dwTo) != 0) {
            TaEffect_LoadModule(Temp, 557);
            Temp->wLvl = 16;
        }
        if (((eInfo->dwPar >> 20) & 3) != 0 || ((eInfo->dwPar >> 22) & 3) != 0) {
            TaEffect_LoadModule(Temp, 513);
        }
        TaEffect_LoadModule(Temp, 604);
        TaEffect_LoadModule(Temp, 605);
        TaView_ReqFocusCamera(0, eInfo->dwFrom, 300, 280);
        Temp->wCount_1 = 128;
        return 1;
    }
    switch (Temp->bCamLvl) {
    case 0:
        if (Temp->wCamTime++ == 0) {
            SzSound_PlaySE(0, 39);
            SzGame_SetAutoBlur(6);
            TaView_ReqFocusCamera(12, eInfo->dwFrom, 100, 180);
            return 1;
        }
        if (Temp->wCamTime >= 12) {
            SzGame_SetAutoBlur(4);
        }
        if (Temp->wCamTime >= 24) {
            if (SzDuel_GetUnitOpenRate(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) == 255) {
                if (SzDuel_GetUnitCtlRateC(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF) == 128) {
                    if (SzEffect_IsModelStandby2(eInfo->dwFrom) != 0) {
                        SzGame_SetAutoBlur(0);
                        TaView_ReqFocusCamera(0, eInfo->dwTo << 8, 300, 280);
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
            TaView_ReqFocusCamera(12, eInfo->dwTo << 8, 100, 180);
            return 1;
        }
        if (Temp->wCamTime >= 12) {
            SzGame_SetAutoBlur(4);
        }
        if (Temp->wCamTime >= 24) {
            if (SzEffect_IsModelStandby2(eInfo->dwTo << 8) != 0) {
                SzGame_SetAutoBlur(0);
                TaView_RestoreOriginalView(Temp, 0);
                Temp->wCamTime = 0;
                Temp->bCamLvl++;
            }
        }
        return 1;
    default:
        break;
    }
    if (TaEffect_IsLoadModule(Temp) != 1) {
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            SzEffect_SetBusyFlag2(eInfo->dwFrom, 1);
            SzEffect_SetBusyFlag2(eInfo->dwTo << 8, 1);
            if (((eInfo->dwPar >> 20) & 3) != 0) {
                SzDuel_OpenMsgPopup(0, -120, 80, 260);
                pCall->eNO = 513;
                pCall->dwFrom = 0;
                pCall->dwTo = (unsigned short)eInfo->dwFrom;
                pCall->dwPar = 0;
                /* The original tests the stored byte, zero-extended, for >= 0
                 * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
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
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, -1)) > 0) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
        } else {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
            Temp->wLvl++;
        }
        return 1;
    case 3:
        temp = Ta_GetLPRevByNaturalEffect(44, (unsigned short)eInfo->dwPar);
        pCall->eNO = 604;
        pCall->dwFrom = eInfo->dwFrom;
        pCall->dwTo = 0;
        if (temp == 0) {
            temp = 0x30000;
        }
        pCall->dwPar = temp;
        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        Temp->ePos[Temp->oNum] = 0;
        Temp->oNum++;
        Temp->wLvl++;
        return 1;
    case 4:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
            if (SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2) {
                if (Temp->ePos[Temp->oNum - 1] == 1) {
                    SzSound_PlaySE(1, 65);
                }
                Temp->ePos[Temp->oNum - 1]++;
            }
            return 1;
        }
        pCall->eNO = 605;
        pCall->dwFrom = eInfo->dwFrom;
        pCall->dwTo = 0;
        pCall->dwPar = 0;
        Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        Temp->oNum++;
        SzDuel_CalcLP(eInfo->dwTo, 1, Ta_GetLPRevByNaturalEffect(44, (unsigned short)eInfo->dwPar));
        if (SzDuel_GetLP(eInfo->dwTo) > 0) {
            SzData_CalcExp(eInfo->dwFrom >> 8, SzDuel_GetUnitCardNo(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF), 0, 20);
        } else {
            SzData_CalcExp(eInfo->dwFrom >> 8, SzDuel_GetUnitCardNo(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF), 0, 24);
        }
        SzData_CalcExp(eInfo->dwTo, SzDuel_GetUnitCardNo(eInfo->dwTo, 0), 0, -8);
        Temp->wLvl++;
        return 1;
    case 5:
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, 1)) < 128) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
        }
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) != 0) {
            if (Temp->fObj2++ == 20) {
                SzSound_PlaySE(0, 2);
            }
            return 1;
        }
        Temp->wLvl++;
        break;
    case 16:
        SzDuel_OpenMsgPopup(0, -120, 80, 270);
        pCall->eNO = 557;
        pCall->dwFrom = 0;
        pCall->dwTo = eInfo->dwFrom;
        pCall->dwPar = 0;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->wLvl++;
        break;
    case 17:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            return 1;
        }
        SzDuel_CloseMsgPopup();
        Temp->wLvl = 0;
        break;
    default:
        if ((Temp->wCount_1 = Ta_CalcCtrlRate(Temp->wCount_1, 15, 1)) < 128) {
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
            SzEffect_SetBlackOutForMonster((unsigned char)Temp->wCount_1);
            return 1;
        }
        if ((eInfo->dwFrom >> 16) != 0) {
            if ((eInfo->dwFrom >> 31) == 0) {
                TaUtil_CalcUnitAP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 1, (eInfo->dwFrom >> 16) & 0x7FFF);
                TaUtil_CalcUnitDP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 1, (eInfo->dwFrom >> 16) & 0x7FFF);
            } else {
                TaUtil_CalcUnitAP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 0, (eInfo->dwFrom >> 16) & 0x7FFF);
                TaUtil_CalcUnitDP((eInfo->dwFrom & 0xFFFF) >> 8, eInfo->dwFrom & 0xFF, 0, (eInfo->dwFrom >> 16) & 0x7FFF);
            }
        }
        SzEffect_SetBlackOutForDuel(128);
        SzEffect_SetBlackOutForMonster(128);
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
