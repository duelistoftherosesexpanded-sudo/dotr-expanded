#include "dotr/ee/small_misc.h"

int TaLogic_IsStatusMatch(int actualStatus, int targetStatus)
{
    switch (targetStatus) {
    case 0:
        if (actualStatus == 0) {
            return 1;
        }
        break;
    case 1:
        if (actualStatus == 1) {
            return 1;
        }
        break;
    case 2:
    default:
        return 1;
    }
    return 0;
}
