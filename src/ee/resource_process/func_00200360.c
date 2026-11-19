#include "dotr/ee/resource_process.h"

int RhFile_OpenFile(char *name)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    char path[128];
    func_0019b648(path, data_002d0686, SzFile_GetEnv(data_002d0680), name);
    state->unknown_6684 = SzMem_StrDup(path);
    return 0;
}
