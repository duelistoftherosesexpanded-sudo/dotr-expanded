#include "dotr/ee/effect_seqs.h"

int Ta_Eff_035(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int i;
    int Stack;
    int nX;
    int nZ;
    int nSide;
    int nID;
    int Unit;
    int Loc;
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    (void)eInfo;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        TaEffect_LoadModule(Temp, 35);
        TaView_ReqStdCompleteView(16);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (TaEffect_IsLoadModule(Temp) == 1) {
            pCall->eNO = 35;
            pCall->dwFrom = 0;
            pCall->dwTo = 0;
            pCall->dwPar = 0;
            Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
            Temp->ePos[Temp->oNum] = 0;
            Temp->oNum++;
            Temp->wLvl++;
        }
        break;
    default:
        if (SzEffect_IsUse(Temp->eID[Temp->oNum - 1]) == 1) {
            if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                if (Temp->ePos[0] == 0) {
                    Ta_InitMazePiece();
                    for (i = 0; i < 100; i++) {
                        nX = func_0019a5c8() % 7;
                        nZ = func_0019a5c8() % 7;
                        if ((func_0019a5c8() % 2) == 0) {
                            nSide = 0;
                        } else {
                            nSide = 1;
                        }
                        nID = func_0019a5c8() % 10 + 1;
                        Unit = SzDuel_GetMapUnitID(nX, nZ);
                        if (Unit >= 0) {
                            if (Ta_IsLDRangeAvilityWithLocation(Unit >> 8, Unit & 0xFF, nX, nZ, 11) != 0) {
                                continue;
                            }
                            if ((Unit & 0xFF) > 0) {
                                if ((Stack = Ta_GetStackMapPiece()) >= 0) {
                                    Loc = SzDuel_GetUnitLoc(nSide, nID);
                                    SzDuel_MoveUnit(nSide, nID, Stack & 0xFF, Stack >> 8);
                                    SzDuel_MoveUnit(Unit >> 8, Unit & 0xFF, Loc & 0xFF, Loc >> 8);
                                    SzDuel_MoveUnit(nSide, nID, nX, nZ);
                                }
                            }
                        } else {
                            SzDuel_MoveUnit(nSide, nID, nX, nZ);
                        }
                    }
                    Temp->ePos[Temp->oNum - 1] = 1;
                }
            }
            return 1;
        }
        if (Temp->bCamLvl == 0) {
            Temp->bCamLvl++;
            TaView_RestoreOriginalView(Temp, 16);
        } else if (Temp->wCamTime++ > 16) {
            if (SzDuel_GetWindowDisp() != 0) {
                SzDuel_SetWindowDraw(1);
            }
            SzDuel_CloseMsgPopup();
            return 0;
        }
        return 1;
    }
    return 1;
}
