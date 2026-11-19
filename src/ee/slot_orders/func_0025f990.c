#include "dotr/ee/slot_orders.h"
void Ta_SetEffectDependOnFlagLst3(struct jp_4e65f *eInfo, struct jp_4e704 *Temp, struct jp_4e65f *pCall, int eNO)
{
    int i;

    for (i = 0; i < 22; i++) {
        if (((eInfo->dwFrom >> (21 - i)) & 1) == 1) {
            /* `+ 0` as in Ta_SetEffectDependOnFlagLst (for the host build's -Wtype-limits only). */
            if (i < 11) {
                pCall->eNO = eNO;
                pCall->dwFrom = 0;
                pCall->dwTo = i;
                pCall->dwPar = eNO == 603 ? 1 : 0;
                if ((Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp)) + 0 >= 0) {
                    Temp->ePos[Temp->oNum] = i;
                    Temp->oNum++;
                }
            } else {
                pCall->eNO = eNO;
                pCall->dwFrom = 0;
                pCall->dwTo = (i - 11) | 0x100;
                pCall->dwPar = eNO == 603 ? 1 : 0;
                if ((Temp->eID[Temp->oNum] = Ta_Do_Effect(pCall, Temp)) + 0 >= 0) {
                    Temp->ePos[Temp->oNum] = (i - 11) | 0x100;
                    Temp->oNum++;
                }
            }
        }
    }
}
