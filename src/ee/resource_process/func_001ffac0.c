#include "dotr/ee/resource_process.h"

void lSzFile_TermRequest(int index, int completed)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    struct DotrResourceSubmitRecord *record =
        (struct DotrResourceSubmitRecord *)&dotr_ee_resource_submit_state_0031c2f0.records[index];
    struct DotrResourcePair *pair;
    if (record->callback) {
        record->callback(record, completed);
    }
    if (record->status >= 0) {
        if (!record->sector_mode) {
            func_0017fd20(record->status);
        }
        record->status = -1;
    }
    if (record->handle) {
        SzMem_Free(record->handle);
        record->handle = 0;
    }
    if (!completed && record->flag_00c && record->unknown_014) {
        SzMem_Free(record->unknown_014);
    }
    if (!SzFile_InqRequestX(record->selector, 0)) {
        if ((pair = SzFile_InqGroup(record->selector)) != 0) {
            if (pair->value) {
                ((void (*)(int))(DotrResourceSubmitWord)pair->value)(record->selector);
            }
            pair->selector = -1;
        }
    }
    if (state->head == index && state->count) {
        if (--state->count > 0) {
            state->head = (state->head + 1) % 256;
        }
    }
    state->flags.bit_1 = 0;
}
