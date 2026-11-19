#include "dotr/ee/small_misc.h"

/* Splits the ring region to read, ending at position, into up to two
 * (address, length) pairs: a/b for the first run, c/d for the wrapped run
 * (zero when no wrap). The length is rounded down to 1024 bytes. */
void iopGetArea(int *a, int *b, int *c, int *d, struct DotrMiscRing *ring, int position)
{
    int over;
    int t = (position + ring->unknown_48 - ring->unknown_4c - 1024) % ring->unknown_48;
    t = (t >> 10) << 10;
    over = ring->unknown_48 - ring->unknown_4c < t;
    if (!over) {
        *a = ring->unknown_44 + ring->unknown_4c;
        *b = t;
        *c = 0;
        *d = 0;
    } else {
        *a = ring->unknown_44 + ring->unknown_4c;
        *b = ring->unknown_48 - ring->unknown_4c;
        *c = ring->unknown_44;
        *d = t - (ring->unknown_48 - ring->unknown_4c);
    }
}
