#include "dotr/ee/position_rules.h"

int TaLogic013_GetDefensePoint(int index)
{
    switch (index) {
    case 0:
        return 0x403;
    case 1:
        return 0x402;
    case 2:
        return 0x404;
    case 3:
        return 0x401;
    case 4:
        return 0x405;
    default:
        return -1;
    }
}
