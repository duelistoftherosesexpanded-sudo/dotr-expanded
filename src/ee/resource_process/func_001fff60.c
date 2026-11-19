#include "dotr/ee/resource_process.h"

/* Installed as the record callback by the wait routines; `record` is unused. */
void lSzFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    dotr_ee_resource_submit_state_0031c2f0.flags.bit_0 = (char)completed;
}
