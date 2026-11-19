#include "dotr/ee/small_misc.h"
/* Collects candidate (kind, slot) halfwords for the kind and then for its partner. */
void TaUtil_GetUnitLst(int sideToCheck, int compMode, unsigned short *units)
{
    int i;
    int hi = (sideToCheck << 8) & 0xffff;
    for (i = 1; i < 11; i++) {
        if (func_001df540_word(sideToCheck, i)) {
            if (Ta_SideMatch(sideToCheck, func_0024de70_pair(sideToCheck, i), compMode) == 1) {
                *units++ = hi | i;
            }
        }
    }
    for (i = 1; i < 11; i++) {
        if (func_001df540_word(sideToCheck == 0 ? 1 : 0, i)) {
            if (Ta_SideMatch(sideToCheck, func_0024de70_pair(sideToCheck == 0 ? 1 : 0, i), compMode) == 1) {
                *units++ = i | ((sideToCheck == 0 ? 1 : 0) << 8);
            }
        }
    }
}
