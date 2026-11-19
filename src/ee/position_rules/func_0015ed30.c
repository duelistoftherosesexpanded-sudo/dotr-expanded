#include "dotr/ee/position_rules.h"

int TaLogic019_IsLinePoint(int column, int row)
{
    int index;

    index = 0;
    do {
        if (((row << 8) | column) == TaLogic019_GetLine(index)) {
            return 1;
        }
        index += 1;
    } while (index < 4);
    return 0;
}
