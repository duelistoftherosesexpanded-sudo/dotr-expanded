#include "dotr/ee/state_queries.h"
/* Counts slots on both sides with matching mode (via 0x24de70) and entry kind. */
int Ta_SearchUncTypeNaturalEffectUserNumWithSide(int kind, int mode)
{
    int count = 0;
    int i;
    for (i = 1; i < 11; i++) {
        if (SzDuel_GetUnitStatus(0, i, 4) && SzDuel_GetUnitStatus(0, i, 3) && SzDuel_GetUnitStatus(0, i, 5) && mode == Ta_GetCurrentSide(0, i)) {
            struct DotrStateEntry8 *e = Ta_GetUnitEffect(0, i, 2);
            if (e && ((unsigned)e->unknown_words[0] >> 8) == (unsigned)kind) {
                count++;
            }
        }
    }
    for (i = 1; i < 11; i++) {
        if (SzDuel_GetUnitStatus(1, i, 4) && SzDuel_GetUnitStatus(1, i, 3) && SzDuel_GetUnitStatus(1, i, 5) && mode == Ta_GetCurrentSide(1, i)) {
            struct DotrStateEntry8 *e = Ta_GetUnitEffect(1, i, 2);
            if (e && ((unsigned)e->unknown_words[0] >> 8) == (unsigned)kind) {
                count++;
            }
        }
    }
    return count;
}
