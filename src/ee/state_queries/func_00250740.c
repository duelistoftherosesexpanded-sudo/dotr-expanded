#include "dotr/ee/state_queries.h"

/* Returns the entry unless the item or entry is missing or both entry words
 * are -1. */
struct DotrStateEntry8 *Ta_GetUnitEffect(int side, int pos, int effInd)
{
    struct DotrStateEntry8 *effect;
    if (SzDuel_GetUnit(side, pos) == 0) {
        return 0;
    }
    effect = SzDuel_GetUnitEffect(side, pos, effInd);
    if (effect == 0) {
        return 0;
    }
    if (effect->unknown_words[0] == -1 && effect->unknown_words[1] == -1) {
        return 0;
    }
    return effect;
}
