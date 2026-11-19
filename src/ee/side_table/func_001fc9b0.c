#include "dotr/ee/resource_process.h"

int SzView_InitCamera(int currentMode)
{
    char filePath[128];
    struct DotrLabelRecord *readRequest;
    struct DotrLabelHeader *h;

    SzView_TermCamera();
    func_0019b648(filePath, data_002cfc50, SzFile_GetEnv(data_002cfc67), currentMode, currentMode);
    readRequest = (struct DotrLabelRecord *)SzFile_ReadParam(1, filePath, 0, -1, 0, -1);
    if (readRequest == 0)
        return 0;
    if (readRequest->byte_size == 0 || (h = readRequest->header) == 0)
        return 0;
    gSzView_pCamera_header[0] = h;
    gSzView_wFrame[0] = h->unknown_0c;
    return 1;
}
