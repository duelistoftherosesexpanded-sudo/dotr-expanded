#include "dotr/ee/position_rules.h"

int TaLogic020_GetLinePiece(int index)
{
    int low;

    low = index & 0xff;
    switch (low) {
    case 0:
        return 0x203;
    case 1:
        return 0x301;
    case 2:
        return 0x305;
    case 3:
        return 0x202;
    case 4:
        return 0x204;
    default:
        return 0x203;
    }
}
