#include "dotr/ee/memory_ops.h"

long *RhMem_FillMemory8(long *destination, long value, int count)
{
    for (; count > 0; count--) {
        *destination++ = value;
    }
    return destination;
}
