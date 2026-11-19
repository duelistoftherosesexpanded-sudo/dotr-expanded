#include "dotr/ee/draw_state.h"

/* Written in word units so host and target agree on the layout. */
struct DotrDrawTag *RhVu1Mac_SetClipParameter(struct DotrDrawTag *tag, int mode, float x, float y, float z)
{
    float *f = (float *)tag;
    int *w = (int *)tag;
    float t;
    w[0] = 0x10000001;
    w[1] = 0;
    w[2] = 0;
    w[3] = 0x6c014052;
    t = 2.0f * x;
    f[4] = 645.0f / t;
    f[5] = -645.0f / t;
    f[6] = 0.01f + y;
    f[7] = z - 0.01f;
    w[8] = 0x10000001;
    w[9] = 0;
    w[10] = 0;
    w[11] = 0x6c01405a;
    w[12] = mode == 1 ? 1 : 0;
    w[13] = 0;
    w[14] = 0;
    w[15] = 0;
    return (struct DotrDrawTag *)(w + 16);
}
