#include "dotr/ee/effect_seqs.h"

int Ta_Eff_066(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
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
        if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
            TaView_ReqFocusCamera(30, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(30);
        }
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        TaEffect_LoadModule(Temp, 513);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        Temp->wCamTime++;
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                if (Temp->wCamTime >= 30) {
                    if (eInfo->dwFrom == 54) {
                        SzDuel_OpenMsgPopup(0, -120, 80, 272);
                    } else {
                        SzDuel_OpenMsgPopup(0, -120, 80, 273);
                    }
                    Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 513);
                    Temp->wLvl++;
                    Temp->wCamTime = 0;
                }
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        return 1;
    case 1:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1) {
                if (SzEffect_GetStatus(Temp->eID[i]) == 2 && (unsigned short)Temp->ePos[i] != 0xFFFF &&
                    SzEffect_GetStatus(Temp->eID[i]) == 2) {
                    switch (eInfo->dwFrom) {
                    case 54:
                        SzDuel_SetUnitKind(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, eInfo->dwPar);
                        break;
                    case 55:
                        SzDuel_SetUnitAttr(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, eInfo->dwPar);
                        break;
                    }
                    Temp->ePos[i] = 0xFFFF;
                }
            } else {
                tCount++;
            }
        }
        if (tCount >= Temp->oNum) {
            SzDuel_CloseMsgPopup();
            Temp->wLvl++;
            TaView_RestoreOriginalView(Temp, 16);
            Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
        }
        return 1;
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
