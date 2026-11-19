#include "dotr/ee/draw_state.h"

int RhBase_GetUserGsAddress(void)
{
    struct DotrDrawState *r = &gRhBase_dl;
    struct DotrDrawSelector *s = &gRhBase_rl;
    return r->unknown_268 + ((data_00325f40[0].unknown_bit_0 ? s->unknown_10 : 0) << 5);
}
