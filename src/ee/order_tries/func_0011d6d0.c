#include "dotr/ee/order_tries.h"

int TaLogic004_IsUnitOnLine(int a, int b)
{
    int here;

    here = SzDuel_GetUnitLoc(a, b);
    switch (here & 0xff) {
    case 0:
        if ((here >> 8) >= 0 && (here >> 8) <= 6) {
            return 3;
        }
        break;
    case 2:
        if ((here >> 8) > 1 && (here >> 8) <= 4) {
            return 0;
        }
        break;
    case 4:
        if ((here >> 8) > 1 && (here >> 8) <= 4) {
            return 1;
        }
        break;
    case 6:
        if ((here >> 8) >= 0 && (here >> 8) <= 6) {
            return 4;
        }
        break;
    }
    return -1;
}
