#ifndef DOTR_EE_PACKED_REBASE_H
#define DOTR_EE_PACKED_REBASE_H

/* Functions that turn tagged offsets inside a packed header into addresses (0x20f370: list at
 * the offset in +0x50, segment offsets at +0x18/+0x20/+0x28/+0x30); same header family as
 * RhText_BindAddress in small_misc.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* A word that holds a tagged offset (tag in the top four bits, offset in the low 28) and is
 * replaced by an address: 32 bits on the target, pointer-sized on the host. */
#ifdef __MWERKS__
typedef unsigned DotrRebaseWord;
#else
#include <stdint.h>
typedef uintptr_t DotrRebaseWord;
#endif

/* Header view from 0x20f370 alone: eight-byte records from +0x10 whose first word is a byte
 * offset from the header (records 1..4 are used for tags 2..5), and the byte offset of a
 * 0xffffffff-terminated word list at +0x50. Names assumed; other bytes unobserved. */
typedef struct DotrRebaseSegment {
    int offset;
    int unknown_04;
} DotrRebaseSegment;
typedef struct DotrRebaseHeader {
    unsigned char unknown_00[0x10];
    DotrRebaseSegment segment[5];
    unsigned char unknown_38[0x18];
    int list;
} DotrRebaseHeader;
int RhModel_BindAddress(DotrRebaseHeader *header);

#endif
