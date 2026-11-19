#include "dotr/ee/small_misc.h"

/* The byte-typed predicate keeps the materialized comparison. */
static inline unsigned char full(struct DotrMiscQueue *queue)
{
    return queue->unknown_0c == queue->unknown_10;
}

DotrMiscAddress voBufGetData(struct DotrMiscQueue *queue)
{
    if (full(queue)) {
        return 0;
    }
    return queue->unknown_00 + queue->unknown_08 * 0x195000;
}
