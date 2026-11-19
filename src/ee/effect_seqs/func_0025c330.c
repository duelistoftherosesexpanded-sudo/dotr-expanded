#include "dotr/ee/effect_seqs.h"

int Ta_Eff_025(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    struct jp_6989c eCall;
    int tCount;
    int Self;
    int Loc;
    int Unit;
    unsigned char FinishCond;
    int i;
    struct jp_6989c *pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    FinishCond = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        for (i = 0; i < 10; i++) {
            Temp->ePos[i] = ((func_0019a5c8() % 7) << 8) | (func_0019a5c8() % 7);
        }
        Temp->wCount_1 = 128;
        TaEffect_LoadModule(Temp, 525);
        TaEffect_LoadModule(Temp, 603);
        return 1;
    }
    switch (Temp->bCamLvl) {
    case 0:
        if (Temp->wLvl > 0) {
            if (Temp->wCamTime++ == 0) {
                TaView_ReqStdCompleteView(16);
            } else if (Temp->wCamTime >= 16) {
                Temp->wCamTime = 0;
                Temp->bCamLvl++;
            }
        }
        break;
    case 1:
        if (Temp->wLvl < 2) {
            if (Temp->wCamTime++ == 0) {
                TaView_ReqRotCamera(60, 16383);
            } else if (Temp->wCamTime >= 60) {
                Temp->wCamTime = 0;
            }
        } else {
            SzDuel_CloseMsgPopup();
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
            Temp->bCamLvl++;
        }
        break;
    default:
        if (Temp->wCamTime++ >= 16) {
            FinishCond = 2;
        }
        break;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                TaView_ReqSkyView();
                Temp->wLvl++;
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        if (Temp->wCount_1 % 8 == 0 && Temp->wCount_2 < 10) {
            pCall->eNO = 525;
            pCall->dwFrom = 0;
            pCall->dwTo = Temp->ePos[Temp->wCount_2];
            pCall->dwPar = 0;
            /* The original tests the stored byte, zero-extended, for >= 0
             * (`andi; bltz`), always true, here and below; `stored` only keeps the host build quiet. */
            if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                Temp->oNum++;
            }
            for (i = 0; i < Temp->wCount_2; i++) {
                if (Temp->ePos[Temp->wCount_2] == Temp->ePos[i]) {
                    Temp->ePos[i] = Temp->ePos[i] | 0x10000;
                }
            }
            Temp->wCount_2++;
        }
        for (i = 0; i < Temp->wCount_2; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 1 && SzEffect_GetStatus(Temp->eID[i]) == 2
                && (Temp->ePos[i] >> 16) == 0) {
                if ((Unit = SzDuel_GetMapUnitID(Temp->ePos[i] & 0xFF, Temp->ePos[i] >> 8)) >= 0
                    && (Unit & 0xFF) > 0) {
                    Self = Ta_GetCurrentSide(Unit >> 8, Unit & 0xFF);
                    if (Ta_IsLDRangeAvility(Unit >> 8, Unit & 0xFF, 11)) {
                        SzDuel_OpenMsgPopup(0, -120, 80, 270);
                        Loc = SzDuel_GetUnitLoc(Self, 0);
                        Ta_SetRangeVisual(Loc & 0xFF, Loc >> 8, 0, Ta_IsLDAbl(9, Self), 1, 1, 11);
                    } else if (Ta_IsResistAll(Unit >> 8, Unit & 0xFF) && (unsigned int)Self != eInfo->dwPar) {
                        SzDuel_OpenMsgPopup(0, 0, 0, SzDuel_GetMsgName(83));
                    } else {
                        pCall->eNO = 603;
                        pCall->dwFrom = 0;
                        pCall->dwTo = Unit;
                        pCall->dwPar = 1;
                        if ((stored = (Temp->eID[i + 10] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                            Temp->ePos[i + 10] = Unit;
                        }
                    }
                }
                Temp->ePos[i] |= 0x10000;
            }
        }
        for (i = 0; i < 10; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0 && SzEffect_IsUse(Temp->eID[i + 10]) == 0
                && Temp->ePos[i + 10] > 0) {
                Ta_DelUnit(Temp->ePos[i + 10] >> 8, Temp->ePos[i + 10] & 0xFF);
                Temp->ePos[i + 10] = 0;
            }
            if (Temp->wCount_2 > 0 && SzEffect_IsUse(Temp->eID[i]) == 0 && SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount >= 10) {
            Temp->wCount_1 = 48;
            Temp->wLvl++;
        } else {
            for (i = 10; i < 20; i++) {
                if (Temp->ePos[i] > 0) {
                    Ta_DelUnit(Temp->ePos[i] >> 8, Temp->ePos[i] & 0xFF);
                }
            }
            Temp->wCount_1++;
        }
        break;
    default:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            FinishCond |= 1;
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
