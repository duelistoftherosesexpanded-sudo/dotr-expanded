#include "dotr/ee/state_queries.h"

/* Replaces its first two arguments by func_001df540_word(side, pos) and forwards
 * the remaining nine to TaLogic_GetInvadeRoot2. */
int TaLogic_GetInvadeRoot(int side, int pos, int c, int d, int e, int f, int g, int h, int i, unsigned char j, unsigned short k)
{
    return TaLogic_GetInvadeRoot2(func_001df540_word(side, pos), c, d, e, f, g, h, i, j, k);
}
