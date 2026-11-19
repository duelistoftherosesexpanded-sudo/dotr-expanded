#include "dotr/ee/memory_ops.h"

int *RhMem_FillMemory4(int *destination, int value, int count)
{
    for (; count > 0; count--) {
        *destination++ = value;
    }
    return destination;
}
