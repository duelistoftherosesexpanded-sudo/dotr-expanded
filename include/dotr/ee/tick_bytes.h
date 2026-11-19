#ifndef DOTR_EE_TICK_BYTES_H
#define DOTR_EE_TICK_BYTES_H

/* Five adjacent small leaves at 0x101af0-0x101c74: two identical two-byte countdown steps, their
 * three-byte setter, a mode-5 dispatcher into 0x1f5620 and a strided zero-byte search.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* View of the object 0x101ba0 reads: only +5 and +6 are observed. */
struct DotrTickSource {
    unsigned char unknown_00[5];
    unsigned char mode;
    unsigned char value;
};

/* Callees of 0x101ba0 with the signatures packed_item.h declares; the vector
 * type is left incomplete here. */
struct DotrObjectVector4;
extern int SzEffect_GetFrom(void);
extern int SzEffect_GetTarget(void);
extern int SzEffect_GetRef2(int packed, int value, struct DotrObjectVector4 *v);

/* 0 when p[0] is 0. Otherwise counts p[1] down and answers 0 while it is not
 * 0; when p[1] is 0, counts p[0] down and answers 1.
 * Assumed: byte pointer, int result. 0x101b50 has the same bytes. */
int ShFx_isTimer(unsigned char *p);
int ShFx_isPartsTimer(unsigned char *p);

/* Stores a at p[2] and p[0] and b at p[1], in that order.
 * Assumed: byte pointer, int a and b, no result. */
void ShFx_initPartsTimer(unsigned char *p, int a, int b);

/* By source->mode: 1 calls SzEffect_GetRef2(SzEffect_GetFrom(), source->value, v), 2 calls
 * SzEffect_GetRef2(SzEffect_GetTarget(), source->value, v); 0 and every other value do nothing.
 * Assumed: the views above, no result (the callee's result is not used). */
void ShFx_AdjustPoint(struct DotrTickSource *source, struct DotrObjectVector4 *v);

/* Index of the first zero byte among p[0], p[stride], ... (n of them), or -1.
 * Assumed: byte pointer, int stride, n and result. */
int ShFx_GetTempIndex(unsigned char *p, int stride, int n);

#endif
