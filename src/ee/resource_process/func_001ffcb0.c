#include "dotr/ee/resource_process.h"
struct DotrResourcePair *SzFile_InqGroup(int fileType)
{
    return SzFile_InqGroupX(fileType, 0);
}
