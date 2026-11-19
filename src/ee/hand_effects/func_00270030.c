#include "dotr/ee/hand_effects.h"

int Ta_Eff_Hand_Fusion(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID)
{
    struct jp_8b329 eCall;
    struct jp_8b329 *pCall = &eCall;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 600);
        pCall->eNO = 602;
        pCall->dwFrom = eInfo->dwFrom;
        pCall->dwTo = eInfo->dwTo;
        pCall->dwPar = 0;
        TaLib_c615(&Temp->nTasakiEfc, pCall, 0);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 600;
            pCall->dwFrom = eInfo->dwFrom;
            pCall->dwTo = eInfo->dwTo;
            pCall->dwPar = 0;
            Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[0] = 0;
            Temp->wLvl++;
        }
        pCall->eNO = 602;
        pCall->dwFrom = eInfo->dwFrom;
        pCall->dwTo = eInfo->dwTo;
        pCall->dwPar = 0;
        TaLib_c615(&Temp->nTasakiEfc, pCall, -1);
        break;
    default:
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            switch (Temp->fObj2++) {
            case 1:
                SzSound_PlaySE(1, 76);
                break;
            case 18:
                SzSound_PlaySE(1, 58);
                break;
            case 80:
                SzSound_PlaySE(1, 59);
                break;
            }
            if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                if (Temp->ePos[0] == 0) {
                    Temp->ePos[0] = 1;
                    Ta_DelUnit2(eInfo->dwFrom >> 8, eInfo->dwFrom & 0xff);
                    Ta_DelUnit2(eInfo->dwTo >> 8, eInfo->dwTo & 0xff);
                    return eInfo->dwPar << 16 | 2;
                }
            }
            return 1;
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
