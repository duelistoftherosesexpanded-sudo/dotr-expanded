#ifndef DOTR_EE_SETUP_CARD_H
#define DOTR_EE_SETUP_CARD_H

/* Declarations for the SzDuel_SetupCard_Pipe original-file unit (src/ee/setup_card/func_001da7f0.c;
 * docs/modules/duel.md), trimmed to what the unit uses. The views and prototypes are
 * assumptions, not recovered declarations.
 * Types and shared prefix. */

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

/* Sony's SDK libraries build with an EE GCC 2.9, which has no
 * __int128; its 128-bit integer is the TI machine mode. */
#if defined(__GNUC__) && !defined(__MWERKS__)
typedef int dk_int128 __attribute__((mode(TI)));
#define __int128 dk_int128
#endif

/* MW_ALIGN(n): an aligned home for a spilled Metrowerks parameter. GCC rejects alignment
 * on a parameter, so it is empty outside Metrowerks. */
#if defined(__MWERKS__)
#define MW_ALIGN(n) __attribute__((aligned(n)))
#else
#define MW_ALIGN(n)
#endif

#include "dotr/ee/setup_card/structs.h"
#include "dotr/ee/setup_card/game.h"
#include "dotr/ee/setup_card/functions.h"

#endif
