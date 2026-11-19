#ifndef DOTR_EE_SHOKI_OBJ_H
#define DOTR_EE_SHOKI_OBJ_H

/* SHOKI_OBJ, which takes and clears the first free 192-byte object slot of a work block.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* func_00195108 (library block, declared only): clears b bytes at a, as other groups
 * assume. */
extern void func_00195108(void *a, int b);

/* One 192-byte object slot; the halfword at +164 is its id (0 = free). The other fields
 * are unknown. */
struct DotrShokiObj {
    char unknown_00[164];
    short id;
    char unknown_a6[26];
};
/* The work block whose object slots start at +3168; the number of slots is the caller's
 * count (the array bound here is only a view). */
struct DotrShokiWork {
    char unknown_000[3168];
    struct DotrShokiObj obj[1];
};

/* 0x106a00 SHOKI_OBJ: when skip is 0, finds the first of count slots with id 0, clears its
 * 192 bytes, sets its id to its index + 1 and returns it; returns 0 when skip is not 0 or no
 * slot is free. Assumed: four int-sized parameters, the second unused. */
struct DotrShokiObj *SHOKI_OBJ(struct DotrShokiWork *w, int unused, int count, int skip);

#endif
