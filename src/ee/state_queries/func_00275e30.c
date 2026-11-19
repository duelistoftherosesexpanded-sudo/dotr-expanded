#include "dotr/ee/state_queries.h"

/* Returns 1 when TaTuto_IsTuto(SzDuel_GetTurnOwner()) is 1; otherwise clears the
 * halfword at +0x1a2 and returns TaLogic_DoAdjustLogicData(p). */
int TaLogic_AdjustLogicData(struct DotrQueryObject1a2 *p)
{
    if (TaTuto_IsTuto(SzDuel_GetTurnOwner()) == 1) {
        return 1;
    }
    p->unknown_1a2 = 0;
    return TaLogic_DoAdjustLogicData(p);
}
