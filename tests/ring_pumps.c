/* Synthetic host tests for the ring_pumps group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/ring_pumps.h"

struct DotrPumpStream { int unused; } data_00350620_pump;
static struct DotrPumpRing ring;
static char span1[8], span2[8];
static struct DotrPumpStream *jump_arg, *span_arg, *feed_arg;
static int jump_calls, span_calls, feed_calls, feed_count, copy_calls, copy_result;
static char *copy_ptrs[4];
static int copy_sizes[4];

int func_002435a0_pump(struct DotrPumpStream *stream)
{
    jump_arg = stream;
    jump_calls++;
    return 0x1234;
}
void func_00243390_pump(struct DotrPumpStream *stream, char **start1, int *count1, char **start2, int *count2)
{
    span_arg = stream;
    span_calls++;
    *start1 = span1; *count1 = 5;
    *start2 = span2; *count2 = 7;
}
void func_00243450_pump(struct DotrPumpStream *stream, int count)
{
    feed_arg = stream;
    feed_count = count;
    feed_calls++;
}
int copy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size)
{
    copy_ptrs[0] = first; copy_ptrs[1] = second; copy_ptrs[2] = a; copy_ptrs[3] = b;
    copy_sizes[0] = first_size; copy_sizes[1] = second_size; copy_sizes[2] = a_size; copy_sizes[3] = b_size;
    copy_calls++;
    return copy_result;
}

static void run(int offset, int size, int result, int expect, int from, int first, int rest)
{
    struct DotrPumpRequest request;
    int calls = copy_calls;

    memset(&request, 0, sizeof(request));
    request.position = ring.bytes + offset;
    request.size = size;
    copy_result = result;
    assert(pcmCallback(99, &request, &ring) == expect);
    assert(copy_calls == calls + 1 && span_calls == copy_calls && feed_calls == copy_calls);
    assert(span_arg == &data_00350620_pump && feed_arg == &data_00350620_pump && feed_count == result);
    assert(copy_ptrs[0] == span1 && copy_sizes[0] == 5 && copy_ptrs[1] == span2 && copy_sizes[1] == 7);
    assert(copy_ptrs[2] == ring.bytes + from && copy_sizes[2] == first);
    assert(copy_ptrs[3] == ring.bytes && copy_sizes[3] == rest);
}

static void check_tail_jump(void)
{
    /* Straight-line coverage. */
    assert(proceedAudio() == 0x1234 && jump_calls == 1 && jump_arg == &data_00350620_pump);
}

static void check_pump(void)
{
    assert(offsetof(struct DotrPumpRing, size) == 0x50008);
    ring.size = 0x100;

    /* Fits before the ring's end: no remainder; positive count answers 1. */
    run(0x10, 0x24, 3, 1, 0x14, 0x20, 0);
    /* Zero and negative counts answer 0. */
    run(0x10, 0x24, 0, 0, 0x14, 0x20, 0);
    run(0x10, 0x24, -1, 0, 0x14, 0x20, 0);
    /* Runs past the end: split at the end, remainder from the start. */
    run(0xf0, 0x24, 1, 1, 0xf4, 0x0c, 0x14);
    /* size - 4 equal to the room left: not clamped, no remainder. */
    run(0xf0, 0x10, 1, 1, 0xf4, 0x0c, 0);
    /* Position + 4 exactly at the end wraps to the start. */
    run(0xfc, 0x24, 1, 1, 0x00, 0x20, 0);
    /* One below the end does not wrap. */
    run(0xfb, 0x24, 1, 1, 0xff, 0x01, 0x1f);
    /* Beyond the end wraps by the capacity. */
    run(0xfe, 0x24, 1, 1, 0x02, 0x20, 0);
}

int main(void)
{
    check_tail_jump();
    check_pump();
    return 0;
}
