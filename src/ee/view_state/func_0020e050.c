#include "dotr/ee/view_state.h"

int RhView_GetScreenDist(float *a)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    *a = v->unknown_160;
    return 1;
}
