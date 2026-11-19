#ifndef DOTR_EE_MOTION_ITEMS_H
#define DOTR_EE_MOTION_ITEMS_H

/* lRhModel motion-item functions near 0x2106e0 that interpolate a model node's scale, rotation
 * and position through the scratchpad work block at 0x70000000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* One set of interpolated values: three quadwords, as the offsets of 0x2106e0 show. Which
 * member is the scale, rotation and position follows the order of the output pointers. */
struct DotrMotionValues {
    float scale[4];
    int rotation[4];             /* words: 0x210cf0 fills them from an integer-returning callee */
    float position[4];
};
/* The scratchpad at 0x70000000 as 0x2106e0 uses it: an initial set copied over two work sets.
 * Every access in the original is absolute (lui at,0x7000), so the target build uses a literal
 * address; the host build points the name at an object the test defines. */
struct DotrMotionPad {
    struct DotrMotionValues initial; /* +0x00 */
    struct DotrMotionValues first;   /* +0x30 */
    struct DotrMotionValues second;  /* +0x60 */
    unsigned char unknown_90[0x50];
    unsigned short flags;            /* +0xe0: bit 2 = items carry a third (scale) track */
};
/* A key of a position or scale track and of a rotation track, 16 bytes each (0x210cf0). */
struct DotrMotionKey {
    unsigned int time;
    float value[3];
};
struct DotrMotionAngleKey {
    unsigned int time;
    int value[3];
};
/* A motion item as 0x210cf0 reads it: track pointers (position, rotation, and scale when bit 2
 * of the pad flags is set) followed by one key count per track. The rotation track holds
 * DotrMotionAngleKey records. */
struct DotrMotionItem {
    struct DotrMotionKey *keys[3];
    unsigned int counts[3];
};
struct DotrMotionShortItem {
    struct DotrMotionKey *keys[2];
    unsigned int counts[2];
};
#ifdef __MWERKS__
#define DOTR_MOTION_PAD ((struct DotrMotionPad *)0x70000000)
#else
extern struct DotrMotionPad dotr_motion_pad;
#define DOTR_MOTION_PAD (&dotr_motion_pad)
#endif

/* 0x20b380 and 0x20b340: copy `count` 16-byte blocks or words. */
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
extern int *RhMem_CopyMemory4(int *destination, int *source, int count);
/* 0x20d1d0: two weights in float registers, two words; a word result. */
extern int RhGeom_RotationInterpolate(float first_rate, float second_rate, int first, int second);
/* 0x20d240: argument registers as 0x2106e0 passes them. */
extern void RhGeom_RoationComplexInterpolate(int *out, int *first, int *second, float first_rate, float second_rate);
/* 0x210cf0: parameter order and types from the registers used; names are descriptive. `data`
 * is a DotrMotionItem or DotrMotionShortItem by the pad flags. Returns `flags` with bit 0, 1, 2
 * cleared for each track present. */
unsigned int lRhModel_GetInterpolateMotionItem(void *data, float frame, unsigned int flags, float *position, int *rotation, float *scale, float rate);
/* 0x2118f0: frame is compared unsigned with the key times. model_setup.h declares this
 * function for its callers with (int frame, unsigned short flags); the definition needs
 * unsigned int flags for the exact code. Which type the original used is unresolved. */
unsigned int lRhModel_GetPulseMotionItem(void *data, unsigned int frame, unsigned int flags, float *position, int *rotation, float *scale, float rate);
/* 0x2106e0: parameter order and types from the registers used; names are descriptive. */
unsigned int lRhModel_GetMultiMotionItem(void *item0, void *item1, float frame0, float rate0, float frame1, float rate1,
                                         unsigned int flags, float *position, int *rotation, float *scale, float rate);

#endif
