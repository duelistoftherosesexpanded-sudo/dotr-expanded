#include "dotr/ee/color_hls.h"

void SzColor_RGB2HLS(float *hls, int r, int g, int b, int max)
{
    /* MATCHING DEVICE: the channels are stored through named members and scanned as an
     * array; with one array view the compiler stores green through the scan pointer. */
    union DotrColorChannels t;
    int hi;
    int lo;
    int i;
    int sum;
    int diff;
    int mn;
    int mx;

    t.bytes = data_0029b2b0;
    t.rgb.r = r;
    t.rgb.g = g;
    t.rgb.b = b;
    hi = 0;
    lo = 0;
    for (i = 1; i < 3; i++) {
        if (t.bytes.v[hi] < t.bytes.v[i]) {
            hi = i;
        }
        if (t.bytes.v[i] < t.bytes.v[lo]) {
            lo = i;
        }
    }
    mx = t.bytes.v[hi];
    mn = t.bytes.v[lo];
    sum = mx + mn;
    diff = mx - mn;
    hls[0] = 0.0f;
    hls[1] = (float)sum / (float)((unsigned char)max * 2);
    hls[2] = hls[1];
    if (diff != 0 && hls[1] > 0.0f && hls[1] < 1.0f) {
        if (hls[1] <= 0.5f) {
            hls[2] = (float)diff / (float)sum;
        } else {
            hls[2] = (float)diff / (float)((unsigned char)max * 2 - sum);
        }
        hls[0] = (float)(hi * 2) + (float)(t.bytes.v[(hi + 1) % 3] - t.bytes.v[(hi + 2) % 3]) / (float)diff;
        if (hls[0] < 0.0f) {
            hls[0] += 6.0f;
        }
    }
}
