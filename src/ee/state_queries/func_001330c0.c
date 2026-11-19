#include "dotr/ee/state_queries.h"

int TaLogic008_IsRefuseEmptyCombo(int id)
{
    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    if (id == 573 || id == 534 || id == 99) {
        return 1;
    }
    return 0;
}
