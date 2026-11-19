#include "dotr/ee/small_misc.h"

int *RhZSort_Finish_Pipe(int *cursor)
{
    struct DotrMiscSortList *list = &gRhZSort_zsl_list_view;
    struct DotrMiscSortEntry *e;
    int *out = cursor;

    if (gRhZSort_zsl_list.first == gRhZSort_zsl_list.end) {
        return cursor;
    }
    RhMath_HeapSortStruct(list->first, ((DotrMiscAddress)list->end - (DotrMiscAddress)list->first) >> 3, 8, lRhZSort_ZCompare);
    for (e = list->first; e < list->end; e++) {
        out[0] = 0x50000000;
        out[1] = e->value;
        out[2] = 0;
        out[3] = 0;
        out += 4;
    }
    list->out = out;
    list->first = list->end;
    list->sorted = 1;
    return out;
}
