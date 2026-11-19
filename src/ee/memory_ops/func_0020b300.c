#include "dotr/ee/memory_ops.h"

unsigned char *RhMem_CopyMemory(unsigned char *destination, unsigned char *source, int count)
{
    for (; count > 0; count--) {
        *destination++ = *source++;
    }
    return destination;
}
