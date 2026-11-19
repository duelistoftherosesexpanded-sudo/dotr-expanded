#include "dotr/ee/effect_seqs.h"

int Ta_ManageDestroyEffect(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall)
{
    int stored;
    int i;
    int j;
    int Unit;
    unsigned int eObj;

    eObj = 0;
    for (i = 0; i < Temp->oNum; i++) {
        if (SzEffect_IsUse(Temp->eID[i]) == 0) {
            continue;
        }
        if (SzEffect_GetStatus(Temp->eID[i]) == 2 && Temp->ePos[i] >= 0) {
            Temp->ePos[i] = -1;
            Temp->wCount_2 = 1;
        }
    }
    if ((int)Temp->wCount_2 > 0) {
        if (Temp->wCount_2++ > 16) {
            for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo); j++) {
                Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, j);
                if (SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xFF, 4) == 0) {
                    continue;
                }
                if ((SzDuel_GetUnitKind(Unit >> 8, Unit & 0xFF) >> 5) == 0) {
                    eObj = ConvUnitIDtoFlagList(eObj, Unit >> 8, Unit & 0xFF);
                }
            }
            if (Ta_GetUnitNumFrom22bitFlagList(eObj) == 0) {
                eObj = eInfo->dwTo;
            }
            Unit = Ta_GetRandomUnit(eObj);
            SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF);
            pCall->eNO = 603;
            pCall->dwFrom = 0;
            pCall->dwTo = Unit;
            pCall->dwPar = 0;
            /* The original tests the stored byte, zero-extended, for >= 0
             * (`andi; bltz`), always true; `stored` only keeps the host build quiet. */
            if ((stored = (Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp))) >= 0) {
                Temp->oNum++;
            }
            TaView_ReqFocusCamera(0, Unit, 100, 180);
            for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo); j++) {
                i = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, j);
                if (i == Unit) {
                    continue;
                }
                Ta_UnitFade(i >> 8, i & 0xFF, -1, 7, 3);
                SzDuel_SetUnitCtlRate(i >> 8, i & 0xFF, -1, 0);
            }
            Temp->wCount_2 = 0;
            return 1;
        }
    }
    return 0;
}
