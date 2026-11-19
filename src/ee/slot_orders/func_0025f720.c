#include "dotr/ee/slot_orders.h"
void Ta_SetEffectDependOnFlagLst(struct jp_4e65f *eInfo, struct jp_4e704 *Temp, struct jp_4e65f *pCall, int eNO)
{
    int i;
    int Num;
    int Unit;

    Num = Ta_GetUnitNumFrom22bitFlagList(eInfo->dwTo);
    for (i = 0; i < Num; i++) {
        Unit = Ta_GetUnitOwnerIDFromLstCount(eInfo->dwTo, i);
        pCall->eNO = eNO;
        pCall->dwFrom = 0;
        pCall->dwTo = Unit;
        pCall->dwPar = eNO == 603 ? 1 : 0;
        /* eID is unsigned, so the test cannot fail; the original still emits it (andi/bltz).
         * The `+ 0` only keeps the host build's -Wtype-limits quiet; the plain comparison
         * compiles to the same code. */
        if ((Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp)) + 0 >= 0) {
            Temp->ePos[Temp->oNum] = Unit;
            Temp->oNum++;
        }
    }
}
