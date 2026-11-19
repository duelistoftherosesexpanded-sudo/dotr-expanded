#include "dotr/ee/position_rules.h"

int TaLogic005_GetDefensePoint(int index)
{
    switch (index) {
    case 0:
        return 0x303;
    case 1:
        return 0x304;
    default:
    case 2:
        return 0x302;
    }
}
