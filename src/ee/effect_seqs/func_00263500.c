#include "dotr/ee/effect_seqs.h"

int Ta_Eff_063(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int LOC;
    int tCount;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 560);
        if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
            TaView_ReqFocusCamera(30, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(30);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            if (Temp->wCamTime++ >= 30) {
                Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 560);
                Temp->wLvl++;
            }
        }
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if ((Temp->ePos[i] & 0xFFFF) != 0xFFFF) {
                        LOC = SzDuel_GetUnitLoc(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF);
                        Ta_DelUnit(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF);
                        SzDuel_AddUnit(Temp->ePos[i] >> 8, LOC & 0xFF, LOC >> 8, eInfo->dwPar, 0);
                        Temp->ePos[i] = 0xFFFF;
                    }
                }
            } else {
                tCount++;
            }
        }
        if (tCount < Temp->oNum) {
            return 1;
        }
        Temp->wLvl++;
        Temp->wCamTime = 0;
        TaView_RestoreOriginalView(Temp, 16);
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
