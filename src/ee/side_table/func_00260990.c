#include "dotr/ee/preset_words.h"

/* Converts block 0 to four words through SzView_V2IV, adds delta to the
 * second word (subtracting 65535 when the magnitude of the sum reaches
 * 65535), converts back through SzView_IV2V and submits the result with
 * SzView_ReqCamera(0, 0, 4, b, 0, out). */
void TaView_ReqRotCamera(unsigned char b, int delta)
{
    int buffer[4];
    float out[4];
    int *v = buffer;
    int t;
    int n;
    SzView_FreeRequest(0, 0);
    SzView_V2IV(func_001faf60_floats(0), v);
    if (v[1] + delta >= 0) {
        t = v[1] + delta;
    } else {
        t = -(v[1] + delta);
    }
    if (t >= 0xffff) {
        n = v[1] + delta - 32768 - 32767;
    } else {
        n = v[1] + delta;
    }
    v[1] = n;
    SzView_IV2V(v, out);
    SzView_ReqCamera(0, 0, 4, b, 0, out);
}
