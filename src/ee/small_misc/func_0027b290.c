#include "dotr/ee/small_misc.h"

/* Fills twelve nibble pairs with 0xf, builds a halfword from enemyUnitSide and enemyUnitArrayPos (all ones when enemyUnitArrayPos is -1),
 * calls TaLogic_GetRoot2 (0x2792c0, through a view that takes the nibble pairs) and returns
 * its result only if TaLogic_GetMinGoalCount then reports a positive count, else 0. */
int TaLogic_IsRootOpen2(int unit, int unitCol, int unitRow, int enemyUnitSide, int enemyUnitArrayPos, int enemyUnitCol, int enemyUnitRow, unsigned char maxDistance)
{
    struct DotrMiscNibbles bytes[12];
    int r;
    struct DotrMiscNibbles *p;
    int i;
    int w;
    for (i = 0, p = bytes; i < 12; i++, p++) p->col = p->row = 0xf;
    if (enemyUnitArrayPos == -1) w = -1; else w = (enemyUnitSide << 8) | enemyUnitArrayPos;
    r = func_002792c0_nibbles(unit, bytes, unitCol, unitRow, enemyUnitCol, enemyUnitRow, w, maxDistance);
    if (TaLogic_GetMinGoalCount(bytes) > 0) return r;
    return 0;
}
