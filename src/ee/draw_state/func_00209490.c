#include "dotr/ee/draw_state.h"

/* Written in word units so host and target agree on the layout. */
struct DotrDrawTag *RhVu1Mac_SetFogOnOff(struct DotrDrawTag *tag, int a, int b)
{
    float *f = (float *)tag;
    int *w = (int *)tag;
    w[0] = 0x10000001;
    w[1] = 0;
    w[2] = 0x10000000;
    w[3] = 0x6c014053;
    if (a) {
        f[6] = gRhVu1Mac_State.unknown_10[0];
        f[7] = gRhVu1Mac_State.unknown_10[1];
    } else {
        f[6] = 0.0f;
        f[7] = 255.0f;
    }
    if (b) {
        f[4] = gRhVu1Mac_State.unknown_10[2];
        f[5] = gRhVu1Mac_State.unknown_10[3];
    } else {
        f[4] = 0.0f;
        f[5] = 0.0f;
    }
    return (struct DotrDrawTag *)(w + 8);
}
