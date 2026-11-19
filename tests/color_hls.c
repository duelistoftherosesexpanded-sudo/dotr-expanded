/* Synthetic host tests for the color_hls group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/color_hls.h"

#include <stdio.h>

static int hls_calls;
/* Assumed semantics: truncation toward zero. */
int func_001a22c0(float value)
{
    hls_calls++;
    return (int)value;
}

static int hls_is(float h, float l, float s, int max, int r, int g, int b)
{
    unsigned char out[4];

    out[3] = 0x5a;
    SzColor_HLS2RGB(out, max, h, l, s);
    return out[0] == r && out[1] == g && out[2] == b && out[3] == 0x5a;
}

/* SzColor_HLS2RGB. One case per branch outcome: hue below 0 (one and two
 * additions), at and above 6 (one and two subtractions); lightness and saturation below 0,
 * inside and above 1; hue at or above and below the channel base; lightness at 0.5 and above
 * it; a result below and at the 255 limit; a scale below 255. Its float-to-int conversions are
 * casts, so the host counts no func_001a22c0 calls. */
static void check_hls2rgb(void)
{
    hls_calls = 0;
    assert(hls_is(0.0f, 0.5f, 1.0f, 255, 255, 0, 0));
    assert(hls_calls == 0);
    assert(hls_is(1.0f, 0.5f, 1.0f, 255, 255, 255, 0));
    assert(hls_is(2.0f, 0.5f, 1.0f, 255, 0, 255, 0));
    assert(hls_is(5.0f, 0.5f, 1.0f, 255, 255, 0, 255));
    assert(hls_is(-4.0f, 0.5f, 1.0f, 255, 0, 255, 0));
    assert(hls_is(-10.0f, 0.5f, 1.0f, 255, 0, 255, 0));
    assert(hls_is(6.0f, 0.5f, 1.0f, 255, 255, 0, 0));
    assert(hls_is(8.0f, 0.5f, 1.0f, 255, 0, 255, 0));
    assert(hls_is(14.0f, 0.5f, 1.0f, 255, 0, 255, 0));
    assert(hls_is(0.0f, -1.0f, 1.0f, 255, 0, 0, 0));
    assert(hls_is(0.0f, 1.0f, 1.0f, 255, 255, 255, 255));
    assert(hls_is(0.0f, 2.0f, 1.0f, 255, 255, 255, 255));
    assert(hls_is(0.0f, 0.25f, -1.0f, 255, 64, 64, 64));
    assert(hls_is(0.0f, 0.5f, 2.0f, 255, 255, 0, 0));
    assert(hls_is(0.0f, 0.75f, 1.0f, 255, 255, 128, 128));
    /* Hue below the base of the rounded channel: the interpolated value is negative and its
     * low byte is stored (observed arithmetic, not an intended colour). */
    assert(hls_is(1.75f, 0.5f, 1.0f, 255, 193, 255, 0));
    assert(hls_is(0.0f, 0.25f, 0.0f, 255, 64, 64, 64));
    assert(hls_is(0.0f, 0.5f, 1.0f, 100, 100, 0, 0));
    assert(hls_is(3.0f, 0.75f, 1.0f, 100, 50, 100, 100));
}

struct DotrColorBytes data_0029b2b0;

static int rgb_is(int r, int g, int b, int max, float h, float l, float s)
{
    float out[4];

    out[3] = 9.0f;
    SzColor_RGB2HLS(out, r, g, b, max);
    return out[0] == h && out[1] == l && out[2] == s && out[3] == 9.0f;
}

/* SzColor_RGB2HLS. One case per branch outcome: no channel difference (grey, black);
 * the largest channel first, second and third; the smallest first, second and third;
 * lightness at 0.5 and above it; lightness at 1 with a difference (hue and saturation left
 * alone); a negative hue wrapped by 6; `max` wider than a byte. A difference with lightness
 * at or below 0 cannot be built from byte channels and is not run. */
static void check_rgb2hls(void)
{
    assert(rgb_is(10, 10, 10, 255, 0.0f, 20.0f / 510.0f, 20.0f / 510.0f));
    assert(rgb_is(0, 0, 0, 255, 0.0f, 0.0f, 0.0f));
    assert(rgb_is(255, 0, 0, 255, 0.0f, 0.5f, 1.0f));
    assert(rgb_is(0, 255, 0, 255, 2.0f, 0.5f, 1.0f));
    assert(rgb_is(0, 0, 255, 255, 4.0f, 0.5f, 1.0f));
    assert(rgb_is(255, 255, 128, 255, 1.0f, 383.0f / 510.0f, 1.0f));
    assert(rgb_is(200, 100, 0, 100, 0.0f, 1.0f, 1.0f));
    assert(rgb_is(255, 0, 128, 255, -128.0f / 255.0f + 6.0f, 0.5f, 1.0f));
    assert(rgb_is(0x1ff, 0x100, 0x200, 0x1ff, 0.0f, 0.5f, 1.0f));
}

int main(void)
{
    check_rgb2hls();
    check_hls2rgb();
    puts("color_hls: ok");
    return 0;
}
