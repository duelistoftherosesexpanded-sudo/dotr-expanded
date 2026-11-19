#include "dotr/ee/position_rules.h"

int TaLogic013_IsDefensePoint(int column, int row)
{
    int index;

    index = 0;
    do {
        if (((row << 8) | column) == TaLogic013_GetDefensePoint(index)) {
            return 1;
        }
        index += 1;
    } while (index < 5);
    return 0;
}
