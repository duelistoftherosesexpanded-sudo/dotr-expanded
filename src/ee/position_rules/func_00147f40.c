#include "dotr/ee/position_rules.h"

int TaLogic013_GetEmptyLine(void)
{
    int i;
    int spot;
    int r;

    for (i = 0; i < 5; i++) {
        spot = TaLogic013_GetDefensePoint(i);
        if ((SzDuel_GetMapUnit(spot & 0xff, spot >> 8) == 0 ? 1 : 0) == 1) {
            switch (i) {
            case 0:
                r = 0x403;
                break;
            case 1:
                r = 0x402;
                break;
            case 2:
                r = 0x404;
                break;
            case 3:
                r = 0x401;
                break;
            case 4:
                r = 0x405;
                break;
            default:
                r = -1;
                break;
            }
            return r;
        }
    }
    return -1;
}
