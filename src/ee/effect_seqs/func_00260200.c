#include "dotr/ee/effect_seqs.h"

int Ta_Eff_ETC(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 513);
    } else switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            Temp->wLvl = Temp->wLvl + 1;
            SzDuel_OpenMsgPopup(0, -120, 80, 260);
            pCall->eNO = 513;
            pCall->dwFrom = 0;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
        }
        break;
    default:
        if (SzEffect_IsUse(Temp->eID[0]) == 0) {
            SzDuel_CloseMsgPopup();
            return 0;
        }
        break;
    }
    return 1;
}
