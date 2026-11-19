#include "dotr/ee/small_misc.h"

void voBufCreate(struct DotrMiscQueue *queue, DotrMiscAddress a, DotrMiscAddress b, int capacity)
{
    int i;
    queue->unknown_00 = a;
    queue->unknown_04 = b;
    queue->unknown_10 = capacity;
    queue->unknown_0c = 0;
    queue->unknown_08 = 0;
    for (i = 0; i < capacity; i++) {
        *(int *)(queue->unknown_04 + i * 0x4ce40) = 0;
    }
}
