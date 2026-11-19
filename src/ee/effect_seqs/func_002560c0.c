#include "dotr/ee/effect_seqs.h"

int Ta_Eff_000(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int tCount;
    unsigned char FinishCond;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    FinishCond = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo);
        Temp->wCount_1 = 128;
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        TaEffect_LoadModule(Temp, 500);
        if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
            TaView_ReqFocusCamera(32, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(32);
        }
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (Temp->wCamTime++ >= 48) {
                if (!Temp->eMod.fLoad) {
                    if (TaEffect_IsLoadModule(Temp) == 1) {
                        Temp->eMod.fLoad = -1;
                    }
                } else {
                    if (eInfo->dwPar != 0) {
                        SzDuel_OpenMsgPopup(TaUtil_GetObjSideNum(eInfo->dwTo), -120, 80, 264);
                    } else {
                        SzDuel_OpenMsgPopup(TaUtil_GetObjSideNum(eInfo->dwTo), -120, 80, 280);
                    }
                    Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 500);
                    Temp->wLvl++;
                }
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            SzDuel_CloseMsgPopup();
            for (tCount = 0; tCount < Temp->oNum; tCount++) {
                if (eInfo->dwFrom == 1) {
                    SzDuel_SetUnitSpellRest(Temp->ePos[tCount] >> 8, Temp->ePos[tCount] & 0xFF, 255);
                } else {
                    Ta_SetUnitCurseRest(Temp->ePos[tCount] >> 8, Temp->ePos[tCount] & 0xFF, eInfo->dwPar);
                    if (eInfo->dwPar != 0) {
                        SzDuel_SetUnitStatus(Temp->ePos[tCount] >> 8, Temp->ePos[tCount] & 0xFF, 0, 1);
                    }
                }
            }
            Temp->wLvl++;
            Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
        }
        break;
    default:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            FinishCond = 1;
        }
        break;
    }
    switch (Temp->bCamLvl) {
    case 0:
        if (FinishCond & 1) {
            if (Temp->wCamTime++ > 16) {
                Temp->bCamLvl++;
                Temp->wCamTime = 0;
                TaView_RestoreOriginalView(Temp, 32);
            }
        }
        break;
    default:
        if (Temp->wCamTime++ > 32) {
            FinishCond |= 2;
        }
        break;
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
