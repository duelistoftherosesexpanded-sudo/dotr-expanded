#include "dotr/ee/ring_pumps.h"

int pcmCallback(int unused, struct DotrPumpRequest *request, struct DotrPumpRing *ring)
{
    char *start1, *start2;
    int count1, count2;
    char *pos = request->position + 4;
    int cap = ring->size;
    char *end = ring->bytes + cap;
    int size;
    int first;
    int n;

    (void)unused;
    if (pos >= end) {
        pos -= cap;
    }
    first = end - pos;
    size = request->size - 4;
    if (size < first) {
        first = size;
    }
    func_00243390_pump(&data_00350620_pump, &start1, &count1, &start2, &count2);
    n = copy2area(start1, count1, start2, count2, pos, first, ring->bytes, size - first);
    func_00243450_pump(&data_00350620_pump, n);
    if (n > 0) {
        return 1;
    }
    return 0;
}
