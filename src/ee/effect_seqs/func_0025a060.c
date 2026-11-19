#include "dotr/ee/effect_seqs.h"

int Ta_Eff_013(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    int i;
    int Num;
    int tCount;
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
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
            TaView_ReqFocusCamera(0, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(16);
        }
        TaEffect_LoadModule(Temp, 513);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 63) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                if (Temp->wCamTime++ > 16) {
                    if ((eInfo->dwFrom == 2 || eInfo->dwFrom == 4) && eInfo->dwPar == 0) {
                        Num = Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo);
                        for (i = 0; i < Num; i++) {
                            Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
                            pCall->eNO = 513;
                            pCall->dwFrom = 0;
                            pCall->dwTo = Unit;
                            pCall->dwPar = 1;
                            /* The original tests the stored byte, zero-extended, for >= 0
                             * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                            if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                                Temp->ePos[Temp->oNum] = Unit;
                                Temp->oNum++;
                            }
                        }
                    } else {
                        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 513);
                    }
                    Temp->wLvl++;
                    Temp->wCamTime = 0;
                }
            }
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i])) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if (Temp->ePos[i] >= 0) {
                        SzEffect_SetBusyFlag2(Temp->ePos[i], 0);
                        if (eInfo->dwFrom == 3 || eInfo->dwFrom == 5) {
                            SzDuel_SetUnitStatus(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, 5, eInfo->dwPar);
                            SzDuel_SetUnitStatus(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, 3, eInfo->dwPar);
                        }
                        if (eInfo->dwFrom == 2 || eInfo->dwFrom == 4) {
                            TaUtil_DoOpen(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, eInfo->dwPar);
                        }
                        Temp->ePos[i] = -1;
                    }
                }
            } else {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            if (Temp->wCamTime++ > 16) {
                TaView_RestoreOriginalView(Temp, 16);
                Temp->wCamTime = 0;
                Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
                Temp->wLvl++;
            }
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
                    SzDuel_CloseMsgPopup();
                    if (SzDuel_GetWindowDisp() != 0) {
                        SzDuel_SetWindowDraw(1);
                    }
                    return 0;
                }
            }
        }
        break;
    }
    return 1;
}
