#ifndef DOTR_EE_MODEL_BIND_H
#define DOTR_EE_MODEL_BIND_H

/* lRhModel_* walkers over the tagged offset lists of a loaded model block (tag 0x2 = offset
 * into the first copy, 0x4 = into the second), called from lRhModel_DrawCommonPart.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

#include "dotr/ee/model_draw.h"

/* The header 0x20faf0 reaches through the scratchpad is assumed to be the one 16 bytes into
 * the block (both functions form block + 16 and read these words from it). */
#define DOTR_MODEL_BLOCK_HEAD(block) ((struct DotrModelHead *)((unsigned char *)(block) + 16))
/* Table at block + option_table: 32 list offsets (relative to the table, zero when absent)
 * and a mask word at +0x80. */
struct DotrModelOptionTable {
    int list[32];
    int mask;
};

/* 0x20fed0. Walks the words at block + bind_list (flag zero) or block + bind_list_second up
 * to a -1 word. A word tagged 0x2 or 0x4 in its top four bits names a slot at second + its
 * low 28 bits; when the slot's own word is tagged 0x2 or 0x4 it is replaced by the address
 * first + its low 28 bits. Other words and slots are left alone. The original also counts
 * the words in a register that is never read.
 * Assumed: void result; parameter types as declared in model_draw.h from the call sites. */
void lRhModel_DynamicAddressBind(struct DotrPatchHead *block, unsigned char *first, unsigned char *second, int flag);

/* 0x212170. Does nothing when option_table or option_enabled is zero. Otherwise, for each of
 * the 32 lists of the table whose bit is clear in the mask (the fifth argument, or the
 * table's own mask when flag is non-zero) and whose offset is non-zero, walks the words up
 * to a zero word: tag 0x2 names four words at first + low 28 bits, tag 0x4 four words at
 * second + low 28 bits (skipped when second is null); they are set to 0x10000000, 0, 0, 0.
 * Assumed: void result; int mask (only bit tests). */
void lRhModel_SetupOptionObject(struct DotrPatchHead *block, unsigned char *first, unsigned char *second, int flag, int mask);

#endif
