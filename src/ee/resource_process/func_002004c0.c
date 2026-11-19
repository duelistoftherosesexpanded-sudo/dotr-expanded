#include "dotr/ee/resource_process.h"

int RhFile_CloseFile(int unknown)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    if (unknown < 0) {
        return 0;
    }
    if (state->unknown_6684) {
        SzMem_Free(state->unknown_6684);
        state->unknown_6684 = 0;
    }
    return 1;
}
