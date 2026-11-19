#include "dotr/ee/kind_gates.h"

int TaLogic_GetCyclePoint(int index)
{
    switch (index) {
    case 0:
        return 2;
    case 1:
        return 6;
    case 2:
        return 0x406;
    case 3:
        return 0x404;
    case 4:
        return 0x604;
    case 5:
        return 0x600;
    case 6:
        return 0x200;
    case 7:
        return 0x202;
    default:
        return 2;
    }
}
