#include "dotr/ee/view_state.h"

int RhView_SetFogParameter(float a, float b, float c, float d, struct DotrViewQuad *q)
{
    struct DotrViewEntry *v = &gRhView_avl[gRhView_nStackIndex[0]];
    v->unknown_174 = a;
    v->unknown_178 = b;
    v->unknown_17c = c;
    v->unknown_180 = d;
    v->unknown_184 = *q;
    return 1;
}
