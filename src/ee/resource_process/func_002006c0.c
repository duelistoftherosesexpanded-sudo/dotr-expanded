#include "dotr/ee/resource_process.h"

/* MATCHING DEVICE: the original enters its wait loop without a duplicated test,
 * which this compiler emits only when a conditional immediately precedes the
 * loop. A null check of the local buffer provides one and generates no code.
 * The original helper or source form is unknown.
 */
static inline int dotr_resource_process_wait(void *token)
{
    if (!token) {
        return 0;
    }
    while (SzFile_InqRequestX(-1, 0)) {
        SzFile_DoRequestS();
    }
    return 1;
}

int RhFile_ExistFile(char *name)
{
    char directory[128];
    char path[128];
    int descriptor;
    if (!name) {
        return 0;
    }
    func_0019b648(directory, data_002d0686, SzFile_GetEnv(data_002d0680), name);
    dotr_resource_process_wait(directory);
    lSzFile_StrNormalize(directory);
    func_0019b648(path, data_002d0648, data_002d064d, directory);
    descriptor = func_0017fa98(path, 1);
    if (descriptor < 0) {
        return 0;
    }
    func_0017fd20(descriptor);
    return 1;
}
