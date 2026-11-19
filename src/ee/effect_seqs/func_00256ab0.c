#include "dotr/ee/effect_seqs.h"

int Ta_Eff_003B(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
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
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Ta_AllUnitFade2(eInfo, Temp, ID, -1, 1, 3);
        TaView_ReqStdCompleteView(30);
        TaEffect_LoadModule(Temp, 503);
        TaEffect_LoadModule(Temp, 502);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCamTime++ > 30) {
            if (TaEffect_IsLoadModule(Temp) == 1) {
                pCall->eNO = 503;
                pCall->dwFrom = 1;
                pCall->dwTo = eInfo->dwTo;
                pCall->dwPar = 8;
                /* The original tests the stored byte, zero-extended, for >= 0
                 * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                    Temp->ePos[Temp->oNum] = eInfo->dwTo;
                    Temp->oNum++;
                }
                pCall->eNO = 502;
                pCall->dwFrom = 0;
                pCall->dwTo = eInfo->dwFrom;
                pCall->dwPar = 7;
                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                    Temp->ePos[Temp->oNum] = eInfo->dwFrom;
                    Temp->oNum++;
                }
                Temp->wLvl++;
            }
        }
        return 1;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) != 0) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    if ((unsigned short)Temp->ePos[i] != 0xFFFF) {
                        Temp->ePos[i] = 0xFFFF;
                    }
                }
            } else {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            Temp->wLvl++;
            TaView_RestoreOriginalView(Temp, 30);
            Temp->wCamTime = 0;
            Ta_AllUnitFade2(eInfo, Temp, ID, 1, 1, 3);
        }
        return 1;
    default:
        if (Temp->wCamTime++ > 30) {
            if (Ta_IsFinishFade() == 1) {
                return 0;
            }
        }
        break;
    }
    return 1;
}
