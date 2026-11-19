#include "dotr/ee/position_rules.h"

int TaLogic001_GetNextGrassPoint(int col, int row)
{
    if (row >= 5) {
        return 0x602;
    }
    if (col >= 5) {
        return 0x206;
    }
    if (col >= row) {
        return 0x105;
    }
    return 0x501;
}
