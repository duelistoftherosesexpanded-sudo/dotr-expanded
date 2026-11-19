#include "dotr/ee/effect_seqs.h"

int Ta_Eff_102(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaUtil_DoOpen(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 1);
        TaView_ReqFocusCamera(0, eInfo->dwTo, 180, 180);
        TaEffect_LoadModule(Temp, 602);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (SzDuel_GetUnitOpenRate(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF) == 255) {
            if (TaEffect_IsLoadModule(Temp) == 1) {
                if (eInfo->dwFrom == 1) {
                    SzDuel_OpenMsgPopup(0, -120, 80, SzDuel_GetMsgName(SzDuel_GetUnitCardNo(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF)));
                } else {
                    SzDuel_OpenMsgPopup(0, -120, 80, 269);
                }
                pCall->eNO = 602;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwTo;
                pCall->dwPar = 0;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                Temp->wLvl++;
            }
        }
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            return 1;
        }
        SzDuel_SetUnitStatus(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF, 0, 1);
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
        if (eInfo->dwPar != 0) {
            Ta_DelUnit(eInfo->dwTo >> 8, eInfo->dwTo & 0xFF);
        } else {
            SzEffect_SetBusyFlag2(eInfo->dwTo, 0);
        }
        Temp->wLvl++;
        return 1;
    default:
        if (Temp->wCamTime++ > 16) {
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    }
    return 1;
}
