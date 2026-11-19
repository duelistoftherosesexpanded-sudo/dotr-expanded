#ifndef DOTR_EE_TEXT_LOAD_H
#define DOTR_EE_TEXT_LOAD_H

/* Load-completion callback of the SzText loader that adds the blocks of a loaded text or texture
 * file to the SzText table.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* An address kept as a 32-bit word in an entry. The host build widens it so the test can
 * point it at host data (see DotrBuildWord in build_deck.h). */
#ifdef __MWERKS__
typedef int DotrTextLoadWord;
#else
typedef long DotrTextLoadWord;
#endif

/* View of the 12-byte SzText entry (entry_table.h struct DotrEntryTableEntry has the same
 * layout with int words). Assumed: +0 holds the loaded data address (nonzero marks the entry
 * as used), +4 an allocated address freed on failure; the halfword at +8 holds three one-bit
 * fields and a signed 13-bit group, +0xa the id. */
struct DotrTextLoadEntry {
    DotrTextLoadWord unknown_000;
    DotrTextLoadWord unknown_004;
    short flag_0 : 1;
    short flag_1 : 1;
    short flag_2 : 1;
    short group : 13;
    short id;
};

/* View of the read request record handed to the callback (resource_submit.h,
 * texture_callbacks.h and model_merge.h have other views). Assumed: +8 a 31-bit byte count of
 * the data read, the top bit of the word at +0xc set when the buffer is owned by the request,
 * +0x14 the loaded data, +0x1c the SzText entry. On the host the pointers move the later
 * offsets. */
struct DotrTextLoadRecord {
    int unknown_00[2];
    int byte_size : 31;
    int unknown_0c : 31;
    int owned : 1;
    int unknown_10;
    int *data;
    int unknown_18;
    struct DotrTextLoadEntry *entry;
};

/* Halfword ids used for kind-1 blocks, indexed by block number. */
extern unsigned short picpackArts[];

/* Callees as their groups declare them (memory_ops.h, object_table.h); SzMem_Free (0x217b50)
 * and SzText_AddFile (0x202810, entry_table.h) take the address as DotrTextLoadWord here so
 * the host can pass pointers; on the target the word is an int as in entry_table.h. */
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
extern void RhText_BindAddress(void *header);
extern void SzMem_Free(DotrTextLoadWord address);
extern int SzText_AddFile(int group, int id, const char *name, DotrTextLoadWord srcImageAddress);

/* Fixed destination 0xb1b000 + offset for copied kind-0/2 blocks. MATCHING DEVICE: the long
 * literal. The host keeps the same numeric address (never dereferenced: the host test stubs
 * the copy). */
#ifdef __MWERKS__
#define DOTR_TEXT_LOAD_COPY_AT(offset) ((int *)(int)((offset) + 0xb1b000L))
#else
#include <stdint.h>
#define DOTR_TEXT_LOAD_COPY_AT(offset) ((int *)(intptr_t)((offset) + 0xb1b000L))
#endif

/* 0x2024d0. */
void lSzText_LoadFileCB(void *record, int completed);

#endif
