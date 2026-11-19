#include "dotr/ee/resource_submit.h"

void SzFile_Init(void)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    struct DotrResourceQueueRecord *record = ReadRequestQueueArray;
    struct DotrResourcePair *pair = FileGroupEntryArray;
    int remaining = 256;
    RhMem_FillMemory(&dotr_ee_resource_submit_state_0031c2f0, 0, 0x66a0);
    while (remaining--) {
        record->status = -1;
        record++;
    }
    remaining = 64;
    while (remaining--) {
        pair->selector = -1;
        pair++;
    }
    state->head = 0;
    state->count = 0;
    state->cached_result = -1;
    state->flags.bit_1 = 0;
}
