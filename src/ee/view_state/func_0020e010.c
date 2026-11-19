#include "dotr/ee/view_state.h"

int RhView_GetScreenCenter(float *a, float *b)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    *a = v->unknown_150;
    *b = v->unknown_154;
    return 1;
}
