#include "dotr/ee/resource_helpers.h"

void SzFile_DoRequest(void)
{
    int start = *(volatile int *)0x10000800;
    do {
        if (!SzFile_DoRequestS()) {
            break;
        }
    } while (*(volatile int *)0x10000800 - start <= 64);
}
