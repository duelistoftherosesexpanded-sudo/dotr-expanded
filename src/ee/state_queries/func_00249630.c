#include "dotr/ee/state_queries.h"

/* The halfword at +4 of the slot-1 entry when its first word has kind 9,
 * otherwise -1. */
int WallFusion(int side, int pos)
{
    struct DotrStateEntry8 *effect = Ta_GetUnitEffect(side, pos, 1);
    if (effect != 0) {
        if ((unsigned)effect->unknown_words[0] >> 8 == 9) {
            return *(unsigned short *)&effect->unknown_words[1];
        }
    }
    return -1;
}
