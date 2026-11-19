#include "dotr/ee/memory_ops.h"

int *RhMem_CopyMemory4(int *destination, int *source, int count)
{
    for (; count > 0; count--) {
        *destination++ = *source++;
    }
    return destination;
}
