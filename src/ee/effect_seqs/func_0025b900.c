#include "dotr/ee/effect_seqs.h"

int Ta_Eff_020(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int Unit;
    int eObj;
    int Ret;
    struct jp_6989c eCall;
    struct jp_6989c *pCall;
    unsigned char id;
    int stored;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    pCall = &eCall;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 17);
        TaEffect_LoadModule(Temp, 24);
        TaEffect_LoadModule(Temp, 603);
        Ta_AllUnitFade(eInfo, Temp, ID, -1, 1, 1);
        TaView_ReqFlatCompleteView(16);
        Temp->wCount_1 = 128;
        return 1;
    } else {
        if (Temp->wLvl == 0) {
            Temp->wCamTime++;
            if (Temp->wCount_1 > 63) {
                Temp->wCount_1 -= 15;
            } else {
                Temp->wCount_1 = 48;
                if (Temp->wCamTime >= 16) {
                    if (TaEffect_IsLoadModule(Temp) == 1) {
                        pCall->eNO = 17;
                        pCall->dwFrom = 0;
                        pCall->dwTo = 0;
                        pCall->dwPar = 0;
                        id = Ta_Do_Effect(pCall, Temp);
                        Temp->eID[Temp->oNum] = id;
                        /* The original tests the stored byte, zero-extended,
                         * for >= 0 (`andi; bltz`), always true; here and twice below. */
                        stored = id;
                        if (stored >= 0) {
                            Temp->oNum++;
                        }
                        Temp->wLvl++;
                    }
                }
            }
            SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        } else if (Temp->wLvl == 1) {
            if (SzEffect_IsUse(Temp->eID[0]) == 1) {
                return 1;
            }
            eObj = 0;
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo); i++) {
                Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
                if (SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xff, 4) != 0) {
                    if ((SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) >> 5) == 0) {
                        eObj = ConvUnitIDtoFlagList(eObj, Unit >> 8, Unit & 0xff);
                    }
                }
            }
            if (Ta_GetUnitNumFrom22bitFlagList(eObj) == 0) {
                eObj = eInfo->dwTo;
            }
            Temp->fObj2 = Ta_GetRandomUnit(eObj);
            TaView_ReqFocusCamera(0, Temp->fObj2, 100, 180);
            /* The unit is the last one the loop above read, and the result is not used. */
            SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xff);
            pCall->eNO = 24;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->fObj2;
            pCall->dwPar = 0;
            id = Ta_Do_Effect(pCall, Temp);
            Temp->eID[0] = id;
            stored = id;
            if (stored >= 0) {
                Temp->oNum++;
                Temp->ePos[0] = Temp->fObj2;
            }
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo); i++) {
                Ret = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
                if (Ret != Unit) {
                    Ta_UnitFade(Ret >> 8, Ret & 0xff, -1, 7, 3);
                    SzDuel_SetUnitCtlRate(Ret >> 8, Ret & 0xff, -1, 0);
                }
            }
            Temp->wLvl++;
        } else if (Temp->wLvl == 2) {
            if (SzEffect_IsUse(Temp->eID[0]) != 0) {
                if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                    if (Temp->ePos[0] >= 0) {
                        Temp->ePos[0] = -1;
                        pCall->eNO = 603;
                        pCall->dwFrom = 0;
                        pCall->dwTo = Temp->fObj2;
                        pCall->dwPar = 0;
                        id = Ta_Do_Effect(pCall, Temp);
                        Temp->eID[Temp->oNum] = id;
                        stored = id;
                        if (stored >= 0) {
                            Temp->oNum++;
                        }
                        Temp->wLvl++;
                    }
                }
            }
        } else if (Temp->wLvl == 3) {
            if (Ta_IsFinishDestEfc(eInfo, Temp, pCall) != 0) {
                Ta_AllUnitFade(eInfo, Temp, ID, 1, 0, 1);
                TaView_RestoreOriginalView(Temp, 16);
                Temp->wCamTime = 0;
                Temp->wLvl++;
            }
        } else {
            func_00199b50(data_002b86e8, SzDuel_GetUnitCtlGoalM(0, 0), SzDuel_GetUnitCtlGoalC(0, 0),
                          SzDuel_GetUnitCtlRateM(0, 0), SzDuel_GetUnitCtlRateC(0, 0));
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
        }
    }
    return 1;
}
