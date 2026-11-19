#include "dotr/ee/kind_gates.h"

int TaLogic017_GetLine(int index, int far)
{
    SzDuel_GetTurnOwner();
    switch (index) {
    case 0:
        return far == 0 ? 0x200 : 0x206;
    case 1:
        return far == 0 ? 0x300 : 0x306;
    case 2:
        return far == 0 ? 0x400 : 0x406;
    case 3:
        return far == 0 ? 0x500 : 0x506;
    case 4:
        return far == 0 ? 0x600 : 0x606;
    case 5:
        return far == 0 ? 0x601 : 0x605;
    case 6:
        return far == 0 ? 0x602 : 0x604;
    default:
        return -1;
    }
}
