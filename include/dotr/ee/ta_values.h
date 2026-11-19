#ifndef DOTR_EE_TA_VALUES_H
#define DOTR_EE_TA_VALUES_H

/* Small Ta_ helpers that compute a number from their argument or from a card's kind and value
 * tables without side effects.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* pow (0x1a6be8, library block, declared only; assumed from the use). The double arithmetic
 * of 0x24c870 also makes the compiler call the library runtime routines litodp (0x1a2eb0),
 * dpadd (0x1a2858) and dptoli (0x1a2f68); they are not named in the C text. */
extern double func_001a6be8_ta(double x, double y);

/* The per-card tables kind_applies.h also reads: a kind byte at 0x29d1f0 and a value
 * halfword at 0x29cf32, both indexed by the card number. Bounds not established. */
extern unsigned char EnchantId_ta[];
extern unsigned short gAnmPat_Bakura_ta[];

/* 0x24c870 Ta_ConvBitLengthToNumericLength: the largest number bitLength bits hold,
 * 2^bitLength - 1, summed as retVal = (int)(retVal + pow(2, i)) for i = 0 .. bitLength - 1;
 * 0 for bitLength <= 0. MATCHING DEVICE: retVal is initialised after i in the for header. */
int Ta_ConvBitLengthToNumericLength(int bitLength);

/* 0x249200 Ta_GetEncEffect: a code chosen by the card's kind byte: kind 0 -> 537 if the value
 * is 0 else 536; kind 1 -> 540 if the value is 5 else 536; kind 2 -> 542; kind 3 -> 541;
 * any other kind -> 536. Meaning of the codes not established. MATCHING DEVICE: the outer
 * cases in source order 0, 3, 2, 1, default and inner switches for kinds 0 and 1 (if/else
 * or ?: give other shapes). */
int Ta_GetEncEffect(int no);

#endif
