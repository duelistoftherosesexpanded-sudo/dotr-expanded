#include "dotr/ee/view_state.h"
void Ex3d_GetSurfaceC(float *a, float *b, float *c, float *out)
{
    out[0] = a[1] * (b[2] - c[2]) + b[1] * (c[2] - a[2]) + c[1] * (a[2] - b[2]);
    out[1] = a[2] * (b[0] - c[0]) + b[2] * (c[0] - a[0]) + c[2] * (a[0] - b[0]);
    out[2] = a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1]);
    out[3] = a[0] * (b[2] * c[1] - b[1] * c[2]) + a[1] * (b[0] * c[2] - b[2] * c[0]) + a[2] * (b[1] * c[0] - b[0] * c[1]);
    out[4] = func_001a6c60(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
}
