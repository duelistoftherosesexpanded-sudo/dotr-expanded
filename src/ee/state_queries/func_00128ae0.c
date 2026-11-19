#include "dotr/ee/state_queries.h"

int TaLogic006_IsRefuseEmptyCombo(int id)
{
    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    if (id == 108 || id == 111 || id == 104) {
        return 1;
    }
    return 0;
}
