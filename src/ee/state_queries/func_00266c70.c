#include "dotr/ee/state_queries.h"
/* Returns the n-th slot (1..10; side 1 adds 0x100) whose three grid bits are set and whose
 * entry kind is `kind`, or -1. */
int Ta_SearchUncTypeNaturalEffectUser(int kind, int n)
{
    int count = 0;
    int i;
    for (i = 1; i < 11; i++) {
        if (SzDuel_GetUnitStatus(0, i, 4) && SzDuel_GetUnitStatus(0, i, 3) && SzDuel_GetUnitStatus(0, i, 5)) {
            struct DotrStateEntry8 *e = Ta_GetUnitEffect(0, i, 2);
            if (e && ((unsigned)e->unknown_words[0] >> 8) == (unsigned)kind) {
                if (count++ == n) {
                    return i;
                }
            }
        }
    }
    for (i = 1; i < 11; i++) {
        if (SzDuel_GetUnitStatus(1, i, 4) && SzDuel_GetUnitStatus(1, i, 3) && SzDuel_GetUnitStatus(1, i, 5)) {
            struct DotrStateEntry8 *e = Ta_GetUnitEffect(1, i, 2);
            if (e && ((unsigned)e->unknown_words[0] >> 8) == (unsigned)kind) {
                if (count++ == n) {
                    return i | 0x100;
                }
            }
        }
    }
    return -1;
}
