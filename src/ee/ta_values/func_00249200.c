#include "dotr/ee/ta_values.h"

int Ta_GetEncEffect(int no)
{
    int r;
    switch (EnchantId_ta[no]) {
    case 0:
        switch (gAnmPat_Bakura_ta[no]) {
        case 0:
            r = 537;
            break;
        default:
            r = 536;
            break;
        }
        break;
    case 3:
        r = 541;
        break;
    case 2:
        r = 542;
        break;
    case 1:
        switch (gAnmPat_Bakura_ta[no]) {
        case 5:
            r = 540;
            break;
        default:
            r = 536;
            break;
        }
        break;
    default:
        r = 536;
        break;
    }
    return r;
}
