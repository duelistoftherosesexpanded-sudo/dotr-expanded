#include "dotr/ee/memory_ops.h"

unsigned char *RhMem_FillMemory(unsigned char *destination, int value, int count)
{
    for (; count > 0; count--) {
        *destination++ = value;
    }
    return destination;
}
