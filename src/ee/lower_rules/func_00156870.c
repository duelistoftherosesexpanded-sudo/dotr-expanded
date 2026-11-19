#include "dotr/ee/lower_rules.h"

int TaLogic017_IsRefuseEmptyCombo(int id)
{
    int word = TaHand_GetCardAPDP(id);

    if ((TaHand_GetCardKind(id) >> 5) != 0) {
        return 1;
    }
    if ((word & 0xFFFF) + (word >> 16) * 2 > 4500) {
        return 1;
    }
    if (TaLogic017_IsExzParts(id) == 1) {
        return 1;
    }
    return 0;
}
