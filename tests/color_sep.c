/* Synthetic host tests for the color_sep group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/color_sep.h"

static int hls_args[4], hls_calls, rgb_max, rgb_calls;
static float hls_in[3], rgb_hls[3];
static unsigned char rgb_out[3];
void SzColor_RGB2HLS(void *hls, int r, int g, int b, unsigned char max)
{
    hls_args[0] = r;
    hls_args[1] = g;
    hls_args[2] = b;
    hls_args[3] = max;
    hls_calls++;
    memcpy(hls, hls_in, sizeof(hls_in));
}
void SzColor_HLS2RGB(unsigned char *out, unsigned char max, float h, float l, float s)
{
    rgb_max = max;
    rgb_hls[0] = h;
    rgb_hls[1] = l;
    rgb_hls[2] = s;
    rgb_calls++;
    memcpy(out, rgb_out, 3);
}

static void run(int mode, unsigned char *out)
{
    hls_in[0] = 0.25f;
    hls_in[1] = 0.5f;
    hls_in[2] = 0.75f;
    SzColor_RGB2SEP(out, 1, 2, 3, mode, 31, 2.0f);
    assert(hls_args[0] == 1 && hls_args[1] == 2 && hls_args[2] == 3 && hls_args[3] == 31 && rgb_max == 31);
    assert(rgb_hls[1] == 0.5f);
}

/* SzColor_RGB2SEP (branch coverage): hue 7 keeps the converted hue, hue below 7 replaces it, hue 6
 * clears the saturation, bit 3 rotates the hue and inverts the output, each byte clamps at 1. */
static void check_rgb2sep(void)
{
    unsigned char out[4] = {9, 9, 9, 0x5a};

    rgb_out[0] = 0;
    rgb_out[1] = 1;
    rgb_out[2] = 2;
    run(7, out);
    assert(rgb_hls[0] == 0.25f && rgb_hls[2] == 1.5f && out[0] == 1 && out[1] == 1 && out[2] == 2);
    run(0x103, out);
    assert(rgb_hls[0] == 3.0f && rgb_hls[2] == 1.5f && out[0] == 1 && out[1] == 1 && out[2] == 2);
    run(6, out);
    assert(rgb_hls[0] == 6.0f && rgb_hls[2] == 0.0f && out[2] == 2);
    run(0, out);
    assert(rgb_hls[0] == 0.0f && rgb_hls[2] == 1.5f);

    /* Inverse: hue (2 + 3) % 6 and (4 + 3) % 6; bytes become 31 - byte, then clamp. */
    rgb_out[0] = 31;
    rgb_out[1] = 30;
    rgb_out[2] = 4;
    run(8 | 2, out);
    assert(rgb_hls[0] == 5.0f && rgb_hls[2] == 1.5f && out[0] == 1 && out[1] == 1 && out[2] == 27);
    run(8 | 4, out);
    assert(rgb_hls[0] == 1.0f && out[2] == 27);
    /* Inverse with hue 6: (6 + 3) % 6 = 3, saturation cleared; with hue 7 the hue stays. */
    run(8 | 6, out);
    assert(rgb_hls[0] == 3.0f && rgb_hls[2] == 0.0f && out[0] == 1 && out[2] == 27);
    run(8 | 7, out);
    assert(rgb_hls[0] == 0.25f && rgb_hls[2] == 1.5f && out[1] == 1 && out[2] == 27);
    assert(out[3] == 0x5a && hls_calls == 8 && rgb_calls == 8);

    /* Only the low byte of mode is read for the flags and the hue (0x10a: inverse, hue 2). */
    run(0x10a, out);
    assert(rgb_hls[0] == 5.0f && out[2] == 27);
    /* max is an unsigned char: 255 is forwarded to both conversions and used for the inverse. */
    rgb_out[0] = 255;
    rgb_out[1] = 200;
    rgb_out[2] = 0;
    SzColor_RGB2SEP(out, 4, 5, 6, 8 | 7, 255, 0.5f);
    assert(hls_args[3] == 255 && rgb_max == 255 && rgb_hls[2] == 0.375f);
    assert(out[0] == 1 && out[1] == 55 && out[2] == 255);
}

int main(void)
{
    check_rgb2sep();
    puts("color_sep: ok");
    return 0;
}
