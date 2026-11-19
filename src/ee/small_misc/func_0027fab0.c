#include "dotr/ee/small_misc.h"
void TaTuto_BkupOriginalView(float *out)
{
    float *a = SzView_GetCP(0);
    float *b = func_001faf60_floats(0);
    out[0] = a[0];
    out[1] = a[1];
    out[2] = a[2];
    out[3] = a[3];
    out[4] = b[0];
    out[5] = b[1];
    out[6] = b[2];
    out[7] = b[3];
}
