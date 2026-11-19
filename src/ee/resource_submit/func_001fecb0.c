#include "dotr/ee/resource_submit.h"

struct DotrResourceQueueRecord *SzFile_ReadRequestParam(int fileType, int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010,
                                              DotrResourceSubmitCallback callback, int unknown_01c)
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
    if (!lSzFile_SetReadRequestParam((struct DotrResourceSubmitRecord *)record, fileType, priority, name, unknown_014,
                       byte_size, byte_offset, unknown_010, callback, unknown_01c)) {
        return 0;
    }
    state->count++;
    return record;
}
