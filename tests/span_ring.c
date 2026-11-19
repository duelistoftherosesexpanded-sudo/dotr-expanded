/* Synthetic host tests for the span_ring group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/span_ring.h"

static DotrSpanRecord recs[4];
static DotrSpanRing ring;

static void set(int i, long first, int pos, int len)
{
    recs[i].first = first;
    recs[i].second = first;
    recs[i].pos = pos;
    recs[i].len = len;
}

static void reset(int count, int tail)
{
    memset(&ring, 0, sizeof(ring));
    memset(recs, 0, sizeof(recs));
    ring.unit = 1; /* positions modulo 2048 */
    ring.records = recs;
    ring.capacity = 4;
    ring.count = count;
    ring.tail = tail;
}

static void check_trim(void)
{
    DotrSpanRecord span;

    memset(&span, 0, sizeof(span));

    /* count 0: nothing is read or written. */
    reset(0, 1);
    set(1, 5, 100, 10);
    span.pos = 100;
    span.len = 10;
    assert(viBufModifyPts(&ring, &span) == 0 && recs[1].len == 10 && ring.count == 0);

    /* Oldest record has length zero: stop at once. */
    reset(2, 2);
    set(0, 5, 100, 0);
    set(1, 5, 100, 10);
    assert(viBufModifyPts(&ring, &span) == 0 && recs[1].len == 10 && ring.count == 2);

    /* Span of length zero: stop at once. */
    reset(1, 1);
    set(0, 5, 100, 10);
    span.len = 0;
    assert(viBufModifyPts(&ring, &span) == 0 && recs[0].len == 10 && ring.count == 1);

    /* Record starts outside the span (distance == span length is outside): untouched. */
    reset(1, 1);
    set(0, 5, 110, 10);
    span.pos = 100;
    span.len = 10;
    assert(viBufModifyPts(&ring, &span) == 0 && recs[0].pos == 110 && recs[0].len == 10);
    assert(ring.count == 1);

    /* Partly covered: the front is trimmed, the count stays, the walk goes on to the next
     * record (index wraps 3 -> 0), which starts outside and ends it. */
    reset(2, 1);
    set(3, 5, 105, 10);
    set(0, 6, 300, 8);
    assert(viBufModifyPts(&ring, &span) == 0 && recs[3].pos == 110 && recs[3].len == 5);
    assert(recs[3].first == 5 && recs[0].pos == 300 && recs[0].len == 8 && ring.count == 2);

    /* Fully covered (overlap equals the record length): cleared, count drops; position
     * wraps modulo 2048 before the clear. */
    reset(1, 1);
    set(0, 5, 2040, 8);
    span.pos = 2040;
    span.len = 8;
    assert(viBufModifyPts(&ring, &span) == 0 && recs[0].first == -1 && recs[0].second == -1);
    assert(recs[0].pos == 0 && recs[0].len == 0 && ring.count == 0);

    /* Negative first doubleword: not cleared, position kept (wrapped), count still drops. */
    reset(1, 1);
    set(0, -2, 2040, 8);
    assert(viBufModifyPts(&ring, &span) == 0 && recs[0].first == -2 && recs[0].second == -2);
    assert(recs[0].pos == 0 && recs[0].len == 0 && ring.count == 0);
    set(0, -2, 2044, 4);
    ring.count = 1;
    assert(viBufModifyPts(&ring, &span) == 0 && recs[0].pos == 0 && ring.count == 0);

    /* Two covered records in a row: both cleared, the walk ends on the emptied third. */
    reset(2, 2);
    set(0, 1, 100, 4);
    set(1, 2, 104, 4);
    span.pos = 100;
    span.len = 20;
    assert(viBufModifyPts(&ring, &span) == 0 && recs[0].first == -1 && recs[1].first == -1);
    assert(ring.count == 0);
}

static int lock_log[2];
static int lock_calls;

void func_0017b920(int unknown)
{
    assert(lock_calls == 0);
    lock_log[lock_calls++] = unknown;
}

void func_0017b900(int unknown)
{
    assert(lock_calls == 1);
    lock_log[lock_calls++] = unknown;
}

/* One 0x244cb0 call on a ring holding `count` records with the given tail; the span has length
 * zero so the real 0x244b70 leaves the ring alone. `stored` says whether a record is expected. */
static void add(int count, int tail, long first, long second, int result, int stored)
{
    DotrSpanRecord span;
    int i;

    reset(count, tail);
    for (i = 0; i < 4; i++) set(i, 0x70 + i, 0, 0);
    ring.unknown_40 = 0x1234;
    span.first = first;
    span.second = second;
    span.pos = 0x321;
    span.len = 0;
    lock_calls = 0;
    assert(viBufPutTs(&ring, &span) == result);
    assert(lock_calls == 2 && lock_log[0] == 0x1234 && lock_log[1] == 0x1234);
    for (i = 0; i < 4; i++) {
        if (stored && i == tail) {
            assert(recs[i].first == first && recs[i].second == second && recs[i].pos == 0x321 && recs[i].len == 0);
        } else {
            assert(recs[i].first == 0x70 + i && recs[i].second == 0x70 + i && recs[i].pos == 0);
        }
    }
    assert(ring.count == count + stored && ring.tail == (stored ? (tail + 1) % 4 : tail));
    assert(span.first == first && span.second == second && span.pos == 0x321);
}

/* 0x244cb0: full ring (count == capacity) and one below it; the four sign combinations of the
 * span's two doublewords (only both negative skips the store); tail wrapping at capacity. */
static void check_add(void)
{
    add(4, 1, 5, 6, 0, 0);
    add(3, 3, 5, 6, 1, 1);
    add(0, 0, 0, 0, 1, 1);
    add(1, 2, -1, 6, 1, 1);
    add(1, 2, 5, -1, 1, 1);
    add(1, 2, -1, -1, 1, 0);
}

int main(void)
{
    check_add();
    check_trim();
    return 0;
}
