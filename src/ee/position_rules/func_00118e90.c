#include "dotr/ee/position_rules.h"

int TaLogic003_GetEmptyAtkLineEnd(int a)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (TaLogic003_IsEmptyAtkLine(a, i) == 0) {
            switch (i) {
            case 0:
                return 0x602;
            case 1:
                return 0x604;
            case 2:
                return 0x600;
            case 3:
            default:
                return 0x606;
            }
        }
    }
    return -1;
}
