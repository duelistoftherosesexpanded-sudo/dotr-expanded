#include "dotr/ee/resource_submit.h"

/* Same logic as SzFile_ReadRequestParam, which 0x1ffe00 contains inline while
 * 0x1ffd30 calls it; the original source relationship is unknown.
 */
static inline struct DotrResourceQueueRecord *dotr_resource_submit_inline(
    int type, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
    int unknown_010, DotrResourceSubmitCallback callback, int unknown_01c)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    struct DotrResourceQueueRecord *record;
    if (!name) {
        return 0;
    }
    if (state->count >= 256) {
        return 0;
    }
    record = &state->records[(state->head + state->count) % 256];
    if (!lSzFile_SetReadRequestParam((struct DotrResourceSubmitRecord *)record, type, priority, name, unknown_014,
                       byte_size, byte_offset, unknown_010, callback, unknown_01c)) {
        return 0;
    }
    state->count++;
    return record;
}

struct DotrResourceQueueRecord *SzFile_ReadParam(int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010)
{
    struct DotrResourceQueueRecord *record;
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    record = dotr_resource_submit_inline(0x1400, priority, name, unknown_014, byte_size,
                                         byte_offset, unknown_010, lSzFile_ReadCB, 0);
    if (!record) {
        return 0;
    }
    while (SzFile_InqRequestX(-1, 0)) {
        SzFile_DoRequestS();
    }
    return state->flags.bit_0 ? record : 0;
}
