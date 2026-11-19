#include "dotr/ee/effect_seqs.h"

int Ta_Eff_032(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int j;
    int tCount;
    int nX;
    int nZ;
    int LOC;
    int uID;
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
            TaView_ReqFocusCamera(0, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
        } else {
            TaView_ReqStdCompleteView(0);
        }
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        TaEffect_LoadModule(Temp, 531);
        TaEffect_LoadModule(Temp, 532);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 531);
                Temp->wLvl++;
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
        if (tCount < Temp->oNum) {
            return 1;
        }
        for (i = 0; i < Temp->oNum; i++) {
            if (Temp->ePos[i] >= 0) {
                LOC = SzDuel_GetUnitLoc(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF);
                nX = LOC >> 8;
                nZ = LOC & 0xFF;
                if ((uID = SzDuel_GetMapUnitID(nX, nZ)) < 0) {
                    SzDuel_MoveUnit(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, nX, nZ);
                } else if (uID & 0xFF) {
                    if (Ta_IsLDRangeAvilityWithLocation(uID >> 8, uID & 0xFF, nX, nZ, 11) == 0) {
                        Ta_SwapUnit(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF, uID >> 8, uID & 0xFF);
                        for (j = 0; j < Temp->oNum; j++) {
                            if (uID == Temp->ePos[j]) {
                                Temp->ePos[j] = -1;
                            }
                        }
                    }
                }
            }
        }
        Ta_SetEffectDependOnFlagLst(eInfo, Temp, pCall, 532);
        Temp->wLvl++;
        break;
    case 2:
        if (Temp->wCamTime == 0) {
            if (Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo) == 1) {
                TaView_ReqFocusCamera(30, Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, 0), 100, 180);
            } else {
                TaView_ReqStdCompleteView(16);
            }
        }
        if (Temp->wCamTime++ >= 24) {
            for (i = 0; i < Temp->oNum; i++) {
                if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                    tCount++;
                }
            }
            if (tCount < Temp->oNum) {
                return 1;
            }
            Temp->wLvl++;
            TaView_RestoreOriginalView(Temp, 16);
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
