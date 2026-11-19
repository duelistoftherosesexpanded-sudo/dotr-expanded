#include "dotr/ee/state_queries.h"

/* Presets four output words to {999, 0, 0, 0}; when the flag query lacks
 * 0x80 and the count is below five, writes {114, SzDuel_GetTurnOwner(), 0, 0} and
 * returns 1. The words are stored before the null test, as observed. */
int Ta_SetDestinyEffect(int side, int *out)
{
    int flags = SzDuel_InqDestinyFlag(side);
    out[0] = 999;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    if (!(flags & 0x80)) {
        if (SzDuel_GetHandCount(side) < 5) {
            if (out != 0) {
                out[0] = 114;
                out[1] = SzDuel_GetTurnOwner();
                out[2] = 0;
                out[3] = 0;
            }
            return 1;
        }
    }
    return 0;
}
