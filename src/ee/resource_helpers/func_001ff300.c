#include "dotr/ee/resource_helpers.h"

int SzFile_InqRequestEx(void)
{
    int selector;
    for (selector = 0; selector < 5; selector++) {
        if (SzFile_InqRequestX(selector, 1)) {
            return 1;
        }
    }
    return 0;
}
