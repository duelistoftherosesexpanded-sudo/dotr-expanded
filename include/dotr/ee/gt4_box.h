#ifndef DOTR_EE_GT4_BOX_H
#define DOTR_EE_GT4_BOX_H

/* Declarations for the GT4_BOX_IN__2DMAP original-file unit (src/ee/gt4_box/func_001095d0.c;
 * docs/modules/other.md). Only the prototypes the unit uses are kept. Types and signatures are
 * assumed views, not recovered declarations. This file holds the basic types and the shared
 * prefix. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;

#ifndef NULL
#define NULL 0
#endif

/* Sony's SDK libraries build with an EE GCC 2.9, which has no __int128; its 128-bit integer is
 * the TI machine mode. */
#if defined(__GNUC__) && !defined(__MWERKS__)
typedef int dk_int128 __attribute__((mode(TI)));
#define __int128 dk_int128
#endif

/* MW_ALIGN(n): an aligned home slot for a spilled Metrowerks parameter. The prototypes in
 * functions.h use it, and GCC rejects alignment on a parameter, so it is empty outside Metrowerks. */
#if defined(__MWERKS__)
#define MW_ALIGN(n) __attribute__((aligned(n)))
#else
#define MW_ALIGN(n)
#endif

#include "dotr/ee/gt4_box/structs.h"
#include "dotr/ee/gt4_box/game.h"
#include "dotr/ee/gt4_box/functions.h"

#endif
