#include "dotr/ee/small_misc.h"

static inline unsigned char same(int a, int b)
{
    return a == b;
}

/* Qualified accesses reproduce the two observed count loads without changing
 * the shared queue view. The original qualifier is unknown. */
#define COUNT(queue) (*(volatile int *)&(queue)->unknown_0c)

DotrMiscAddress voBufGetTag(struct DotrMiscQueue *queue)
{
    if (same(COUNT(queue), 0)) return 0;
    return queue->unknown_04
        + ((queue->unknown_08 - COUNT(queue) + queue->unknown_10)
            % queue->unknown_10) * 0x4ce40;
}
