#include "dotr/ee/color_hls.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

#define MIN_002413F0(a, b) ((a) < (b) ? (a) : (b))
#define MAX_002413F0(a, b) ((a) > (b) ? (a) : (b))

/* SzColor_HLS2RGB: converts hue (wrapped into [0, 6)), lightness and saturation (each clamped
 * to [0, 1]) to three bytes scaled by `value`. The rounded hue picks the full channel (two hue
 * units per channel); the other two are the low channel and the one interpolated by the hue's
 * distance from that channel's base. Each result is rounded (+0.5, float-to-int cast) and
 * limited to 255. Compiled warm (the casts are float-to-int conversions). */
void SzColor_HLS2RGB(unsigned char *out, unsigned char value, float hue, float light, float sat)
{
    float color[3];
    float frac;
    int max;
    int mid;
    int low;

    while (hue < 0.0f) {
        hue += 6.0f;
    }
    while (hue >= 6.0f) {
        hue -= 6.0f;
    }
    light = MIN_002413F0(MAX_002413F0(light, 0.0f), 1.0f);
    sat = MIN_002413F0(MAX_002413F0(sat, 0.0f), 1.0f);
    max = ((int)(hue + 0.5f) % 6) >> 1;
    if (hue >= (float)(max * 2)) {
        mid = (max + 1) % 3;
        low = (max + 2) % 3;
    } else {
        low = (max + 1) % 3;
        mid = (max + 2) % 3;
    }
    if (light <= 0.5f) {
        color[low] = value * (light * (1.0f - sat));
        color[max] = light * (value * 2) - color[low];
    } else {
        color[max] = value * (sat + light * (1.0f - sat));
        color[low] = light * (value * 2) - color[max];
    }
    frac = hue - (float)(max * 2);
    color[mid] = color[low] + frac * (color[max] - color[low]);
    out[0] = MIN_002413F0((int)(color[0] + 0.5f), 0xFF);
    out[1] = MIN_002413F0((int)(color[1] + 0.5f), 0xFF);
    out[2] = MIN_002413F0((int)(color[2] + 0.5f), 0xFF);
}
