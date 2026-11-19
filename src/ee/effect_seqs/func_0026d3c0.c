#include "dotr/ee/effect_seqs.h"

/* Larger and smaller of two values; the distance below is spelled with both, as the code computes both. */
#define GRAV_MAX(a, b) ((a) > (b) ? (a) : (b))
#define GRAV_MIN(a, b) ((a) < (b) ? (a) : (b))
int Ta_SetGravityEffect(struct jp_6989c *eInfo, struct jp_69942 *Temp, int Dist)
{
    int Reserve[22];
    struct jp_6989c eCall;
    struct jp_6989c *pCall = &eCall;
    int eObj;
    int Unit;
    int Loc;
    int Num;
    int i;

    eCall.eNO = 999;
    eCall.dwFrom = 0;
    eCall.dwTo = 0;
    eCall.dwPar = 0;
    for (i = 0; i < 22; i++) {
        Reserve[i] = -1;
    }
    Num = Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo);
    for (i = 0, eObj = 0; i < Num; i++) {
        Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
        Loc = SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xFF);
        if (Dist == (int)((GRAV_MAX(eInfo->dwFrom & 0xFF, (unsigned)(Loc & 0xFF)) - GRAV_MIN(eInfo->dwFrom & 0xFF, (unsigned)(Loc & 0xFF))) + (GRAV_MAX(eInfo->dwFrom >> 8, (unsigned)(Loc >> 8)) - GRAV_MIN(eInfo->dwFrom >> 8, (unsigned)(Loc >> 8))))) {
            eObj = ConvUnitIDtoFlagList(eObj, Unit >> 8, Unit & 0xFF);
        }
    }
    Num = Ta_GetUnitNumFrom22bitFlagList(eObj);
    for (i = 0; i < Num; i++) {
        Unit = Ta_GetMinimumOrderUnitFromFlagList2(eObj, eInfo->dwPar);
        if (Unit >= 0) {
            eObj = ConvUnitIDtoFlagList_OFF(eObj, Unit >> 8, Unit & 0xFF);
            if ((Loc = Ta_RequestGravityReserve(eInfo->dwFrom & 0xFF, eInfo->dwFrom >> 8, Unit, Reserve)) >= 0) {
                Reserve[i] = Loc;
                pCall->eNO = 572;
                pCall->dwFrom = 0;
                pCall->dwTo = Unit;
                pCall->dwPar = Loc;
                Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp);
                Temp->ePos[Temp->oNum] = (Unit << 16) | Loc;
                Temp->oNum++;
            }
        }
    }
    return Num;
}
