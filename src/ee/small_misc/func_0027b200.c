#include "dotr/ee/small_misc.h"

int TaLogic_IsRootOpen(int side, int pos, int c, int d, int e, int f, int g, int h, unsigned char maxDistance)
{
    return TaLogic_IsRootOpen2(func_001df540_word(side, pos), c, d, e, f, g, h, maxDistance);
}
