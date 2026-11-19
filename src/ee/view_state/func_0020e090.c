#include "dotr/ee/view_state.h"

int RhView_GetClipZ(float *a, float *b)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    *a = v->unknown_16c;
    *b = v->unknown_170;
    return 1;
}
