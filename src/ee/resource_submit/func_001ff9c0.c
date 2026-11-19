#include "dotr/ee/resource_submit.h"

static inline struct DotrResourceQueueRecord *dotr_resource_submit_record(
    struct DotrResourceSubmitState *state, int index)
{
    struct DotrResourceQueueRecord *record = &state->records[index];
    return record;
}

int lSzFile_NextRequest(void)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    int best = -1;
    int best_priority = -1;
    int i;
    while (state->count) {
        if (state->records[state->head].status >= 0) {
            break;
        }
        if (--state->count > 0) {
            state->head = (state->head + 1) % 256;
        }
    }
    for (i = 0; i < state->count; i++) {
        int index = (state->head + i) % 256;
        if (dotr_resource_submit_record(state, index)->status >= 0) {
            if (best_priority < dotr_resource_submit_record(state, index)->priority) {
                best_priority = dotr_resource_submit_record(state, index)->priority;
                best = index;
            }
        }
    }
    return best;
}
