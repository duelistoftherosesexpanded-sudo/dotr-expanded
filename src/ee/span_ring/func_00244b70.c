#include "dotr/ee/span_ring.h"

/* Walks the ring from its oldest record (tail - count, modulo capacity) and trims from the
 * front of each record the part covered by `span`, positions taken modulo unit * 2048. A
 * record trimmed to nothing is cleared (when its first doubleword is not negative) and the
 * count drops, not below zero. Stops at a record of length zero, at a span of length zero
 * or at the first record that does not start inside the span. Always returns 0. */
int viBufModifyPts(DotrSpanRing *ring, DotrSpanRecord *span)
{
    int i;
    int size;
    int more;
    DotrSpanRecord *rec;
    int over;
    int left;
    int n;

    more = 1;
    i = (ring->capacity + (ring->tail - ring->count)) % ring->capacity;
    size = ring->unit << 11;
    if (ring->count > 0) {
        do {
            rec = &ring->records[i];
            if (rec->len == 0) {
                break;
            }
            if (span->len == 0) {
                break;
            }
            if ((rec->pos + size - span->pos) % size < span->len) {
                over = span->pos + span->len - rec->pos;
                n = (rec->len < over) ? rec->len : over;
                rec->pos = (rec->pos + n) % size;
                rec->len -= n;
                if (rec->len == 0) {
                    if (rec->first >= 0) {
                        rec->first = -1;
                        rec->second = -1;
                        rec->pos = 0;
                        rec->len = 0;
                    }
                    left = ring->count - 1;
                    ring->count = (left < 0) ? 0 : left;
                }
            } else {
                more = 0;
            }
            i = (i + 1) % ring->capacity;
        } while (more);
    }
    return 0;
}
