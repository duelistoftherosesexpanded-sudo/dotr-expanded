#include "dotr/ee/effect_seqs.h"

int Ta_Eff_015(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    int tCount;
    unsigned char FinishCond;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
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
        TaView_ReqStdTopDownView(0);
        TaEffect_LoadModule(Temp, 15);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            for (i = 0; i < 22; i++) {
                if (((eInfo->dwTo >> (21 - i)) & 1) == 1) {
                    if (i < 11) {
                        pCall->eNO = 15;
                        pCall->dwFrom = 0;
                        pCall->dwTo = i;
                        pCall->dwPar = SzDuel_GetUnitKind(0, i) >> 5;
                        /* The original tests the stored byte, zero-extended, for >= 0
                         * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                        if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                            Temp->ePos[Temp->oNum] = i;
                            Temp->oNum++;
                        }
                    } else {
                        pCall->eNO = 15;
                        pCall->dwFrom = 0;
                        pCall->dwTo = (i - 11) | 0x100;
                        pCall->dwPar = SzDuel_GetUnitKind(1, i - 11) >> 5;
                        if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                            Temp->ePos[Temp->oNum] = (i - 11) | 0x100;
                            Temp->oNum++;
                        }
                    }
                }
            }
            Temp->wLvl++;
        }
        break;
    default:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            FinishCond = 1;
        }
        break;
    }
    switch (Temp->bCamLvl) {
    case 0:
        if (FinishCond & 1) {
            Temp->bCamLvl++;
            Temp->wCamTime = 0;
            TaView_RestoreOriginalView(Temp, 30);
        }
        break;
    default:
        if (Temp->wCamTime++ > 30) {
            FinishCond |= 2;
        }
        break;
    }
    if (FinishCond == 3) {
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
