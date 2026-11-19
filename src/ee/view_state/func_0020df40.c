#include "dotr/ee/view_state.h"

int RhView_SetScreen(float a, float b, float c, float d, float e)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    v->unknown_160 = a;
    v->unknown_164 = b;
    v->unknown_168 = c;
    v->unknown_150 = d;
    v->unknown_154 = e;
    return 1;
}
