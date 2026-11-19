#include "dotr/ee/resource_helpers.h"

int SzFile_EntryGroup(int fileType, int value)
{
    struct DotrResourcePair *current = FileGroupEntryArray;
    int remaining = 64;
    while (remaining--) {
        if (current->selector < 0) {
            current->selector = fileType;
            current->value = value;
            return 1;
        }
        current++;
    }
    return 0;
}
