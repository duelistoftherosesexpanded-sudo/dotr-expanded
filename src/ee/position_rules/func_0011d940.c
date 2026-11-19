#include "dotr/ee/position_rules.h"

int TaLogic004_GetAtkLineEnd(int a, int k)
{
    (void)a;
    switch (k) {
    case 0:
        return 0x402;
    case 1:
        return 0x404;
    case 2:
        return 0x600;
    case 3:
    default:
        return 0x606;
    }
}
