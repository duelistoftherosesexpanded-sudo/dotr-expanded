#include "dotr/ee/resource_submit.h"

struct DotrResourceQueueRecord *SzFile_ReadRequest(int fileType, char *name, int unknown_014, int byte_size,
                                              DotrResourceSubmitCallback callback, int unknown_01c)
{
    return SzFile_ReadRequestParam(fileType, 0, name, unknown_014, byte_size, 0, -1, callback, unknown_01c);
}
