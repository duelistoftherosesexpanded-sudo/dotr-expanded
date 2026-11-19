#include "dotr/ee/resource_helpers.h"

struct DotrResourceQueueRecord *SzFile_InqRequestX(int selector, int grouped)
{
    int shift = grouped ? 10 : 0;
    struct DotrResourceQueueRecord *current = ReadRequestQueueArray;
    int remaining = 256;
    while (remaining--) {
        if (current->status >= 0) {
            if (selector < 0 || selector == (current->selector >> shift)) {
                return current;
            }
        }
        current++;
    }
    return 0;
}
