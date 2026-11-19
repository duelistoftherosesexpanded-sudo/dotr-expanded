#include "dotr/ee/effect_seqs.h"

int Ta_Eff_017_MEGUMI(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    int i;
    int tCount;
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
        TaView_ReqStdCompleteView(16);
        TaEffect_LoadModule(Temp, 20);
        TaEffect_LoadModule(Temp, 529);
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
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
                    SzDuel_OpenMsgPopup(0, -120, 80, 262);
                    pCall->eNO = 20;
                    pCall->dwFrom = 0;
                    pCall->dwTo = 0;
                    pCall->dwPar = 0;
                    /* The original tests the stored byte, zero-extended, for >= 0
                     * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                    if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                        Temp->oNum++;
                    }
                    Temp->wLvl++;
                    Temp->wCamTime = 0;
                }
            }
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 529);
        Temp->wLvl++;
        return 1;
    case 2:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount < Temp->oNum) {
            return 1;
        }
        SzDuel_CloseMsgPopup();
        for (i = 0; i < 22; i++) {
            if (eInfo->dwTo >> (21 - i) & 1) {
                if (i < 11) {
                    TaUtil_CalcUnitDP(0, i, eInfo->dwPar >> 16, eInfo->dwPar & 0xFFFF);
                    if (eInfo->dwFrom == 1) {
                        TaUtil_CalcUnitAP(0, i, eInfo->dwPar >> 16, eInfo->dwPar & 0xFFFF);
                    }
                } else {
                    TaUtil_CalcUnitDP(1, i - 11, eInfo->dwPar >> 16, eInfo->dwPar & 0xFFFF);
                    if (eInfo->dwFrom == 1) {
                        TaUtil_CalcUnitAP(1, i - 11, eInfo->dwPar >> 16, eInfo->dwPar & 0xFFFF);
                    }
                }
            }
        }
        Temp->wLvl++;
        break;
    case 3:
        if (Temp->wCamTime++ > 16) {
            Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
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
