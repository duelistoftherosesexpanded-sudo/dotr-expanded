#include "dotr/ee/state_queries.h"

int TaLogic005_IsRefuseEmptyCombo(int id)
{
    int kind;

    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    kind = TaHand_GetCardKind(id) & 0x1f;
    if (kind == 0 || kind == 15 || kind == 18) {
        return 1;
    }
    return 0;
}
