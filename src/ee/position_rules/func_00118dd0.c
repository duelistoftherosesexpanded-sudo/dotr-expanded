#include "dotr/ee/position_rules.h"

int TaLogic003_GetEmptyAtkLineStart(int a)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (TaLogic003_IsEmptyAtkLine(a, i) != 0) {
            switch (i) {
            case 0:
                return 0x102;
            case 1:
                return 0x104;
            case 2:
                return 0x100;
            case 3:
            default:
                return 0x106;
            }
        }
    }
    return -1;
}
