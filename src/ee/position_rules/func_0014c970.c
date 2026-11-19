#include "dotr/ee/position_rules.h"

int TaLogic014_IsLinePoint(int column, int row)
{
    int index;

    index = 0;
    do {
        if (((row << 8) | column) == TaLogic014_GetLine(index)) {
            return 1;
        }
        index += 1;
    } while (index < 3);
    return 0;
}
