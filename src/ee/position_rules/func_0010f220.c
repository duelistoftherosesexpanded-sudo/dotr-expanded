#include "dotr/ee/position_rules.h"

int TaLogic002_IsRefuseEmptyCombo(int id)
{
    if ((TaHand_GetCardKind() >> 5) != 0) {
        return 0;
    }
    if (Ta_GetHandEnchant(id, 0x31e) >= 0) {
        return 1;
    }
    if (id == 0x18e) {
        return 1;
    }
    return 0;
}
