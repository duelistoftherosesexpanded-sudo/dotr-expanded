#include "dotr/ee/view_state.h"

int RhView_SetAspect(float a, float b)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    v->unknown_158 = a;
    v->unknown_15c = b;
    return 1;
}
