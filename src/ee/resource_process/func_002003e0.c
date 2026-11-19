#include "dotr/ee/resource_process.h"

/* Submit-and-wait logic equal to 0x1ffd30/0x1ffe00 with 0x1fecb0 called, which
 * 0x2003e0 contains inline; the original source relationship is unknown.
 */
static inline struct DotrResourceQueueRecord *dotr_resource_process_submit_wait(
    int priority, char *name, int unknown_014, int byte_size, int byte_offset, int unknown_010)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    struct DotrResourceQueueRecord *record;
    record = SzFile_ReadRequestParam(0x1400, priority, name, unknown_014, byte_size, byte_offset,
                           unknown_010, lSzFile_ReadCB, 0);
    if (!record) {
        return 0;
    }
    while (SzFile_InqRequestX(-1, 0)) {
        SzFile_DoRequestS();
    }
    return state->flags.bit_0 ? record : 0;
}

int RhFile_ReadAndBlock(int unknown, int byte_offset, int byte_size, int unknown_014)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    if (unknown < 0) {
        return 0;
    }
    return dotr_resource_process_submit_wait(2, (char *)(DotrResourceSubmitWord)state->unknown_6684,
                                             unknown_014, byte_size, byte_offset, byte_size)
               ? 1 : 0;
}
