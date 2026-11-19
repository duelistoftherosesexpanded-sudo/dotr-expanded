#ifndef DOTR_TYPES_H
#define DOTR_TYPES_H

#include <stdint.h>

/* Fixed-width storage only. Host pointers are not target addresses. */
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef u32 DotrTargetAddress;

_Static_assert(sizeof(DotrTargetAddress) == 4, "PS2 target address width");

#endif
