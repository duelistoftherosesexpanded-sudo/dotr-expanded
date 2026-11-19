#include "dotr/ee/position_rules.h"

int TaLogic010_GetLineHeight(int side)
{
    int i;
    unsigned char best = 0;
    for (i = 1; i <= 5; i++) {
        if (func_001df540_word(side, i) != 0) {
            unsigned char v = SzDuel_GetUnitLoc(side, i) >> 8;
            if (best < v) {
                best = v;
            }
        }
    }
    if (best <= 1) {
        best = 2;
    }
    return best;
}
