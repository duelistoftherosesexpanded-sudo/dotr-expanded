#include "dotr/ee/state_queries.h"

int TaLogic011_IsRefuseEmptyCombo(int id)
{
    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    return TaHand_IsCardEffect(id) != 0 ? 1 : 0;
}
