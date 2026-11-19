#include "dotr/ee/low_leaves.h"
int Ta_Do_Effect(struct jp_11dc9 *eInfo, struct jp_11e6e *Temp)
{
    int eID;
    switch (TaEffect_IsLoadModule2(Temp, eInfo->eNO)) {
    case 0:
        return -2;
    case -1:
        return -3;
    default:
        break;
    }
    eID = SzEffect_Entry(0, eInfo->eNO, eInfo->dwPar, eInfo->dwTo, eInfo->dwFrom);
    if (eID < 0) {
        return -1;
    }
    return eID;
}
