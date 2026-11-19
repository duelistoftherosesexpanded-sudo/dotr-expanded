#include "dotr/ee/effect_seqs.h"

int Ta_Eff_072(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 128;
        for (i = 0; i < 11; i++) {
            SzEffect_SetBusyFlag2(i, 1);
            SzEffect_SetBusyFlag2(i | 0x100, 1);
        }
        TaView_ReqFocusCamera(30, eInfo->dwFrom, 100, 180);
        TaEffect_LoadModule(Temp, 513);
        TaEffect_LoadModule(Temp, 572);
        TaView_ReqStdCompleteView(30);
        Temp->wCount_2 = 2;
        SzGame_SetAutoBlur(4);
    } else switch (Temp->wLvl) {
    case 0:
        Temp->wCamTime++;
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (Temp->wCamTime >= 30) {
                if (TaEffect_IsLoadModule(Temp) == 1) {
                    pCall->eNO = 513;
                    pCall->dwFrom = 0;
                    pCall->dwTo = eInfo->dwFrom;
                    pCall->dwPar = 0;
                    Temp->eID[0] = Ta_Do_Effect(pCall, Temp);
                    Temp->ePos[0] = eInfo->dwFrom;
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
        if (SzEffect_IsUse(Temp->eID[0]) != 0) {
            return 1;
        }
        TaView_ReqStdCompleteView(0);
        Temp->wLvl++;
        break;
    case 2:
        Temp->oNum = 0;
        if (Temp->wCount_2 >= 14) {
            TaView_RestoreOriginalView(Temp, 16);
            SzGame_SetAutoBlur(0);
            Temp->wCamTime = 0;
            Temp->wLvl = 4;
            break;
        }
        if (Ta_SetGravityEffect(eInfo, Temp, Temp->wCount_2) > 0) {
            Temp->wLvl++;
            break;
        }
        Temp->wCount_2++;
        break;
    case 3:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) != 0) {
                return 1;
            }
        }
        for (i = 0; i < Temp->oNum; i++) {
            SzDuel_MoveUnit(Temp->ePos[i] >> 24, (Temp->ePos[i] >> 16) & 0xFF, Temp->ePos[i] & 0xFF,
                (Temp->ePos[i] & 0xFFFF) >> 8);
        }
        if (Temp->wCount_2 >= 14) {
            Temp->wLvl++;
            break;
        }
        Temp->wLvl = 2;
        Temp->wCount_2++;
        return 1;
    case 4:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            if (Temp->wCamTime++ > 16) {
                for (i = 0; i < 11; i++) {
                    SzEffect_SetBusyFlag2(i, 0);
                    SzEffect_SetBusyFlag2(i | 0x100, 0);
                }
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
        }
        break;
    default:
        break;
    }
    return 1;
}
