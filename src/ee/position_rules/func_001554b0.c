#include "dotr/ee/position_rules.h"

int TaLogic017_IsLinePoint(int a, int b, int magicType)
{
    int i;
    int key = (b << 8) | a;
    for (i = 0; i < 7; i++) {
        if (key == TaLogic017_GetLine(i, magicType)) {
            return 1;
        }
    }
    return 0;
}
