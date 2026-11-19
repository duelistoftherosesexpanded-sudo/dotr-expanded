#include "dotr/ee/effect_seqs.h"

int Ta_Eff_112(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int tCount;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int i;
    int Unit;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        Temp->wCount_1 = 128;
        TaEffect_LoadModule(Temp, 612);
        for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo); i++) {
            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
            SzEffect_SetBusyFlag2(Unit, 0);
            TaUtil_DoOpen(Unit >> 8, Unit & 0xFF, 1);
        }
        TaView_ReqStdCompleteView(30);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo); i++) {
                if (SzEffect_IsModelStandby2(Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i)) != 0) {
                    tCount++;
                }
            }
            if (tCount == Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo)) {
                if (TaEffect_IsLoadModule(Temp) == 1) {
                    if (Temp->wCamTime++ >= 46) {
                        if (tCount == 1) {
                            TaView_ReqFocusCamera(0, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
                        }
                        SzDuel_OpenMsgPopup(0, -120, 80, 276);
                        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 612);
                        Temp->wCamTime = 0;
                        Temp->wLvl++;
                    }
                }
            }
            Temp->wCount_1 = 48;
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1) {
                TaEffect_SetOuchMotion((unsigned short)Temp->ePos[i]);
                if ((Temp->ePos[i] >> 16) == 0) {
                    if (Temp->wCamTime++ >= 32) {
                        Ta_UnitFade((Temp->ePos[i] & 0xFFFF) >> 8, Temp->ePos[i] & 0xFF, -1, 1, 3);
                        Temp->ePos[i] |= 0x10000;
                    }
                }
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if (Temp->ePos[i] >= 0) {
                        SzEffect_SetBusyFlag2(Temp->ePos[i] & 0xFFFF, 1);
                        Ta_DelUnit((Temp->ePos[i] & 0xFFFF) >> 8, Temp->ePos[i] & 0xFF);
                        Temp->ePos[i] = -1;
                    }
                }
            } else {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
            Temp->wCamTime = 0;
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wLvl++;
        }
        break;
    default:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            if (Temp->wCamTime++ > 16) {
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
