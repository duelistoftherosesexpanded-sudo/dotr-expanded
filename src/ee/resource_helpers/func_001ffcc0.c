#include "dotr/ee/resource_helpers.h"

struct DotrResourcePair *SzFile_InqGroupX(int fileType, int grouped)
{
    int shift = grouped ? 10 : 0;
    struct DotrResourcePair *current = FileGroupEntryArray;
    int remaining = 64;
    while (remaining--) {
        if (current->selector >= 0) {
            if (fileType < 0 || fileType == (current->selector >> shift)) {
                return current;
            }
        }
        current++;
    }
    return 0;
}
