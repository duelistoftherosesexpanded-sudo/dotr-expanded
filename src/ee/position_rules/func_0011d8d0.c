#include "dotr/ee/position_rules.h"

int TaLogic004_GetAtkLineStart(int a, int lineIndex)
{
    (void)a;
    switch (lineIndex) {
    case 0:
        return 0x204;
    case 1:
        return 0x202;
    case 2:
        return 0;
    case 3:
    default:
        return 6;
    }
}
