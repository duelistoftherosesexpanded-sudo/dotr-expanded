#include "dotr/ee/lower_rules.h"

void TaLogic_SetTaskAtkOrMove(struct jp_9add8 *pTask, int nEnemyID, unsigned char fView)
{
    int Loc;
    int Ret;

    Loc = SzDuel_GetUnitLoc(nEnemyID >> 8, nEnemyID & 0xff);
    Ret = TaLogic_GetActionReaction(pTask->wUnit >> 8, pTask->wUnit & 0xff, Loc & 0xff, Loc >> 8, 0, fView);
    switch (Ret) {
    case 1:
    case 2:
        pTask->wPurpose = 0x6ff;
        pTask->dwGoal = (Loc << 16) | (pTask->dwGoal & 0xffff);
        break;
    default:
        pTask->wPurpose = 0x2ff;
        pTask->dwGoal = (nEnemyID << 16) | (pTask->dwGoal & 0xffff);
        break;
    }
}
