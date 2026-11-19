#include "dotr/ee/resource_submit.h"

struct DotrResourceQueueRecord *SzFile_Read(char *name, int unknown_014, int byte_size)
{
    struct DotrResourceQueueRecord *record;
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    record = SzFile_ReadRequestParam(0x1400, 0, name, unknown_014, byte_size, 0, -1, lSzFile_ReadCB, 0);
    if (!record) {
        return 0;
    }
    while (SzFile_InqRequestX(-1, 0)) {
        SzFile_DoRequestS();
    }
    return state->flags.bit_0 ? record : 0;
}
