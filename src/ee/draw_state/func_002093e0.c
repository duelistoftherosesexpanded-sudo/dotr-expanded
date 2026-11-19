#include "dotr/ee/draw_state.h"

/* Written in word units so host and target agree on the layout. */
struct DotrDrawTag *RhVu1Mac_SetFogParameterEx(struct DotrDrawTag *tag, float a, float b, float c, float d)
{
    float *f = (float *)tag;
    int *w = (int *)tag;
    float sx, sy;
    w[0] = 0x10000001;
    w[1] = 0;
    w[2] = 0x10000000;
    w[3] = 0x6c014053;
    sy = 255.0f / (d - c);
    sx = 255.0f / (b - a);
    f[4] = sy;
    f[5] = c * sy;
    f[6] = sx;
    f[7] = b * sx;
    gRhVu1Mac_State.unknown_10[0] = f[6];
    gRhVu1Mac_State.unknown_10[1] = f[7];
    gRhVu1Mac_State.unknown_10[2] = f[4];
    gRhVu1Mac_State.unknown_10[3] = f[5];
    gRhVu1Mac_State.unknown_20[0] = a;
    gRhVu1Mac_State.unknown_20[1] = b;
    gRhVu1Mac_State.unknown_20[2] = c;
    gRhVu1Mac_State.unknown_20[3] = d;
    return (struct DotrDrawTag *)(w + 8);
}
