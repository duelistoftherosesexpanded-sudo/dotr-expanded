#include "dotr/ee/span_ring.h"

int viBufPutTs(DotrSpanRing *ring, DotrSpanRecord *span)
{
    int added = 0;

    func_0017b920(ring->unknown_40);
    if (ring->count < ring->capacity) {
        viBufModifyPts(ring, span);
        if (span->first >= 0 || span->second >= 0) {
            ring->records[ring->tail].first = span->first;
            ring->records[ring->tail].second = span->second;
            ring->records[ring->tail].pos = span->pos;
            ring->records[ring->tail].len = span->len;
            ring->count++;
            ring->tail = (ring->tail + 1) % ring->capacity;
        }
        added = 1;
    }
    func_0017b900(ring->unknown_40);
    return added;
}
