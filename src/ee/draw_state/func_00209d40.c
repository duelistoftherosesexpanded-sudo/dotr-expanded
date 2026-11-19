#include "dotr/ee/draw_state.h"

float RhBase_GetAspectY(int *out)
{
    struct DotrDrawState *r = &gRhBase_dl;
    if (out) {
        *out = r->unknown_248;
    }
    return r->unknown_244;
}
