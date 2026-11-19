#include "dotr/ee/effect_seqs.h"

int Ta_Eff_001(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    struct jp_6a917 *pVarInfo;
    struct jp_6989c *pCall;
    struct jp_6989c eCall;
    int i;
    int Num;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    pCall = &eCall;
    if (ID >= 0) {
        InitEffectTemp(Temp);
        if (eInfo->dwPar == 999) {
            SzDuel_CloseMsgPopup();
            return 0;
        }
        switch (TaHand_GetCardKind(eInfo->dwPar & 0xFFFF) >> 5) {
        case 0:
            if (SzDuel_GetUnitMonsterCount(eInfo->dwFrom & 0xFFFF) >= 5) {
                eInfo->dwPar = 999;
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        default:
            if (SzDuel_GetUnitSpellCount(eInfo->dwFrom & 0xFFFF) >= 5) {
                eInfo->dwPar = 999;
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        }
        SzDuel_SetWindowDraw(0);
        TaEffect_LoadModule(Temp, 501);
        TaView_ReqFocusCamera2(16, eInfo->dwTo, 180);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (eInfo->dwPar == 999) {
            SzDuel_CloseMsgPopup();
            return 0;
        }
        switch (TaHand_GetCardKind(eInfo->dwPar & 0xFFFF) >> 5) {
        case 0:
            if (SzDuel_GetUnitMonsterCount(eInfo->dwFrom & 0xFFFF) >= 5) {
                if (SzDuel_GetWindowDisp()) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        default:
            if (SzDuel_GetUnitSpellCount(eInfo->dwFrom & 0xFFFF) >= 5) {
                if (SzDuel_GetWindowDisp()) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        }
        SzDuel_SetWindowDraw(0);
        if (TaEffect_IsLoadModule(Temp) != 1) {
            return 1;
        }
        Temp->wLvl++;
        break;
    case 1:
        SzDuel_SetWindowDraw(0);
        pCall->eNO = 501;
        pCall->dwFrom = 0;
        pCall->dwTo = eInfo->dwTo;
        pCall->dwPar = 0;
        /* Host build: `+ 0` only keeps GCC's -Wtype-limits quiet; the original
         * tests the stored unsigned byte for >= 0, which is always true. */
        if ((Temp->eID[0] = Ta_Do_Effect(pCall, Temp)) + 0 >= 0) {
            Temp->oNum++;
        }
        Temp->wLvl++;
        break;
    case 2:
        SzDuel_SetWindowDraw(0);
        if (SzEffect_IsUse(Temp->eID[0])) {
            if (SzEffect_GetStatus(Temp->eID[0]) == 2) {
                /* Host build: `+ 0` only keeps GCC's -Wtype-limits quiet; the
                 * original compares the low byte of ePos[0] with 0xFFFF, which is always true. */
                if ((unsigned char)Temp->ePos[0] + 0 != 0xFFFF) {
                    if (eInfo->dwPar == 999) {
                        pVarInfo = SzDuel_AddUnit(eInfo->dwFrom & 0xFFFF, eInfo->dwTo & 0xFF, eInfo->dwTo >> 8,
                                                  SzDuel_InqGrave(eInfo->dwFrom & 0xFFFF,
                                                                  SzDuel_GetGraveCount(eInfo->dwFrom & 0xFFFF) - 1),
                                                  0);
                        if (pVarInfo) {
                            if (pVarInfo->bVarKind >> 5) {
                                pVarInfo->fFinish = -1;
                            }
                            if (((eInfo->dwFrom >> 16) & 1) == 1) {
                                Temp->fObj = (pVarInfo->bOwner << 8) | pVarInfo->bID;
                            }
                        }
                        if ((eInfo->dwFrom >> 17) & 1) {
                            SzDuel_DelGrave(eInfo->dwPar >> 16, SzDuel_GetGraveCount(eInfo->dwFrom & 0xFFFF) - 1);
                            SzDuel_PackGrave(eInfo->dwPar >> 16);
                        }
                    } else {
                        pVarInfo = SzDuel_AddUnit(eInfo->dwFrom & 0xFFFF, eInfo->dwTo & 0xFF, eInfo->dwTo >> 8,
                                                  eInfo->dwPar & 0xFFFF, 0);
                        if (pVarInfo) {
                            if (pVarInfo->bVarKind >> 5) {
                                pVarInfo->fFinish = -1;
                            }
                            if (((eInfo->dwFrom >> 16) & 1) == 1) {
                                Temp->fObj = (pVarInfo->bOwner << 8) | pVarInfo->bID;
                            }
                        }
                        if ((eInfo->dwFrom >> 17) & 1) {
                            Num = SzDuel_GetGraveCount(eInfo->dwPar >> 16);
                            for (i = 0; i < Num; i++) {
                                if ((eInfo->dwPar & 0xFFFF) == (unsigned)SzDuel_InqGrave(eInfo->dwPar >> 16, i)) {
                                    SzDuel_DelGrave(eInfo->dwPar >> 16, i);
                                    i = Num;
                                }
                            }
                            SzDuel_PackGrave(eInfo->dwPar >> 16);
                        }
                    }
                    Temp->ePos[0] = 0xFFFF;
                }
            }
            return 1;
        }
        Temp->wLvl++;
        break;
    case 3:
        SzDuel_SetWindowDraw(0);
        if (Temp->fObj) {
            TaUtil_DoOpen(Temp->fObj >> 8, Temp->fObj & 0xFF, 1);
            Temp->wLvl++;
        } else {
            Temp->wLvl++;
        }
        break;
    default:
        SzDuel_SetWindowDraw(0);
        switch (Temp->bCamLvl) {
        case 0:
            TaView_RestoreOriginalView(Temp, 16);
            Temp->wCamTime = 0;
            Temp->bCamLvl++;
            break;
        default:
            if (Temp->wCamTime++ > 16) {
                if (SzDuel_GetWindowDisp()) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        }
        break;
    }
    return 1;
}
