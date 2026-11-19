#ifndef DOTR_EE_SPAN_RING_H
#define DOTR_EE_SPAN_RING_H

/* Functions on a ring of 24-byte span records (owner: pointer at +0x50, capacity +0x54, count
 * +0x58, tail +0x5c).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Span record and owner views (placeholder names); offsets observed in 0x244b70, member names
 * assumed. `long` is the 64-bit type of the pinned compiler (the function reads the first
 * doubleword signed and stores -1 to both). Host pointers are wider, so host tests check
 * behaviour, not layout. */
typedef struct DotrSpanRecord {
    long first;
    long second;
    int pos;
    int len;
} DotrSpanRecord;
typedef struct DotrSpanRing {
    int unknown_00[2];
    int unit;
    int unknown_0c[13];
    int unknown_40; /* passed to 0x17b920 / 0x17b900 around 0x244cb0's work */
    int unknown_44[3];
    DotrSpanRecord *records;
    int capacity;
    int count;
    int tail;
} DotrSpanRing;

extern int viBufModifyPts(DotrSpanRing *ring, DotrSpanRecord *span);
/* SDK-block routines bracketing 0x244cb0; one word argument, result unused (assumed). */
extern void func_0017b920(int unknown);
extern void func_0017b900(int unknown);
extern int viBufPutTs(DotrSpanRing *ring, DotrSpanRecord *span);

/* viBufGetTs (0x244de0; the name is the Japanese build's). Reads two hardware registers at
 * literal addresses before taking the lock: DMA channel 4 MADR (0x1000b410) and IPU_BP
 * (0x10002020); the host build gives each an object. Owner word +0 is taken as the start
 * address of the data area and the word at +0x38 as a saved bit position (low seven bits
 * used); both are read through unknown_00 and unknown_0c. small_misc.h declares this
 * function for its caller with a void result and untyped pointers. */
#ifdef __MWERKS__
#define DOTR_SPAN_D4_MADR (*(volatile unsigned *)0x1000b410)
#define DOTR_SPAN_IPU_BP (*(volatile unsigned *)0x10002020)
#else
extern unsigned dotr_span_d4_madr_1000b410;
extern unsigned dotr_span_ipu_bp_10002020;
#define DOTR_SPAN_D4_MADR dotr_span_d4_madr_1000b410
#define DOTR_SPAN_IPU_BP dotr_span_ipu_bp_10002020
#endif
int viBufGetTs(DotrSpanRing *ring, DotrSpanRecord *out);

#endif
