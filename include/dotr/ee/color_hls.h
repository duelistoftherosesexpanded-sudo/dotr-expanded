#ifndef DOTR_EE_COLOR_HLS_H
#define DOTR_EE_COLOR_HLS_H

/* SzColor conversions from hue/lightness/saturation back to RGB bytes (0x2413f0), a callee of
 * SzColor_RGB2SEP (color_sep).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* The definitions' view of the two conversions. SzColor_RGB2HLS takes `max` as int; color_sep.h
 * declares the caller's view, in which SzColor_RGB2SEP passes an unsigned char `max` (same
 * register, already zero-extended); the two headers are therefore not included together.
 * SzColor_HLS2RGB takes its scale as an unsigned char (the same as color_sep.h's view).
 * func_001a22c0 (library code) is assumed to convert a float to an int. */
void SzColor_RGB2HLS(float *hls, int r, int g, int b, int max);
void SzColor_HLS2RGB(unsigned char *out, unsigned char value, float hue, float light, float sat);
extern int func_001a22c0(float value);

/* SzColor_RGB2HLS (0x241200) uses the low byte of each argument. Its three-byte local is first
 * copied from data_0029b2b0 (three bytes read as a halfword and a byte; contents not
 * interpreted) and then overwritten; the two views below are assumed layouts of that local,
 * not recovered types. */
struct DotrColorBytes {
    unsigned char v[3];
};
union DotrColorChannels {
    struct DotrColorBytes bytes;
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    } rgb;
};
extern struct DotrColorBytes data_0029b2b0;

#endif
