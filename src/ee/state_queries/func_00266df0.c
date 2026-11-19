#include "dotr/ee/state_queries.h"
/* Counts the present slots 1..10 of both sides whose Ta_GetUnitEffect(side, slot, 2) record
 * has the effect id `kind` (word 0 >> 8) and whose status bits 4, 3 and 5 are all set. */
int Ta_SearchUncTypeNaturalEffectUserNum(int kind)
{
    int i;
    int count = 0;
    struct DotrStateEntry8 *e;
    struct DotrStateEntry8 *f;
    int j;
    for (i = 1; i < 11; i++) {
        if (func_001df540_word(0, i)) {
            if ((e = Ta_GetUnitEffect(0, i, 2)) && SzDuel_GetUnitStatus(0, i, 4) && SzDuel_GetUnitStatus(0, i, 3) && SzDuel_GetUnitStatus(0, i, 5)) {
                if (((unsigned)e->unknown_words[0] >> 8) == (unsigned)kind) {
                    count++;
                }
            }
        }
    }
    for (j = 1; j < 11; j++) {
        if (func_001df540_word(1, j)) {
            if ((f = Ta_GetUnitEffect(1, j, 2)) && SzDuel_GetUnitStatus(1, j, 4) && SzDuel_GetUnitStatus(1, j, 3) && SzDuel_GetUnitStatus(1, j, 5)) {
                if (((unsigned)f->unknown_words[0] >> 8) == (unsigned)kind) {
                    count++;
                }
            }
        }
    }
    return count;
}
