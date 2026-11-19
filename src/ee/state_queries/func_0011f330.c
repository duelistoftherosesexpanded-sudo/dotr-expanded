#include "dotr/ee/state_queries.h"

int TaLogic004_IsRefuseEmptyCombo(int id)
{
    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    if (id == 496 || (unsigned)(id - 509) <= 1 || id == 514 || id == 502 || id == 523) {
        return 1;
    }
    return 0;
}
