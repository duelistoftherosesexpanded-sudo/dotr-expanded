#include "dotr/ee/driver_scans.h"

int TaLogic_DoAdjustLogicData(struct jp_9b352 *pCtl)
{
    int tSide;
    int i;

    tSide = SzDuel_GetTurnOwner();
    if (TaTuto_IsTuto(tSide) == 1) {
        return 1;
    }
    if (pCtl != 0) {
        pCtl->bCount = 0;
        for (i = 0; i < 32; i++) {
            pCtl->wPadQue[i] = 0xFFFF;
        }
        for (i = 0; i < 22; i++) {
            if (pCtl->nTask[i].wUnit < 0xFFFF) {
                if (SzDuel_GetUnit(pCtl->nTask[i].wUnit >> 8, pCtl->nTask[i].wUnit & 0xFF) == 0 ||
                    Ta_GetCurrentSide(pCtl->nTask[i].wUnit >> 8, pCtl->nTask[i].wUnit & 0xFF) != tSide) {
                    pCtl->nTask[i].wUnit = 0xFFFF;
                    pCtl->nTask[i].wOrder = 255;
                    pCtl->nTask[i].dwGoal = -1;
                    pCtl->nTask[i].wPurpose = 0xFFFF;
                    pCtl->nTask[i].bCond = 0;
                /* The second argument is the comparison's result, as the original code has it
                 * (`xor; sltu`). */
                } else if (SzDuel_GetUnitSummonOrder(pCtl->nTask[i].wUnit >> 8, (pCtl->nTask[i].wUnit & 0xFF) != pCtl->nTask[i].wOrder)) {
                    pCtl->nTask[i].wOrder = SzDuel_GetUnitSummonOrder(pCtl->nTask[i].wUnit >> 8, pCtl->nTask[i].wUnit & 0xFF);
                    pCtl->nTask[i].dwGoal = -1;
                    pCtl->nTask[i].wPurpose = 0xFFFF;
                    pCtl->nTask[i].bCond = 0;
                }
            }
        }
        for (i = 0; i < 11; i++) {
            if (Ta_GetCurrentSide(0, i) == tSide) {
                if (TaLogic_IsUnitAssign(i, pCtl) == 0) {
                    if (SzDuel_GetUnit(0, i) != 0) {
                        TaLogic_DoUnitAssign(i, pCtl);
                    }
                }
            }
            if (Ta_GetCurrentSide(1, i) == tSide) {
                if (TaLogic_IsUnitAssign(i | 0x100, pCtl) == 0) {
                    if (SzDuel_GetUnit(1, i) != 0) {
                        TaLogic_DoUnitAssign(i | 0x100, pCtl);
                    }
                }
            }
        }
        TaLogic_DoSortTaskList(pCtl);
        return 1;
    }
    return -1;
}
