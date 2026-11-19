#ifndef DOTR_EE_UNSIGNED_CONVERT_H
#define DOTR_EE_UNSIGNED_CONVERT_H

/* Unsigned integer to float and double conversion helpers just below the library block that halve
 * values out of signed range and double the result.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Library-block conversion routines these helpers call (declared only; the library block
 * 0x17b48c-0x1a6c80 is outside the decompilation scope). Assumed from their use: 0x1a1f18
 * converts a signed 64-bit integer to float, 0x1a1e70 a signed 64-bit integer to double,
 * 0x1a2858 (dpadd) adds two doubles. */
extern float func_001a1f18_uconv(long v);
extern double func_001a1e70_uconv(long v);
extern double func_001a2858_uconv(double a, double b);

/* 0x17b340 _f_ulltof: unsigned 64-bit integer to float. Values that are positive as signed
 * are converted directly; otherwise the value is halved keeping its low bit (round to odd),
 * converted and doubled. */
float _f_ulltof(unsigned long x);
/* 0x17b380 _d_utod: unsigned 32-bit integer (zero-extended from the low half of the
 * register) to double, with the same halving path, the doubling done through dpadd. The
 * halving path is reached only for 0 after the zero-extension. MATCHING DEVICE: the
 * parameter is declared unsigned long and narrowed with two shifts (a cast adds an extra
 * dsll32/dsra32 pair). */
double _d_utod(unsigned long x);

#endif
