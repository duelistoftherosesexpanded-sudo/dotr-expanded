#include "dotr/ee/memory_ops.h"

void RhMath_HeapSortStruct(unsigned char *base, int count, int size, int (*compare)(unsigned char *, unsigned char *))
{
    int i;
    int j;
    unsigned char *b;
    unsigned char *p;
    unsigned char *q;
    unsigned char *first;
    int l;
    unsigned char *tmp = data_003261f0;

    b = base - size;
    for (l = count >> 1; l > 0; l--) {
        i = l;
        RhMem_CopyMemory(tmp, b + l * size, size);
        for (; (j = i * 2) <= count; i = j) {
            if (j < count && compare(b + j * size, b + (j + 1) * size) < 0) {
                j++;
            }
            p = b + j * size;
            if (compare(tmp, p) >= 0) {
                break;
            }
            RhMem_CopyMemory(b + i * size, p, size);
        }
        RhMem_CopyMemory(b + i * size, tmp, size);
    }
    while (count > 1) {
        first = b + count * size;
        RhMem_CopyMemory(tmp, first, size);
        RhMem_CopyMemory(first, b + size, size);
        count--;
        for (i = 1; (j = i * 2) <= count; i = j) {
            if (j < count && compare(b + j * size, b + (j + 1) * size) < 0) {
                j++;
            }
            q = b + j * size;
            if (compare(tmp, q) >= 0) {
                break;
            }
            RhMem_CopyMemory(b + i * size, q, size);
        }
        RhMem_CopyMemory(b + i * size, tmp, size);
    }
}
