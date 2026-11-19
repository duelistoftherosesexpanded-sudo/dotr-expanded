#include "dotr/ee/color_sep.h"

/* Converts r, g, b to hue/lightness/saturation, replaces the hue by the low three bits of mode
 * (rotated by three steps when bit 3 asks for the inverse), scales the saturation by scale or
 * clears it for hue 6, converts back into out and keeps every byte at 1 or above.
 * MATCHING DEVICE: the three floats are a structure local, the low byte of mode is taken once
 * into `flags`, the hue is stored from the unsigned value, and `max` is an unsigned char (with an
 * int `max` the function masks it again before the first call, which the original does not). */
void SzColor_RGB2SEP(unsigned char *out, int r, int g, int b, int mode, unsigned char max, float scale)
{
    struct DotrColorSepHls hls;
    int inv;
    int gray;
    int hue;
    unsigned char flags;

    flags = mode;
    if (flags & 8) {
        inv = 1;
    } else {
        inv = 0;
    }
    hue = flags & 7;
    if (hue == 6) {
        gray = 1;
    } else {
        gray = 0;
    }
    SzColor_RGB2HLS(&hls, r, g, b, max);
    if (hue < 7) {
        mode &= 7;
        if (inv) {
            mode = (unsigned char)(((unsigned char)mode + 3) % 6);
        }
        hls.h = (unsigned int)mode;
    }
    if (gray) {
        hls.s = 0.0f;
    } else {
        hls.s = hls.s * scale;
    }
    SzColor_HLS2RGB(out, max, hls.h, hls.l, hls.s);
    if (inv) {
        out[0] = max - out[0];
        out[1] = max - out[1];
        out[2] = max - out[2];
    }
    out[0] = (out[0] > 1) ? out[0] : 1;
    out[1] = (out[1] > 1) ? out[1] : 1;
    out[2] = (out[2] > 1) ? out[2] : 1;
}
