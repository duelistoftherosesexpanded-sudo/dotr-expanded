#include "dotr/ee/state_queries.h"

/* Votes over kind-86 entries: mode 3 counts +1, mode 2 counts -1 unless Ta_GetCurrentSide
 * returns who; returns the sign of the total. */
int Ta_GetSummonCalcRevision(int who)
{
    int i;
    int count;
    int score;
    int lo;
    int hi;
    struct DotrStateEntry8 *e;
    int slot;
    unsigned w;
    int mode;
    count = Ta_SearchUncTypeNaturalEffectUserNum(86);
    score = 0;
    for (i = 0; i < count; i++) {
        slot = Ta_SearchUncTypeNaturalEffectUser(86, i);
        hi = slot >> 8;
        lo = slot & 0xff;
        e = Ta_GetUnitEffect(hi, lo, 2);
        if (e) {
            w = e->unknown_words[0];
            if ((w >> 8) == 86) {
                mode = (w >> 6) & 3;
                if (mode == 2) {
                    if (who != Ta_GetCurrentSide(hi, lo)) {
                        score--;
                    }
                } else if (mode == 3) {
                    score++;
                }
            }
        }
    }
    if (score > 0) {
        score = 1;
    } else if (score < 0) {
        score = -1;
    }
    return score;
}
