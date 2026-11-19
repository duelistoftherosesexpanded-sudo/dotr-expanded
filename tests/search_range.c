/* Synthetic host tests for the search_range group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/search_range.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int SzDuel_GetUnitKind(int arg0, int arg1) { return 0; }
int SzDuel_GetUnitAttr(int arg0, int arg1) { return 0; }
int SzDuel_GetUnitLoc(int arg0, int arg1) { return 0x0203; } /* row 2, column 3 */
int SzDuel_GetUnitSummonOrder(int unused0, int unused1) { return 0; }
int SzDuel_GetUnitStatus(int arg0, int arg1, int arg2) { return 0; }
int SzDuel_GetField(int arg0, int arg1) { return 0; }
int SzDuel_GetUnitCardNo(int arg0, int arg1) { return 0; }
int ConvUnitIDtoFlagList(unsigned int arg0, int arg1, int arg2) { return (int)(arg0 | (1u << (arg1 * 11 + arg2))); }
void TaUtil_GetUnitLst(int arg0, int arg1, short *arg2) { arg2[0] = 0x0104; } /* side 1, unit 4 */
int Ta_SideMatch(int arg0, int arg1, int arg2) { return 1; }
int Ta_GetCurrentSide(int arg0, int arg1) { return arg0; }
int TaUtil_GetUnitAP(int arg0, int arg1, int arg2, int arg3) { return 0; }
int TaUtil_GetUnitDP(int arg0, int arg1, int arg2, int arg3) { return 0; }
int TaLogic_GetUnitCtlLstLen(unsigned short *values) { return 1; }

int main(void)
{
    /* Distance search (range 5) from (3, 2) with filter 0x1f (any unit): the one listed unit
     * stands on the start cell, so its flag is returned. */
    assert(SearchUnitByRange(3, 2, 0, 3, 0x1f, 5) == (1 << (11 + 4)));
    return 0;
}
