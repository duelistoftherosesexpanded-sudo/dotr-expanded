#include "dotr/ee/small_misc.h"

int TaLogic_IsPassRoot(int side, int pos, int col, int row, int destCol, int destRow, unsigned short *atkTargetPtr)
{
    return TaLogic_IsPassRoot2(func_001df540_word(side, pos), col, row, destCol, destRow, atkTargetPtr);
}
