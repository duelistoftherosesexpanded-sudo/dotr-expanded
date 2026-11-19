#include "dotr/ee/effect_seqs.h"

int Ta_Eff_056(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int tCount;
    int Unit;
    int i;
    int j;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwFrom); i++) {
            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwFrom, i);
            TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
        }
        TaEffect_LoadModule(Temp, 59);
        TaView_ReqStdCompleteView(16);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwFrom); j++) {
            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwFrom, j);
            if (SzDuel_GetUnitOpenRate(Unit >> 8, Unit & 0xFF) == 255) {
                tCount++;
            }
        }
        if (tCount >= 3) {
            if (TaEffect_IsLoadModule(Temp) == 1) {
                Temp->wLvl = 1;
                SzDuel_OpenMsgPopup(0, -120, 80, 261);
                pCall->eNO = 59;
                pCall->dwFrom = eInfo->dwFrom;
                pCall->dwTo = eInfo->dwTo;
                pCall->dwPar = eInfo->dwPar;
                Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            }
        }
        return 1;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwFrom); i++) {
                    Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwFrom, i);
                    Ta_DelUnit(Unit >> 8, Unit & 0xFF);
                }
            }
            return 1;
        }
        SzDuel_CloseMsgPopup();
        TaView_RestoreOriginalView(Temp, 16);
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
