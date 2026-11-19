#include "dotr/ee/preset_words.h"

/* After SzView_FreeRequest(0, 0), doubles the first float of SzView_GetLook(0) and submits
 * SzView_GetCP(0) with kind 3 and that look vector with kind 4 (SzView_ReqCamera), both
 * tagged b. */
void TaView_ReqHalfFarView(unsigned char b)
{
    void *p;
    float *q;
    SzView_FreeRequest(0, 0);
    p = func_001f9d30_floats(0);
    q = func_001faf60_floats(0);
    *q = 2.0f * *q;
    func_001fa8a0_tag(0, 0, 3, b, 0, p);
    func_001fa8a0_tag(0, 0, 4, b, 0, q);
}
