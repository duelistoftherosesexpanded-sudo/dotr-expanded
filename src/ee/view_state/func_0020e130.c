#include "dotr/ee/view_state.h"

int RhView_GetFogParameter(float *a, float *b, float *c, float *d, struct DotrViewQuad *q)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    *a = v->unknown_174;
    *b = v->unknown_178;
    *c = v->unknown_17c;
    *d = v->unknown_180;
    *q = v->unknown_184;
    return 1;
}
