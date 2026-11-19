#include "dotr/ee/memory_ops.h"

DotrWord128 *RhMem_CopyMemory16(DotrWord128 *destination, DotrWord128 *source, int count)
{
    for (; count > 0; count--) {
        *destination++ = *source++;
    }
    return destination;
}
