#include "dotr/ee/key_tracks.h"
float lSzView_CalcInterpCameraMotion1(struct DotrKeySingle *keys, int n, float t)
{
    int lo;
    int mid;
    int hi;
    struct DotrKeySingle *a;
    struct DotrKeySingle *b;
    float d;
    float w0;
    float w1;

    mid = 0;
    if (n > 2) {
        lo = 0;
        hi = n;
        for (;;) {
            mid = (lo + hi) >> 1;
            if (lo >= mid) {
                break;
            }
            if (!(t < (float)keys[mid + 1].t)) {
                lo = mid;
                continue;
            }
            if (t < (float)keys[mid].t) {
                hi = mid;
                continue;
            }
            break;
        }
    }
    a = &keys[mid];
    if (mid + 1 >= n) {
        return a->v;
    }
    b = &keys[mid + 1];
    d = (float)(b->t - a->t);
    w0 = ((float)b->t - t) / d;
    w1 = (t - (float)a->t) / d;
    return w0 * a->v + w1 * b->v;
}
