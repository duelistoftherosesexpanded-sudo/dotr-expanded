#include "dotr/ee/effect_seqs.h"

int Ta_Eff_028(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID)
{
    int stored;
    int i;
    int tCount;
    int eObj;
    int Unit;
    struct jp_6989c eCall;
    struct jp_6989c *pCall;

    pCall = &eCall;
    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    tCount = 0;
    SzDuel_SetWindowDraw(0);
    if (ID >= 0) {
        InitEffectTemp(Temp);
        Temp->wCount_1 = 128;
        TaView_ReqStdCompleteView(16);
        TaEffect_LoadModule(Temp, 22);
        TaEffect_LoadModule(Temp, 603);
        return 1;
    }
    switch (Temp->wLvl) {
    case 0:
        if (Temp->wCount_1 > 48) {
            Temp->wCount_1 -= 15;
        } else {
            Temp->wCount_1 = 48;
            if (TaEffect_IsLoadModule(Temp) == 1) {
                pCall->eNO = 22;
                pCall->dwFrom = 0;
                pCall->dwTo = 0;
                pCall->dwPar = 0;
                /* The original tests the stored byte, zero-extended, for >= 0
                 * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
                if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                    Temp->oNum++;
                }
                Temp->wLvl++;
            }
        }
        if (Temp->wCount_1 < 48) {
            Temp->wCount_1 = 48;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        break;
    case 1:
        if (SzEffect_IsUse(Temp->eID[0]) == 1) {
            return 1;
        }
        Unit = SearchUnitByRange(0, 0, 0, 3, 52, 14);
        Ta_RemoveObjFromFlagLst2(Unit, eInfo->dwFrom);
        for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(Unit); i++) {
            eObj = Ta_GetUnitOwnerIDFromLstCount(Unit, i);
            Ta_SetUnitCurseRest(eObj >> 8, eObj & 0xFF, 0);
            SzDuel_SetUnitSpellRest(eObj >> 8, eObj & 0xFF, 0);
            TaUtil_CalcUnitAP(eObj >> 8, eObj & 0xFF, 4, 0);
            TaUtil_CalcUnitDP(eObj >> 8, eObj & 0xFF, 4, 0);
        }
        Temp->wLvl++;
        break;
    case 2:
        for (i = 0; i < 22; i++) {
            if ((eInfo->dwTo >> (21 - i) & 1) == 1) {
                if (i < 11) {
                    pCall->eNO = 603;
                    pCall->dwFrom = 0;
                    pCall->dwTo = i;
                    pCall->dwPar = 1;
                    if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                        Temp->oNum++;
                    }
                } else {
                    pCall->eNO = 603;
                    pCall->dwFrom = 0;
                    pCall->dwTo = (i - 11) | 0x100;
                    pCall->dwPar = 1;
                    if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                        Temp->oNum++;
                    }
                }
            }
        }
        Temp->wCount_1 = 0;
        Temp->wLvl++;
        /* fall through */
    case 3:
        for (i = 0; i < Temp->oNum; i++) {
            if (SzEffect_IsUse(Temp->eID[i]) == 0) {
                tCount++;
            }
        }
        if (tCount < Temp->oNum) {
            return 1;
        }
        for (i = 0; i < 22; i++) {
            if (eInfo->dwTo >> (21 - i) & 1) {
                if (i < 11) {
                    Ta_DelUnit(0, i);
                } else {
                    Ta_DelUnit(1, i - 11);
                }
            }
        }
        Temp->wLvl++;
        TaView_RestoreOriginalView(Temp, 16);
        Temp->wCamTime = 0;
        break;
    default:
        Temp->wCount_1 += 15;
        if (Temp->wCount_1 > 128) {
            Temp->wCount_1 = 128;
        }
        SzEffect_SetBlackOutForDuel((unsigned char)Temp->wCount_1);
        if (Temp->wCount_1 == 128) {
            if (Temp->wCamTime++ > 16) {
                if (SzDuel_GetWindowDisp() != 0) {
                    SzDuel_SetWindowDraw(1);
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
        }
        break;
    }
    return 1;
}
