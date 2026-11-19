#include "dotr/ee/view_state.h"

int RhView_SetClipZ(float a, float b)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    v->unknown_16c = a;
    v->unknown_170 = b;
    return 1;
}
