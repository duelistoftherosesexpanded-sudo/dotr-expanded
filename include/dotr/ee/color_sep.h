#ifndef DOTR_EE_COLOR_SEP_H
#define DOTR_EE_COLOR_SEP_H

/* SzColor colour conversions around 0x241200-0x241a00 (RGB to hue/lightness/saturation and back,
 * tinted output); observed callers and callees of SzColor_RGB2SEP.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* Assumed from the single call each in SzColor_RGB2SEP: three floats written through hls from
 * three components and their maximum; the reverse takes the floats in f12-f14. This is the
 * caller's view: `max` is passed as an unsigned char (the caller does not mask it again);
 * color_hls.h declares the definitions' int view, so the two headers are not included together. */
extern void SzColor_RGB2HLS(void *hls, int r, int g, int b, unsigned char max);
extern void SzColor_HLS2RGB(unsigned char *out, unsigned char max, float h, float l, float s);
/* Assumed layout of the three-float hue/lightness/saturation local (not a recovered type). */
struct DotrColorSepHls {
    float h;
    float l;
    float s;
};
/* SzColor_RGB2SEP: the float arrives in f12 beside six integer arguments. small_misc.h declares
 * the six-integer view its caller SzColor_CLUT2SEP uses (the float passes through that caller
 * untouched), so the two headers cannot be included together. */
void SzColor_RGB2SEP(unsigned char *out, int r, int g, int b, int mode, unsigned char max, float scale);

#endif
