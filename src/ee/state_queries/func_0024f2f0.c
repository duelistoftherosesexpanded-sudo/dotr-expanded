#include "dotr/ee/state_queries.h"

/* The low byte of one lookup word is passed twice to 0x2509e0 and once, with
 * the word's upper part, to 0x250b40; the two results are added. */
int Ta_GetAPPlusDP(int side, int pos)
{
    int unitLoc = SzDuel_GetUnitLoc(side, pos);
    int low = unitLoc & 0xff;
    int first = TaUtil_GetUnitAP(side, pos, low, low);
    return first + TaUtil_GetUnitDP(side, pos, low, unitLoc >> 8);
}
