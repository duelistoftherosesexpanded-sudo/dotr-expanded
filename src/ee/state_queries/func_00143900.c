#include "dotr/ee/state_queries.h"

int TaLogic012_GetAgreeLineZ(struct DotrStateItem *item)
{
    int i;
    for (i = 1; i <= 5; i++) {
        if (CheckLandAgreement(SzDuel_GetField(0, i), item) == 1) {
            return i;
        }
    }
    return -1;
}
