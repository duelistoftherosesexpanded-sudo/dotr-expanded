#include "dotr/ee/position_rules.h"

int TaLogic020_GetNextGrassPoint(int a, int b)
{
    if (b == 0) {
        if (a >= 3) {
            return 0x106;
        }
        if (a <= 2) {
            return 0x100;
        }
    }
    if (b <= 5) {
        if (a >= 3) {
            return 0x605;
        }
        if (a <= 2) {
            return 0x601;
        }
    }
    if (a >= 3) {
        return 0x601;
    }
    return 0x605;
}
