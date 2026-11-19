#include "dotr/ee/resource_process.h"

/* Same logic as SzFile_ReadRequestParam, which 0x200510 contains inline (as
 * SzFile_ReadParam, 0x1ffe00, does); the original source relationship is unknown.
 */
static inline struct DotrResourceQueueRecord *dotr_resource_process_submit_inline(
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

int RhFile_ReadAndCallbackByFileName(char *name, int byte_offset, int byte_size, int unknown_014,
                  void (*function)(int), int unknown_01c, int flag)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    char path[128];
    if (!name) {
        return 0;
    }
    func_0019b648(path, data_002d0686, SzFile_GetEnv(data_002d0680), name);
    state->unknown_6680 = (int)(DotrResourceSubmitWord)function;
    return dotr_resource_process_submit_inline(0x1400, flag ? 8 : 7, path, unknown_014, byte_size,
                                               byte_offset, byte_size, lRhFile_ReadCB, unknown_01c)
               ? 1 : 0;
}
