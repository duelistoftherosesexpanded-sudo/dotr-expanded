#include "dotr/ee/draw_state.h"

int RhVu1Mac_GetPointLight(struct DotrDrawVector4 *a, struct DotrDrawVector4 *b, float *c, float *d, float *e)
{
    if (a) {
        *a = gRhVu1Mac_State.unknown_30;
    }
    if (b) {
        *b = gRhVu1Mac_State.unknown_40;
    }
    if (c) {
        *c = gRhVu1Mac_State.unknown_50;
    }
    if (d) {
        *d = gRhVu1Mac_State.unknown_54;
    }
    if (e) {
        *e = gRhVu1Mac_State.unknown_58;
    }
    return gRhVu1Mac_State.unknown_5c;
}
