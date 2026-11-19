#include "dotr/ee/effect_seqs.h"

int Ta_Eff_054(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int eObj;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaUtil_DoOpen((eInfo->dwPar & 0xFFFF) >> 8, eInfo->dwPar & 0xFF, 1);
        TaEffect_LoadModule(Temp, 34);
        TaEffect_LoadModule(Temp, 555);
        TaEffect_LoadModule(Temp, 557);
        TaEffect_LoadModule(Temp, 602);
        TaEffect_LoadModule(Temp, 603);
        TaView_ReqFocusCamera3(0, eInfo->dwPar & 0xFFFF, 180, 180);
        Temp->wLvl = 16;
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        SzDuel_SetUnitStatus((eInfo->dwPar & 0xFFFF) >> 8, eInfo->dwPar & 0xFF, 0, 1);
        SzDuel_OpenMsgPopup(0, -120, 80,
            SzDuel_GetMsgName(SzDuel_GetUnitCardNo((eInfo->dwPar & 0xFFFF) >> 8, eInfo->dwPar & 0xFF)));
        if (eInfo->dwPar >> 16 != 49) {
            Ta_DelUnit((eInfo->dwPar & 0xFFFF) >> 8, eInfo->dwPar & 0xFF);
        }
        TaView_ReqFocusCamera(0, eInfo->dwFrom, 100, 180);
        pCall->eNO = 34;
        pCall->dwFrom = 0;
        pCall->dwTo = eInfo->dwFrom;
        pCall->dwPar = 0;
        Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
        Temp->wLvl++;
        return 1;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        switch (eInfo->dwPar >> 16) {
        case 4:
            eObj = SearchUnitByRange(0, 0, Ta_GetCurrentSide(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xFF), 2, 53, 14);
            if ((eInfo->dwTo = Ta_RemoveObjFromFlagLst2(eObj,
                Ta_IsUnitControl((eInfo->dwPar & 0xFFFF) >> 8, eInfo->dwPar & 0xFF))) == 0) {
                if (Temp->wCamTime++ > 16) {
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
                return 1;
            }
            if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
                TaView_ReqFocusCamera(0, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
            } else {
                TaView_ReqStdCompleteView(0);
            }
            Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 555);
            Temp->wLvl = 2;
            return 1;
        case 48:
            Temp->wLvl = 4;
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            break;
        case 49:
            TaView_ReqFocusCamera(0, eInfo->dwTo, 100, 180);
            pCall->eNO = 555;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 1;
            Temp->wLvl = 2;
            Temp->oNum++;
            return 1;
        default:
            break;
        }
        return 1;
    case 2:
        switch (eInfo->dwPar >> 16) {
        case 4:
            if (Ta_ManageDestroyEffect(eInfo, Temp, pCall) == 0) {
                break;
            }
            Temp->wLvl++;
            break;
        case 49:
            if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1
                && SzEffect_GetStatus(Temp->eID[Temp->oNum - 1]) == 2
                && Temp->ePos[Temp->oNum - 1] >= 0) {
                pCall->eNO = 557;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwTo;
                pCall->dwPar = 0;
                Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                Temp->ePos[Temp->oNum] = 1;
                Temp->wLvl++;
                Temp->oNum++;
            }
            break;
        default:
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    case 3:
        switch (eInfo->dwPar >> 16) {
        case 4:
            if (Ta_IsFinishDestEfc(eInfo, Temp, pCall) != 0) {
                SzDuel_CloseMsgPopup();
                TaView_RestoreOriginalView(Temp, 16);
                Temp->wLvl++;
            }
            break;
        case 49:
            for (i = 0; i < Temp->oNum; i++) {
                if (SzEffect_IsUse(Temp->eID[i]) == 1) {
                    return 1;
                }
            }
            /* MATCHING DEVICE: the target word passes through i into eObj; with one variable the
             * compiler keeps the two halves in registers instead of the word itself. */
            i = eInfo->dwTo;
            eObj = i;
            i = SzDuel_GetUnitAP((unsigned)eObj >> 8, eObj & 0xFF, 0);
            TaUtil_CalcUnitAP((unsigned)eObj >> 8, eObj & 0xFF, 1, i >> 1);
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wLvl++;
            break;
        default:
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    case 4:
        if (Temp->wCamTime++ > 16) {
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    case 16:
        if (SzDuel_GetUnitOpenRate((eInfo->dwPar & 0xFFFF) >> 8, eInfo->dwPar & 0xFF) == 255
            && TaEffect_IsLoadModule(Temp) == 1) {
            switch (eInfo->dwPar >> 16) {
            case 49:
                pCall->eNO = 602;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwPar & 0xFFFF;
                pCall->dwPar = 0;
                break;
            default:
                pCall->eNO = 602;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwPar & 0xFFFF;
                pCall->dwPar = 1;
                break;
            }
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->wLvl = 0;
            SzDuel_OpenMsgPopup(0, -120, 80, 259);
            return 1;
        }
        break;
    default:
        return 1;
    }
    return 1;
}
