#include "dotr/ee/small_misc.h"

/* Clears two bit words, then sets one bit per leading negative source word.
 * The test uses the word's two fields; the bit position uses only the running
 * index, as in the observed instructions. */
void Ta_SetEffectObjByField(struct DotrMiscBitWords *out, int *source)
{
    int i;
    out->unknown_04 = 0;
    out->unknown_08 = 0;
    for (i = 0; *source < 0; source++, i++) {
        if ((*source & 0xff) + (*source >> 8) * 7 < 29) {
            out->unknown_04 |= 1 << (28 - i);
        } else {
            out->unknown_08 |= 1 << (21 - (i - 29));
        }
    }
}
