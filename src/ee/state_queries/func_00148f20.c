#include "dotr/ee/state_queries.h"

int TaLogic013_IsRefuseEmptyCombo(int id)
{
    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    if (TaHand_IsCardEffect(id) == 1) {
        return 1;
    }
    return 0;
}
