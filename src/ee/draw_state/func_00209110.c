#include "dotr/ee/draw_state.h"
struct DotrDrawTag *RhVu1Mac_SetPointLightToVu1(struct DotrDrawTag *tag, float *a, float *b, float x, float y, float z)
{
    float *f = (float *)tag;
    float t;
    if (a == 0 || b == 0) {
        ((int *)f)[0] = 0x10000002;
        ((int *)f)[1] = 0;
        ((int *)f)[2] = 0x11000000;
        ((int *)f)[3] = 0x6c020054;
        RhMem_FillMemory8((long *)(f + 4), 0, 4);
        f += 12;
        gRhVu1Mac_State.unknown_5c = 0;
    } else {
        ((int *)f)[0] = 0x10000002;
        ((int *)f)[1] = 0;
        ((int *)f)[2] = 0x11000000;
        ((int *)f)[3] = 0x6c020054;
        t = z * y;
        f[4] = a[1];
        f[5] = a[2];
        f[6] = a[3];
        f[7] = t / (y - x);
        f[8] = b[0];
        f[9] = b[1];
        f[10] = b[2];
        f[11] = z / (y - x);
        f += 12;
        RhMem_CopyMemory4((int *)&gRhVu1Mac_State.unknown_30, (int *)a, 4);
        RhMem_CopyMemory4((int *)&gRhVu1Mac_State.unknown_40, (int *)b, 4);
        gRhVu1Mac_State.unknown_50 = x;
        gRhVu1Mac_State.unknown_54 = y;
        gRhVu1Mac_State.unknown_58 = z;
        gRhVu1Mac_State.unknown_5c = 1;
    }
    return (struct DotrDrawTag *)f;
}

