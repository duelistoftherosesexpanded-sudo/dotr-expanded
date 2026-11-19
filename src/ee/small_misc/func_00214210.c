#include "dotr/ee/small_misc.h"

/* Adds delta to each nonzero word among the three leading words of each of the
 * `count` elements; with flag, also to the word before the base and to the word
 * before each adjusted target. Returns 1. */
int lNjaReloc_AdjustMData3(struct DotrMiscFixup24 *p, int delta, int count, int flag)
{
    int i;
    int j;
    if (flag) {
        ((DotrMiscAddress *)p)[-1] = delta + ((DotrMiscAddress *)p)[-1];
    }
    for (i = 0; i < count; i++) {
        for (j = 0; j < 3; j++) {
            if (p->unknown_00[j] != 0) {
                p->unknown_00[j] = delta + p->unknown_00[j];
                if (flag) {
                    ((DotrMiscAddress *)p->unknown_00[j])[-1] = delta + ((DotrMiscAddress *)p->unknown_00[j])[-1];
                }
            }
        }
        p++;
    }
    return 1;
}
