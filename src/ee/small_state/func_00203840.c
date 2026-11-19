#include "dotr/ee/small_state.h"

/* For each of two entries whose six-bit field is 5: a word counts up while
 * its bit is set in both the first and second halfword, otherwise it
 * restarts at the negated byte; the three halfwords then shift down. */
void SzPad_Act(void)
{
    int i, j;
    for (i = 0; i < 2; i++) {
        struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[i];
        if (e->unknown_5b.unknown_bits_0_5 == 5) {
            for (j = 0; j < 16; j++) {
                int mask = 1 << j;
                if ((e->unknown_00 & mask) && (e->unknown_00 & mask) == (e->unknown_02 & mask)) {
                    e->unknown_18[j]++;
                } else {
                    e->unknown_18[j] = -e->unknown_59;
                }
            }
            e->unknown_02 = e->unknown_00;
            e->unknown_00 = e->unknown_04;
            e->unknown_04 = 0;
        }
    }
}
